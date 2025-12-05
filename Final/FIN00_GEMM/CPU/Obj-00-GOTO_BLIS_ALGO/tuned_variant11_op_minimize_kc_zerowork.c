/*
  This is an incremental implementation of the GOTO/BLIS Algorithm
  for General Matrix times Matrix Multilication (GEMM)

  Details for this algorithm and similar can be found here:


  # Layering for memory
  Anatomy of High-Performance Matrix Multiplication
  https://dl.acm.org/doi/pdf/10.1145/1356052.1356053

  BLIS: A Framework for Rapidly Instantiating BLAS Functionality
  https://dl.acm.org/doi/pdf/10.1145/2764454

  The BLIS Framework: Experiments in Portability
  https://dl.acm.org/doi/pdf/10.1145/2755561

  https://dl.acm.org/doi/pdf/10.1145/2925987
  Analytical Modeling Is Enough for High-Performance BLIS

  # Instruction Level parallelism and SIMD
  Automating the Last-Mile for High Performance Dense Linear Algebra
  https://arxiv.org/pdf/1611.08035

  # Shared Memory
  Anatomy of High-Performance Many-Threaded Matrix Multiplication
  https://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=6877334


  # Distributed Mem
  Collective communication: theory, practice, and experience
  https://www.cs.utexas.edu/~pingali/CSE392/2011sp/lectures/Conc_Comp.pdf

  SUMMA: Scalable Universal Matrix Multiplication Algorithm
  https://onlinelibrary.wiley.com/doi/abs/10.1002/%28SICI%291096-9128%28199704%299%3A4%3C255%3A%3AAID-CPE250%3E3.0.CO%3B2-2

  PARALLEL MATRIX MULTIPLICATION: A SYSTEMATIC JOURNEY
  https://www.cs.utexas.edu/~flame/pubs/2D3DFinal.pdf



  - richard.m.veras@ou.edu

*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef COMPUTE_NAME
#define COMPUTE_NAME baseline
#endif

#ifndef DISTRIBUTE_DATA_NAME
#define DISTRIBUTE_DATA_NAME baseline_distribute
#endif

#ifndef COLLECT_DATA_NAME
#define COLLECT_DATA_NAME baseline_collect
#endif


#ifndef DISTRIBUTED_ALLOCATE_NAME
#define DISTRIBUTED_ALLOCATE_NAME baseline_allocate
#endif


#ifndef DISTRIBUTED_FREE_NAME
#define DISTRIBUTED_FREE_NAME baseline_free
#endif

/*
 C-Pre-Processor (cpp) macros for blocksizes will allows us
 to have constant values at compile time that we can either
 set here, or pass as a compiler flag "-DBLOCK_NC=512".
*/

#ifndef BLOCK_NC
#define BLOCK_NC 192
#endif /* BLOCK_NC */

#ifndef BLOCK_KC
#define BLOCK_KC 128
#endif /* BLOCK_KC */

#ifndef BLOCK_MC
#define BLOCK_MC 128
#endif /* BLOCK_MC */

#ifndef BLOCK_NR
#define BLOCK_NR 6
#endif /* BLOCK_NR */


#ifndef BLOCK_MR
#define BLOCK_MR 16
#endif /* BLOCK_MR */

/* ASSERTIONS: We want block sizes to nest perfectly. */
#if (BLOCK_NC) % (BLOCK_NR) != 0
#error "NR must be a factor of NC.\n"
#endif

#if (BLOCK_MC) % (BLOCK_MR) != 0
#error "MR must be a factor of MC.\n"
#endif


// https://stackoverflow.com/questions/3437404/min-and-max-in-c
#define min(a,b)				\
  ({ __typeof__ (a) _a = (a);			\
    __typeof__ (b) _b = (b);			\
    _a < _b ? _a : _b; })

#define max(a,b)				\
  ({ __typeof__ (a) _a = (a);			\
    __typeof__ (b) _b = (b);			\
    _a > _b ? _a : _b; })

void COMPUTE_NAME( int m0, int n0, int k0,
		   int rs_A, int cs_A,
		   int rs_B, int cs_B,
		   int rs_C, int cs_C,
		   float *A_distributed,
		   float *B_distributed,
		   float *C_distributed )

