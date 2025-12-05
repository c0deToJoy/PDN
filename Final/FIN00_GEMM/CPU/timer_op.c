/*
  Timer harness for running a "function under test" for num_runs number of
  runs.


*/
#include <float.h>
#include <limits.h>
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#include "timer.h"

// Function under test
extern void COMPUTE_NAME_REF( int m0, int n0, int k0,
			      int rs_A, int cs_A,
			      int rs_B, int cs_B,
			      int rs_C, int cs_C,
			      float *A_distributed,
			      float *B_distributed,
			      float *C_distributed );

extern void COMPUTE_NAME_TST( int m0, int n0, int k0,
			      int rs_A, int cs_A,
			      int rs_B, int cs_B,
			      int rs_C, int cs_C,
			      float *A_distributed,
			      float *B_distributed,
			      float *C_distributed );


extern void DISTRIBUTED_ALLOCATE_NAME_REF( int m0, int n0, int k0,
					   int rs_A, int cs_A,
					   int rs_B, int cs_B,
					   int rs_C, int cs_C,
					   float **A_distributed,
					   float **B_distributed,
					   float **C_distributed );

extern void DISTRIBUTED_ALLOCATE_NAME_TST( int m0, int n0, int k0,
					   int rs_A, int cs_A,
					   int rs_B, int cs_B,
					   int rs_C, int cs_C,
					   float **A_distributed,
					   float **B_distributed,
					   float **C_distributed );



extern void DISTRIBUTE_DATA_NAME_REF( int m0, int n0, int k0,
				      int rs_A, int cs_A,
				      int rs_B, int cs_B,
				      int rs_C, int cs_C,
				      float *A_sequential,
				      float *B_sequential,
				      float *C_sequential,
				      float *A_distributed,
				      float *B_distributed,
				      float *C_distributed );

extern void DISTRIBUTE_DATA_NAME_TST( int m0, int n0, int k0,
				      int rs_A, int cs_A,
				      int rs_B, int cs_B,
				      int rs_C, int cs_C,
				      float *A_sequential,
				      float *B_sequential,
				      float *C_sequential,
				      float *A_distributed,
				      float *B_distributed,
				      float *C_distributed );




extern void COLLECT_DATA_NAME_REF( int m0, int n0,
				   int rs_C, int cs_C,
				   float *C_distributed,
				   float *C_sequential );

extern void COLLECT_DATA_NAME_TST( int m0, int n0,
				   int rs_C, int cs_C,
				   float *C_distributed,
				   float *C_sequential );




extern void DISTRIBUTED_FREE_NAME_REF( int m0, int n0, int k0,
				       int rs_A, int cs_A,
				       int rs_B, int cs_B,
				       int rs_C, int cs_C,
				       float *A_distributed,
				       float *B_distributed,
				       float *C_distributed );

extern void DISTRIBUTED_FREE_NAME_TST( int m0, int n0, int k0,
				       int rs_A, int cs_A,
				       int rs_B, int cs_B,
				       int rs_C, int cs_C,
				       float *A_distributed,
				       float *B_distributed,
				       float *C_distributed );


void fill_buffer_with_random( int num_elems, float *buff )
{
    for(int i = 0; i < num_elems; ++i)
	buff[i] = ((float)(rand()-((RAND_MAX)/2)))/((float)RAND_MAX);
}

void fill_buffer_with_value( int num_elems, float val, float *buff )
{
    for(int i = 0; i < num_elems; ++i)
	buff[i] = val;
}

double pick_min_in_list(int num_trials, double *results)
{
  double current_min = DBL_MAX;

  for( int i = 0; i < num_trials; ++i )
    if( results[i] < current_min )
      current_min = results[i];

  return current_min;
}

long pick_max_in_list(int num_trials, long *results)
{
  long current_max = LONG_MIN;

  for( int i = 0; i < num_trials; ++i )
    if( results[i] > current_max )
      current_max = results[i];

  return current_max;
}


void flush_cache()
{

  int size = 1024*1024*8;

  int *buff = (int *)malloc(sizeof(int)*size);
  int i, result = 0;
  volatile int sink;
  for (i = 0; i < size; i ++)
    result += buff[i];
  sink = result; /* So the compiler doesn't optimize away the loop */

  free(buff);
}

