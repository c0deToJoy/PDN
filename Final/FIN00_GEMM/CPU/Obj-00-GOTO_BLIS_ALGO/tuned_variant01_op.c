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
      int n0_fringe_start = n0 - (n0%(BLOCK_NC));

      // Steady State
      for( int j0_o = 0; j0_o < n0_fringe_start; j0_o += BLOCK_NC )
	for( int p0 = 0; p0 < k0; ++p0 )
	  for( int i0 = 0; i0 < m0; ++i0 )
	    for( int j0_i = 0; j0_i < BLOCK_NC; ++j0_i )
	      {
		int j0 = j0_o + j0_i;
		float A_ip = A_distributed[i0 * cs_A + p0 * rs_A];
		float B_pj = B_distributed[p0 * cs_B + j0 * rs_B];

		C_distributed[i0 * cs_C + j0 * rs_C]  += A_ip*B_pj;
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