{
  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;


  MPI_Comm_rank(MPI_COMM_WORLD, &rid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if(rid == root_rid )
    {
      // We will reduce the fringe to only include
      // what spills out of NR.
      int n0_fringe_start = n0 - (n0%(BLOCK_NR));
      int k0_fringe_start = k0 - (k0%(BLOCK_KC));
      // We will reduce the fringe to only include
      // what spills out of MR.
      int m0_fringe_start = m0 - (m0%(BLOCK_MR));

      int num_j0_i_blocks = (BLOCK_NC) / (BLOCK_NR);
      int num_i0_i_blocks = (BLOCK_MC) / (BLOCK_MR);


      /*
	Start of Steady State

      */
      // Steady State
      for( int j0_o = 0; j0_o < n0_fringe_start; j0_o += BLOCK_NC )
	{
	  int block_nc_remainder = min(BLOCK_NC, n0_fringe_start-j0_o);
	  
	  // Steady State
	  for( int p0_o = 0; p0_o < k0; p0_o += BLOCK_KC )
	    {
	      // When we pack B and A we will pad the buffers
	      // with zeros instead of dealing with the fringe
	      // case.
	      // This should be enough, but we can also
	      // pass the size of remaining kc block to the kernel
	      // and short circuit the computation. The complication
	      // there is we might want to unroll that loop, so we
	      // will have a small fringe.
	      int block_kc_remainder = min(BLOCK_KC, k0-p0_o);
	      
	      // We will pack and perform a Data layout transformation
	      // on a KCxNC block of B
	      // This step will take O(n^2) time and will be amortized
	      // by O(n^3) of FMA work.
	      // The layout of B_dlt will mirror the order that the micro-
	      // kernel will use to access elements.
	      //   B_dlt[j0_i/NR][p0_i][j0_r] <-- B[p0_o][j0_o]
	      // Right now we will use a multi-dimension C array, but
	      // later on we will flatten it.

	      float B_dlt[num_j0_i_blocks][BLOCK_KC][BLOCK_NR];
	      for( int j0_i = 0; j0_i < BLOCK_NC; j0_i += BLOCK_NR )
		for( int p0_i = 0; p0_i < BLOCK_KC; ++p0_i )
		  for( int j0_r = 0; j0_r < BLOCK_NR; ++j0_r  )
		    {
		      int j0 = j0_o + j0_i + j0_r;
		      int p0 = p0_o + p0_i;

		      int j0_i_bid = j0_i/(BLOCK_NR);

		      // If we are not in the fringe then pack the value
		      // otherwise pad with 0.
		      if (p0 < k0 & j0 < n0 )
			B_dlt[j0_i_bid][p0_i][j0_r] =
			  B_distributed[p0 * cs_B + j0 * rs_B];
		      else
			B_dlt[j0_i_bid][p0_i][j0_r] = 0.0f;
		    }


	      // Steady State
	      for( int i0_o = 0; i0_o < m0_fringe_start; i0_o += BLOCK_MC )
		{
		  // How much of an MR divisible block do we have left?
		  // This will be passed to the kernel.
		  int block_mc_remainder = min(BLOCK_MC, m0_fringe_start - i0_o);
		  
		  // We will pack and perform a Data layout transformation
		  // on a MCxKC block of A. Like the packing of B, this work
		  // will be amortized by a lot of computation.
		  // A_dlt[i0_i/MR][p0_i][i0_r]
		  float A_dlt[num_i0_i_blocks][BLOCK_KC][BLOCK_MR];

		  for( int i0_i = 0; i0_i < BLOCK_MC; i0_i += BLOCK_MR  )
		    for( int p0_i = 0; p0_i < BLOCK_KC; ++p0_i )
		      for( int i0_r = 0; i0_r < BLOCK_MR; ++i0_r  )
			{
			  int i0 = i0_o + i0_i + i0_r;
			  int p0 = p0_o + p0_i;
			  int i0_i_bid = i0_i/(BLOCK_MR);

			  if (p0 < k0  & i0 < m0)
			    A_dlt[i0_i_bid][p0_i][i0_r] =
			      A_distributed[i0 * cs_A + p0 * rs_A];
			  else
			    A_dlt[i0_i_bid][p0_i][i0_r] = 0.0f;
			}

		  // KERNEL: Performs a MCxNCxKC matrix-matrix multiplication
		  for( int j0_i = 0; j0_i < block_nc_remainder; j0_i += BLOCK_NR )
		    for( int i0_i = 0; i0_i < block_mc_remainder; i0_i += BLOCK_MR  )
		      // MICRO-KERNEL: Performs a MRxNRxKC matrix-matrix multiplication
		      {
			// We are going to keep a small NRxMR block of C's updates
			// then write them back to memory when we are done.
			float C_micro[BLOCK_NR][BLOCK_MR];

			// Zero out C_micro
			// C_micro[][] = 0
			for( int j0_r = 0; j0_r < BLOCK_NR; ++j0_r  )
			  for( int i0_r = 0; i0_r < BLOCK_MR; ++i0_r  )
			    {
			      C_micro[j0_r][i0_r] = 0.0f;
			    }

			// Rank-K update (lots of very parallel outer-products)
			// This is where all of the work happens and we need
			// to use SIMD to get the peak floating point performance
			// however, the current layout for A and B is not amenable
			// to doing this efficiently. We will have to fix that
			// later. We will also need to partially unroll KC for ILP,
			// but that will also happen later.
			//
			// C_micro [ir][jr] += A_{ir,p} * B_{p,jr}
			for( int p0_i = 0; p0_i < block_kc_remainder; ++p0_i )
			  for( int j0_r = 0; j0_r < BLOCK_NR; ++j0_r  )
			    for( int i0_r = 0; i0_r < BLOCK_MR; ++i0_r  )
			      {
				int j0 = j0_o + j0_i + j0_r;
				int i0 = i0_o + i0_i + i0_r;
				int p0 = p0_o + p0_i;

				int j0_i_bid = j0_i/(BLOCK_NR);
				int i0_i_bid = i0_i/(BLOCK_MR);

				float A_ip = A_dlt[i0_i_bid][p0_i][i0_r];
				float B_pj = B_dlt[j0_i_bid][p0_i][j0_r];

				C_micro[j0_r][i0_r] += A_ip*B_pj;
			      }

			// Update C[] with C_micro[][]
			for( int j0_r = 0; j0_r < BLOCK_NR; ++j0_r  )
			  for( int i0_r = 0; i0_r < BLOCK_MR; ++i0_r  )
			    {
			      int j0 = j0_o + j0_i + j0_r;
			      int i0 = i0_o + i0_i + i0_r;

			      C_distributed[i0 * cs_C + j0 * rs_C] += C_micro[j0_r][i0_r];
			    }

		      }
		}
	      /*
		End of Steady State
	      */

	      // Fringe for m0
	      for( int i0 = m0_fringe_start; i0 < m0; ++i0 )
		for( int j0_i = 0; j0_i < block_nc_remainder; ++j0_i )
		  for( int p0_i = 0; p0_i < block_kc_remainder; ++p0_i )
		    {
		      int j0 = j0_o + j0_i;
		      int p0 = p0_o + p0_i;
		      float A_ip = A_distributed[i0 * cs_A + p0 * rs_A];
		      float B_pj = B_distributed[p0 * cs_B + j0 * rs_B];

		      C_distributed[i0 * cs_C + j0 * rs_C]  += A_ip*B_pj;
		    }

	    }
	}
      // Fringe for n0
      for( int j0 = n0_fringe_start; j0 < n0; ++j0 )
	for( int p0 = 0; p0 < k0; ++p0 )
	  for( int i0 = 0; i0 < m0; ++i0 )
	    {
	      float A_ip = A_distributed[i0 * cs_A + p0 * rs_A];
	      float B_pj = B_distributed[p0 * cs_B + j0 * rs_B];
	      C_distributed[i0 * cs_C + j0 * rs_C]  += A_ip*B_pj;
	    }

	}
  else
    {
      /* STUDENT_TODO: Modify this is you plan to use more
	 than 1 rank to do work in distributed memory context. */
    }
}


// Create the buffers on each node
void DISTRIBUTED_ALLOCATE_NAME( int m0, int n0, int k0,
				int rs_A, int cs_A,
				int rs_B, int cs_B,
				int rs_C, int cs_C,
				float **A_distributed,
				float **B_distributed,
				float **C_distributed )
{
  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if(rid == root_rid )
    {

      *A_distributed=(float *)malloc(sizeof(float)*m0*k0);
      *B_distributed=(float *)malloc(sizeof(float)*k0*n0);
      *C_distributed=(float *)malloc(sizeof(float)*m0*n0);
    }
  else
    {
      /*
	STUDENT_TODO: Modify this is you plan to use more
	than 1 rank to do work in distributed memory context.

	Note: In the original configuration only rank with
	rid == 0 has all of its buffers allocated.
      */

    }
}


void DISTRIBUTE_DATA_NAME( int m0, int n0, int k0,
			   int rs_A, int cs_A,
			   int rs_B, int cs_B,
			   int rs_C, int cs_C,
			   float *A_sequential,
			   float *B_sequential,
			   float *C_sequential,
			   float *A_distributed,
			   float *B_distributed,
			   float *C_distributed )
{

  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;

  // Layout for sequential data
  int rs_AS = rs_A;
  int cs_AS = cs_A;

  int rs_BS = rs_B;
  int cs_BS = cs_B;

  int rs_CS = rs_C;
  int cs_CS = cs_C;

  MPI_Comm_rank(MPI_COMM_WORLD, &rid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if(rid == root_rid )
    {

      // Note: Here is a perfect opportunity to change the layout
      //       of your data which has the potential to give you
      //       a sizeable performance gain.
      //
      //       If you change the layout here, make sure you use
      //       the correct strides for the new layout in compute
      // Layout for distributed data
      int rs_AD = rs_A;
      int cs_AD = cs_A;
      int rs_BD = rs_B;
      int cs_BD = cs_B;
      int rs_CD = rs_C;
      int cs_CD = cs_C;


      // Distribute A
      for( int i0 = 0; i0 < m0; ++i0 )
	for( int p0 = 0; p0 < k0; ++p0 )
	  {
	    A_distributed[i0 * cs_AD + p0 * rs_AD] =
	      A_sequential[i0 * cs_AS + p0 * rs_AS];
	  }

      // Distribute B
      for( int p0 = 0; p0 < k0; ++p0 )
	for( int j0 = 0; j0 < n0; ++j0 )
	  {
	    B_distributed[p0 * cs_BD + j0 * rs_BD] =
	      B_sequential[p0 * cs_BS + j0 * rs_BS];
	  }

      for( int i0 = 0; i0 < m0; ++i0 )
	for( int j0 = 0; j0 < n0; ++j0 )
	  {
	    C_distributed[i0 * cs_CD + j0 * rs_CD] =
	      C_sequential[i0 * cs_CS + j0 * rs_CS];
	  }

    }
  else
    {
      /*
	STUDENT_TODO: Modify this is you plan to use more
	than 1 rank to do work in distributed memory context.

	Note: In the original configuration only rank with
	rid == 0 has all of the necessary data for the computation.
	All other ranks have garbage in their data. This is where
	rank with rid == 0 needs to SEND data to the other nodes
	to RECEIVE the data, or use COLLECTIVE COMMUNICATION to
	distribute the data.
      */

    }

}



void COLLECT_DATA_NAME( int m0, int n0,
			int rs_C, int cs_C,
			float *C_distributed,
			float *C_sequential )
{
  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;

  // Layout for sequential data
  // A is column major
  // C is column major
  int rs_CS = rs_C;
  int cs_CS = cs_C;



  MPI_Comm_rank(MPI_COMM_WORLD, &rid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if(rid == root_rid )
    {
      // Note: Here is a perfect opportunity to change the layout
      //       of your data which has the potential to give you
      //       a sizeable performance gain.
      // Layout for distributed data

      int rs_CD = rs_C;
      int cs_CD = cs_C;

      // Collect the output
      for( int i0 = 0; i0 < m0; ++i0 )
	for( int j0 = 0; j0 < n0; ++j0 )
	C_sequential[i0 * cs_CS + j0 * rs_CS] =
	  C_distributed[i0 * cs_CD + j0 * rs_CD];
    }
  else
    {
      /*
	STUDENT_TODO: Modify this is you plan to use more
	than 1 rank to do work in distributed memory context.

	Note: In the original configuration only rank with
	rid == 0 performs the computation and copies the
	"distributed" data to the "sequential" buffer that
	is checked by the verifier on rank rid == 0. If the
	other ranks contributed to the computation, then
	rank rid == 0 needs to RECEIVE the contributions that
	the other ranks SEND, or use COLLECTIVE COMMUNICATIONS
	for the same result.
      */

    }

}




void DISTRIBUTED_FREE_NAME( int m0, int n0, int k0, int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
		   float *A_distributed,
		   float *B_distributed,
		   float *C_distributed
			    )
{
  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if(rid == root_rid )
    {

      free(A_distributed);
      free(B_distributed);
      free(C_distributed);
    }
  else
    {
      /*
	STUDENT_TODO: Modify this is you plan to use more
	than 1 rank to do work in distributed memory context.

	Note: In the original configuration only rank with
	rid == 0 allocates the "distributed" buffers for itself.
	If the other ranks were modified to allocate their own
	buffers then they need to be freed at the end.
      */

    }

}