void time_function_under_test(int num_trials,
			      int num_runs_per_trial,
			      double *results, // results from each trial
			      int m0, int n0, int k0, int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
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

  // Initialize the start and stop variables.
  TIMER_INIT_COUNTERS(stop, start);

  // Click the timer a few times so the subsequent measurements are more accurate
  MPI_Barrier(MPI_COMM_WORLD);
  TIMER_WARMUP(stop,start);

  // flush the cache
  flush_cache();
  MPI_Barrier(MPI_COMM_WORLD);

  for(int trial = 0; trial < num_trials; ++trial )
    {

	/*
	  Time code.
	*/
        // start timer
      TIMER_GET_CLOCK(start);

	////////////////////////
        // Benchmark the code //
	////////////////////////

	for(int runs = 0; runs < num_runs_per_trial; ++runs )
	  {
	    COMPUTE_NAME_TST( m0, n0, k0, rs_A, cs_A, rs_B, cs_B, rs_C, cs_C,
			      A_distributed,
			      B_distributed,
			      C_distributed );

	  }

	////////////////////////
        // End Benchmark      //
	////////////////////////


        // stop timer
	TIMER_GET_CLOCK(stop);

	// subtract the start time from the stop time
	TIMER_GET_DIFF(start,stop,results[trial]);


	// Use the longest individual rank's runtime as the measured time.
	double max_time;
	MPI_Reduce(
		   &results[trial],
		   &max_time,
		   1,
		   MPI_DOUBLE,
		   MPI_MAX,
		   root_rid,
		   MPI_COMM_WORLD);

	if( rid == root_rid )
	  {
	    results[trial] = max_time;
	  }




    }

}


int scale_p_on_pos_ret_v_on_neg(int p, int v)
{
  if (v < 1)
    return -1*v;
  else
    return v*p;
}



