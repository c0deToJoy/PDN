/*
  This is the baseline implementation of a Triangular Matrix Times Matrix
  Multiplication  (TRMM)

  C = AB, where
  A is an MxM lower triangular (A_{i,p} = 0 if p > i) Matrix. It is indexed by i0 and p0
  B is an MxN matrix. It is indexed by p0 and j0.
  C is an MxN matrix. It is indexed by i0 and j0.


  Parameters:

  m0 > 0: dimension
  n0 > 0: dimension



  float* A_sequential: pointer to original A matrix data
  float* A_distributed: pointer to the input data that you have distributed across
  the system

  float* C_sequential:  pointer to original output data
  float* C_distributed: pointer to the output data that you have distributed across
  the system

  float* B_sequential:  pointer to original weights data
  float* B_distributed: pointer to the weights data that you have distributed across
  the system

  Functions:

  DISTRIBUTED_ALLOCATE_NAME(...): Allocate the distributed buffers.
  DISTRIBUTE_DATA_NAME(...): takes the sequential data and distributes it across the system.
  COMPUTE_NAME(...): Performs the stencil computation.
  COLLECT_DATA_NAME(...): Collect the distributed output and combine it back to the sequential
  one for testing.
  DISTRIBUTED_FREE_NAME(...): Free the distributed buffers that were allocated


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
      for( int j0 = 0; j0 < n0; ++j0 )
	{
	  for( int i0 = 0; i0 < m0; ++i0 )
	    {
	      for( int p0 = 0; p0 < k0; ++p0 )
		{
		  float A_ip = A_distributed[i0 * cs_A + p0 * rs_A];
		  float B_pj = B_distributed[p0 * cs_B + j0 * rs_B];

		  C_distributed[i0 * cs_C + j0 * rs_C]  += A_ip*B_pj;
		}
	    }
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