int main( int argc, char *argv[] )
{
  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;

  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rid);
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  // What we will output to
  FILE *result_file;

  int num_trials = 3;
  int num_runs_per_trial = 1;

  // Problem parameters
  int min_size;
  int max_size;
  int step_size;

  int in_m0;
  int in_n0;
  int in_k0;

  int in_rs_A;
  int in_cs_A;
  int in_rs_B;
  int in_cs_B;
  int in_rs_C;
  int in_cs_C;

  // Get command line arguments

    if(argc == 1 )
    {
      min_size  = 16;
      max_size  = 256;
      step_size = 16;

      // defaults
      in_m0=1;
      in_n0=1;
      in_k0=1;

      // default to printing to stdout
      result_file = stdout;

      // ALL Col-Major
      in_rs_A = 1;
      in_cs_A = -1;
      in_rs_B = 1;
      in_cs_B = -1;
      in_rs_C = 1;
      in_cs_C = -1;
    }
    else if(argc == 12 + 1 || argc == 13 + 1 )
      {
	min_size  = atoi(argv[1]);
	max_size  = atoi(argv[2]);
	step_size = atoi(argv[3]);

	in_m0=atoi(argv[4]);
	in_n0=atoi(argv[5]);
	in_k0=atoi(argv[6]);

	in_rs_A = atoi(argv[7]);
	in_cs_A = atoi(argv[8]);
	in_rs_B = atoi(argv[9]);
	in_cs_B = atoi(argv[10]);
	in_rs_C = atoi(argv[11]);
	in_cs_C = atoi(argv[12]);

	// default to printing to stdout
	result_file = stdout;

	if(argc == 13 + 1)
	  {
	    // we don't want every node opening the same file
	    // to write to.
	    if(rid == 0 )
	      {
		result_file = fopen(argv[13],"w");
	      }
	    else
	      {
		result_file = NULL;
	      }
	  }
      }
    else
      {
	printf("usage: %s min max step m0 n0 k0 rsa csa rsb csb rsc csc [filename]\n",
	       argv[0]);
	exit(1);
      }

  // Print out the first line of the output in csv format
  if( rid == 0 )
    {
      /*root node */
      fprintf(result_file, "num_ranks,m0,n0,k0,rsa,csa,rsb,csb,rsc,csc,result\n");
    }
  else
    {/* all other nodes*/ }


  for( int p = min_size;
       p < max_size;
       p += step_size )
    {

      // input sizes
      int m0=scale_p_on_pos_ret_v_on_neg(p,in_m0);
      int n0=scale_p_on_pos_ret_v_on_neg(p,in_n0);
      int k0=scale_p_on_pos_ret_v_on_neg(p,in_k0);
      
      int rs_A=scale_p_on_pos_ret_v_on_neg(p,in_rs_A);
      int cs_A=scale_p_on_pos_ret_v_on_neg(p,in_cs_A);
      int rs_B=scale_p_on_pos_ret_v_on_neg(p,in_rs_B);
      int cs_B=scale_p_on_pos_ret_v_on_neg(p,in_cs_B);
      int rs_C=scale_p_on_pos_ret_v_on_neg(p,in_rs_C);
      int cs_C=scale_p_on_pos_ret_v_on_neg(p,in_cs_C);

      // TODO: Should factor in non-unit strides here
      // How big of a buffer do we need
      int A_sequential_sz=m0*k0;
      int B_sequential_sz=k0*n0;
      int C_sequential_sz=m0*n0;

      float *A_sequential_tst = (float *)malloc(sizeof(float)*A_sequential_sz);
      float *C_sequential_tst = (float *)malloc(sizeof(float)*C_sequential_sz);
      float *B_sequential_tst = (float *)malloc(sizeof(float)*B_sequential_sz);


      if( rid == 0)
	{ /* root node */
	  // fill src_ref with random values
	  fill_buffer_with_random( A_sequential_sz, A_sequential_tst );
	  fill_buffer_with_random( B_sequential_sz, B_sequential_tst );
	  fill_buffer_with_value( C_sequential_sz, -1, C_sequential_tst );
	}
      else
	{/* all other nodes. */}


      // run the test
      float *A_distributed_tst;
      float *B_distributed_tst;
      float *C_distributed_tst;

      // Allocate distributed buffers for the reference
      DISTRIBUTED_ALLOCATE_NAME_TST( m0, n0,  k0, rs_A,  cs_A,  rs_B,  cs_B,  rs_C,  cs_C,
				     &A_distributed_tst,
				     &B_distributed_tst,
				     &C_distributed_tst );

      // Distribute the sequential buffers
      DISTRIBUTE_DATA_NAME_TST( m0, n0,  k0,
				rs_A,  cs_A,
				rs_B,  cs_B,
				rs_C,  cs_C,
				A_sequential_tst,
				B_sequential_tst,
				C_sequential_tst,
				A_distributed_tst,
				B_distributed_tst,
				C_distributed_tst );

      // Perform the computation
      double *results = (double*)malloc(sizeof(double)*num_trials);

      time_function_under_test(num_trials,
			       num_runs_per_trial,
			       results, // results from each trial
			       m0, n0, k0, rs_A, cs_A, rs_B, cs_B, rs_C, cs_C,
			       A_distributed_tst,
			       B_distributed_tst,
			       C_distributed_tst
			       );

      double min_res = pick_min_in_list(num_trials, results);

      double nanoseconds = ((double)min_res)/(num_runs_per_trial);

      // Number of floating point operations
      double num_flops = 2*((double)m0) *((double) n0) * ((double) k0);

      // This gives us throughput as GFLOP/s
      float throughput =  num_flops / nanoseconds;

      free(results);

      // Collect the distributed data and write it to a sequential buffer
      COLLECT_DATA_NAME_TST( m0, n0, rs_C, cs_C,
			     C_distributed_tst,
			     C_sequential_tst );

      // Finally free the buffers
      DISTRIBUTED_FREE_NAME_TST( m0, n0, k0, rs_A, cs_A, rs_B, cs_B, rs_C, cs_C,
				 A_distributed_tst,
				 B_distributed_tst,
				 C_distributed_tst );


      if( rid == 0)
	{
	  /* root node */

	  fprintf(result_file, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%.2f\n",
		  num_ranks,
		  m0,n0,k0,
		  rs_A, cs_A, rs_B, cs_B, rs_C, cs_C,
		  throughput);
	}
      else
	{/* all other nodes */}


      // Free the sequential buffers
      free(A_sequential_tst);
      free(C_sequential_tst);
      free(B_sequential_tst);
    }

  if( rid == 0)
    {
      /* root node */

      fclose(result_file);
    }
  else
    {/* all other nodes */}


  MPI_Finalize();
}

