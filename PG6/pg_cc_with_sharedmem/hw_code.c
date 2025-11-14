/*
  HW Shared Memory Collectives

  Instructions: find all instances of "STUDENT_TODO" and replace with 
                distributed memory code that makes the test corresponding 
                to that function pass.


		To compile and run your code locally on a machine with MPI installed  
		(https://rantahar.github.io/introduction-to-mpi/setup.html)	      
		Use:								      
		  make compile-local 						      
		  make run-local						      
		
		To access Schooner see: https://www.ou.edu/oscer/support/machine_access
		To compile and run on Schooner use:
		  make compile-schooner	    
		  make run-schooner		    

		



  Submission: For this assignment you will upload three artifacts to canvas. 
              DO NOT INCLUDE YOUR NAME or other identifying information in
              these artifacts.
              1. [figures.pdf] containing pictures describing the movements
                               being performed by these collectives.
	      2. [results.txt] containing the test output of your code.
	      3. [code.c] Your modified version of this code.


  - richard.m.veras@ou.edu
*/




/*
  You will be implementing the following collective patterns using shared memory and barriers.

  1. Broadcast
  2. Reduce
  3. Scatter
  4. Gather
  5. All Reduce
  6. All gather
  7. All to All


  Resources:
  - https://rookiehpc.org/openmp/docs/barrier/index.html
  - https://www3.nd.edu/~zxu2/acms60212-40212-S12/Lec-11-02.pdf
  
*/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <omp.h>


/*
  Helper functions
*/

void print_float_mem(char *name, int vlen, float *src)
{

  printf("%s = [ ",name);
  for( int vid = 0; vid < vlen; ++vid )
    {
      if ( src[vid] < 0.0f )
	printf( " x, ", src[vid] );
      else
	printf( "%2.f, ", src[vid] );
    }
    printf("]\n");
}






float max_pair_wise_diff(int m, float *a, float *b)
{
  float max_diff = 0.0;
  
  for(int i = 0; i < m; ++i)
    {
      float sum  = fabs(a[i]+b[i]);
      float diff = fabs(a[i]-b[i]);

      float res = 0.0f;

      if(sum == 0.0f)
	res = diff;
      else
	res = 2*diff/sum;

      if( res > max_diff )
	max_diff = res;
    }

  return max_diff;
}



void fill_sequences( int size, int start, float *dst )
{
  for( int i = 0; i < size; ++i )
      dst[i]= (float)(i+start);
}

void neg_xout_sequences( int size, float *dst )
{
  for( int i = 0; i < size; ++i )
    {
      dst[i] = -1.0f;
    }
}

/*
  Functions for distributing the sequential data. Each node
  is receiving only a small portion of the "input_sequential."
  Make sure you understand which elements each node has.

  NOTE: These are not the only ways to distribute data.
*/


//////////////////////////
//////////////////////////
//////////////////////////
//////////////////////////
/* START OF COLLECTIVES */
//////////////////////////
//////////////////////////
//////////////////////////
//////////////////////////

/*
  
  Broadcast

*/
void student_bcast(int root_tid, int num_threads, float *input_sequential, float *output_sequential )
{
  float shared_memory[num_threads]; // FORESHADOWING: GPUs also have shared memory

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();

    // root will write to shared memory
    if (tid == root_tid)
      for(int dst_tid = 0; dst_tid < num_threads; dst_tid++)
	{
	  shared_memory[dst_tid] = input_sequential[root_tid];
	}

    // Wait for barrier
    #pragma omp barrier

    // everyone will read from shared memory
    /*
      STUDENT_TODO
    */
    output_sequential[tid] = shared_memory[root_tid];
    /*
      END_STUDENT_TODO
    */

  }

}

void reference_bcast(int root_tid, int num_threads, float *input_sequential, float *output_sequential )
{

  for( int tid = 0; tid < num_threads; ++tid )
    {
      output_sequential[tid] = input_sequential[root_tid];
    }
}


void test_broadcast(char *prob, int root_tid, int num_threads)
{
  
  float *input_sequential = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_threads);

  fill_sequences( num_threads, 1, input_sequential );
  neg_xout_sequences( num_threads, output_sequential_test );
  neg_xout_sequences( num_threads, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */
  reference_bcast(root_tid, num_threads, input_sequential, output_sequential_reference );

  /*
    Test  Parallel Implementation
  */

  // This is the function being implemented
  student_bcast(root_tid, num_threads, input_sequential, output_sequential_test); 

  /* 
     Test the answer
  */
    {

      float res = max_pair_wise_diff(num_threads,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_broadcast: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_threads, input_sequential);
	  print_float_mem("out_seq_tst", num_threads, output_sequential_test);
	  print_float_mem("out_seq_ref", num_threads, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  
  free(input_sequential);
  free(output_sequential_test);
  free(output_sequential_reference);
}

////////////////////////
////////////////////////
////////////////////////
////////////////////////

/*
  Reduction
*/

void reference_reduction(int root_tid, int num_threads, float *input_sequential, float *output_sequential )
{


  output_sequential[root_tid] = 0.0f;
  for( int tid = 0; tid < num_threads; ++tid )
    {
      output_sequential[root_tid] += input_sequential[tid];
    }
}

void student_reduction( int root_tid, int num_threads,
			float *src_sequential,
			float *output_sequential)
{
  float shared_memory[num_threads]; // FORESHADOWING: GPUs also have shared memory (https://developer.nvidia.com/blog/using-shared-memory-cuda-cc/)

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();

    // Everyone writes their copy to shared memory
    shared_memory[tid] = src_sequential[tid];

    // Wait for barrier // FORESHARDOWING: For GPUs this is __syncthreads()
    #pragma omp barrier

    // root will sum up the result.
    // Note, there are faster ways to do this with openmp's reduction constructs.
    /*
      STUDENT_TODO
    */
    if (tid == root_tid)
      {
	output_sequential[root_tid] = 0.0f;
	for(int src_tid = 0; src_tid < num_threads; src_tid++)
	  {
	    output_sequential[root_tid] += shared_memory[src_tid];
	  }
      }
    /*
      END_STUDENT_TODO
    */

  }
}


void test_reduction(char *prob, int root_tid, int num_threads)
{
  float *input_sequential = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_threads);

  fill_sequences( num_threads, 1, input_sequential );
  neg_xout_sequences( num_threads, output_sequential_test );
  neg_xout_sequences( num_threads, output_sequential_reference );

  /*
    Reference Sequential Implementation
  */

  reference_reduction(root_tid, num_threads, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */

  // This is the function being implemented
  student_reduction(root_tid, num_threads, input_sequential, output_sequential_test); 

  /* 
     Test the answer
  */

    {

      float res = max_pair_wise_diff(num_threads,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_reduction: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_threads, input_sequential);
	  print_float_mem("out_seq_tst", num_threads, output_sequential_test);
	  print_float_mem("out_seq_ref", num_threads, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  
  free(input_sequential);
  free(output_sequential_test);
  free(output_sequential_reference);
}


////////////////////////
////////////////////////
////////////////////////
////////////////////////

/*
  
  Scatter

*/
void student_scatter(int root_tid, int num_threads, float *input_sequential, float *output_sequential)
{
  float shared_memory[num_threads]; // FORESHADOWING: GPUs also have shared memory

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();

    // root will write to shared memory
    if (tid == root_tid)
      for(int dst_tid = 0; dst_tid < num_threads; dst_tid++)
	{
	  shared_memory[dst_tid] = input_sequential[dst_tid+root_tid*num_threads];
	}

    // Wait for barrier
    #pragma omp barrier

    // everyone will read from shared memory
    /*
      STUDENT_TODO
    */
    output_sequential[tid] = shared_memory[tid];
    /*
      END_STUDENT_TODO
    */

  }
}

void reference_scatter(int root_tid, int num_threads, float *input_sequential, float *output_sequential )
{

  for( int tid = 0; tid < num_threads; ++tid )
    {      
      output_sequential[tid] = input_sequential[tid+root_tid*num_threads];
    }
}


void test_scatter(char *prob, int root_tid, int num_threads)
{
  float *input_sequential = (float *)malloc(sizeof(float)*num_threads*num_threads);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_threads);

  fill_sequences( num_threads*num_threads, 1, input_sequential );

  neg_xout_sequences( num_threads, output_sequential_test );
  neg_xout_sequences( num_threads, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */
  reference_scatter(root_tid, num_threads, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  // This is the function being implemented
  student_scatter(root_tid, num_threads, input_sequential, output_sequential_test); 

  /* 
     Test the answer
  */

    {

      float res = max_pair_wise_diff(num_threads,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_scatter: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_threads*num_threads, input_sequential);
	  print_float_mem("out_seq_tst", num_threads, output_sequential_test);
	  print_float_mem("out_seq_ref", num_threads, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  free(input_sequential);
  free(output_sequential_test);
  free(output_sequential_reference);
}

////////////////////////
////////////////////////
////////////////////////
////////////////////////

/*

  Gather

*/

void reference_gather(int root_tid, int num_threads, float *input_sequential, float *output_sequential )
{
  for( int tid = 0; tid < num_threads; ++tid )
    {
      output_sequential[tid+root_tid*num_threads] = input_sequential[tid];
    }
}

void student_gather( int root_tid, int num_threads,
		     float *src_sequential,
		     float *output_sequential)
{

  float shared_memory[num_threads]; // FORESHADOWING: GPUs also have shared memory

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();

    // Everyone writes their copy to shared memory
    shared_memory[tid] = src_sequential[tid];

    // Wait for barrier
    #pragma omp barrier

    // root will sum up the result.
    // Note, there are faster ways to do this with openmp's reduction constructs.
    /*
      STUDENT_TODO
    */
    if (tid == root_tid)
      {
	for(int src_tid = 0; src_tid < num_threads; src_tid++)
	  {
	    output_sequential[src_tid + root_tid * num_threads] = shared_memory[src_tid];
	  }
      }
    /*
      END_STUDENT_TODO
    */

  }

}


void test_gather(char *prob, int root_tid, int num_threads)
{
  float *input_sequential = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_threads*num_threads);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_threads*num_threads);

  fill_sequences( num_threads, 1, input_sequential );
  neg_xout_sequences( num_threads*num_threads, output_sequential_test );
  neg_xout_sequences( num_threads*num_threads, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */
  reference_gather(root_tid, num_threads, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // This is the function being implemented
  student_gather(root_tid, num_threads, input_sequential, output_sequential_test); 

  /* 
     Test the answer
  */

  
    {

      float res = max_pair_wise_diff(num_threads*num_threads,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_gather: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_threads, input_sequential);
	  print_float_mem("out_seq_tst", num_threads*num_threads, output_sequential_test);
	  print_float_mem("out_seq_ref", num_threads*num_threads, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  
  free(input_sequential);
  free(output_sequential_test);
  free(output_sequential_reference);
}


////////////////////////
////////////////////////
////////////////////////
////////////////////////

/*
  All Reduce
*/
void reference_all_reduce( int num_threads, float *input_sequential, float *output_sequential )
{
  float sum = 0.0f;
  for( int tid = 0; tid < num_threads; ++tid )
    {
      sum += input_sequential[tid];
    }

  for( int tid = 0; tid < num_threads; ++tid )
    {
      output_sequential[tid] = sum;
    }

}

void student_all_reduce( int num_threads,
			 float *src_sequential,
			 float *output_sequential)
{
  float shared_memory[num_threads]; // FORESHADOWING: GPUs also have shared memory (https://developer.nvidia.com/blog/using-shared-memory-cuda-cc/)

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();

    // Everyone writes their copy to shared memory
    shared_memory[tid] = src_sequential[tid];

    // Wait for barrier // FORESHARDOWING: For GPUs this is __syncthreads()
    #pragma omp barrier

    // root will sum up the result.
    // Note, there are faster ways to do this with openmp's reduction constructs.
    /*
      STUDENT_TODO
    */
    output_sequential[tid] = 0.0f;
    for(int src_tid = 0; src_tid < num_threads; src_tid++)
      {
	      output_sequential[tid] += shared_memory[src_tid];
      }

    /*
      END_STUDENT_TODO
    */

  }
}


void test_all_reduce(char *prob, int root_tid, int num_threads)
{
  float *input_sequential = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_threads);

  fill_sequences( num_threads, 1, input_sequential );
  neg_xout_sequences( num_threads, output_sequential_test );
  neg_xout_sequences( num_threads, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */

  reference_all_reduce( num_threads, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // This is the function being implemented
  student_all_reduce( num_threads, input_sequential, output_sequential_test); 

  /* 
     Test the answer
  */

    {

      float res = max_pair_wise_diff(num_threads,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_all_reduce: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_threads, input_sequential);
	  print_float_mem("out_seq_tst", num_threads, output_sequential_test);
	  print_float_mem("out_seq_ref", num_threads, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  
  free(input_sequential);
  free(output_sequential_test);
  free(output_sequential_reference);
}

////////////////////////
////////////////////////
////////////////////////
////////////////////////

/*

  All Gather

*/

void reference_all_gather(int num_threads, float *input_sequential, float *output_sequential )
{
  for( int tid = 0; tid < num_threads; ++tid )
    for( int i = 0; i < num_threads; ++i )
      {
	output_sequential[i+tid*num_threads] = input_sequential[i];
      }
}

void student_all_gather( int num_threads,
			 float *src_sequential,
			 float *output_sequential)
{
  float shared_memory[num_threads]; // FORESHADOWING: GPUs also have shared memory

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();

    // Everyone writes their copy to shared memory
    shared_memory[tid] = src_sequential[tid];

    // Wait for barrier
    #pragma omp barrier

    // root will sum up the result.
    // Note, there are faster ways to do this with openmp's reduction constructs.
    /*
      STUDENT_TODO
    */
    for(int src_tid = 0; src_tid < num_threads; src_tid++)
      {
	      output_sequential[src_tid + tid*num_threads] = shared_memory[src_tid];
      }

    /*
      END_STUDENT_TODO
    */

  }
}

void test_all_gather(char *prob, int root_tid, int num_threads)
{
  float *input_sequential = (float *)malloc(sizeof(float)*num_threads);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_threads*num_threads);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_threads*num_threads);

  fill_sequences( num_threads, 1, input_sequential );
  neg_xout_sequences( num_threads*num_threads, output_sequential_test );
  neg_xout_sequences( num_threads*num_threads, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */

  reference_all_gather( num_threads, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */

  // This is the function being implemented
  student_all_gather( num_threads, input_sequential, output_sequential_test); 


  /* 
     Test the answer
  */

    {

      float res = max_pair_wise_diff(num_threads*num_threads,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_all_gather: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_threads, input_sequential);
	  print_float_mem("out_seq_tst", num_threads*num_threads, output_sequential_test);
	  print_float_mem("out_seq_ref", num_threads*num_threads, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  
  free(input_sequential);
  free(output_sequential_test);
  free(output_sequential_reference);
}

////////////////////////
////////////////////////
////////////////////////
////////////////////////



/*

  All to All
  (Or... Transpose)

*/

void reference_all_to_all(int num_threads, float *input_sequential, float *output_sequential )
{

  for( int tid = 0; tid < num_threads; ++tid )
    for( int i = 0; i < num_threads; ++i )
      {
	output_sequential[i+tid*num_threads] = input_sequential[tid+i*num_threads];
      }
}

void student_all_to_all( int num_threads,
			 float *src_sequential,
			 float *output_sequential)
{

  float shared_memory[num_threads*num_threads]; // FORESHADOWING: GPUs also have shared memory

  #pragma omp parallel
  {
    int tid = omp_get_thread_num();

    // Everyone writes their copy to shared memory
    for( int i = 0; i < num_threads; ++i )
      shared_memory[tid*num_threads+i] = src_sequential[tid*num_threads+i];

    // Wait for barrier
    #pragma omp barrier

    // root will sum up the result.
    // Note, there are faster ways to do this with openmp's reduction constructs.
    /*
      STUDENT_TODO
    */
    for( int i = 0; i < num_threads; ++i )
      output_sequential[i + tid*num_threads] = shared_memory[i * num_threads + tid];


    /*
      END_STUDENT_TODO
    */

  }
}


void test_all_to_all(char *prob, int root_tid, int num_threads)
{
  float *input_sequential = (float *)malloc(sizeof(float)*num_threads*num_threads);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_threads*num_threads);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_threads*num_threads);

  fill_sequences( num_threads*num_threads, 1, input_sequential );
  neg_xout_sequences( num_threads*num_threads, output_sequential_test );
  neg_xout_sequences( num_threads*num_threads, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */

  reference_all_to_all( num_threads, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // This is the function being implemented
  student_all_to_all( num_threads, input_sequential, output_sequential_test); 

  /* 
     Test the answer
  */

    {

      float res = max_pair_wise_diff(num_threads*num_threads,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_all_to_all: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_threads*num_threads, input_sequential);
	  print_float_mem("out_seq_tst", num_threads*num_threads, output_sequential_test);
	  print_float_mem("out_seq_ref", num_threads*num_threads, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  free(input_sequential);
  free(output_sequential_test);
  free(output_sequential_reference);
}


void main( int argc, char **argv )
{
  int num_threads = 4; // omp_get_num_threads()
  omp_set_num_threads(num_threads);
  
  // We want a random root tid for each time you run this.
  srand(time(NULL));
  int root_tid = (rand())%(num_threads);
  printf("Random Root TID: %i\n",root_tid);

  test_broadcast("Prob 01",root_tid, num_threads);
  test_reduction("Prob 02",root_tid, num_threads);
  test_scatter("Prob 03",root_tid, num_threads);
  test_gather("Prob 04",root_tid, num_threads);
  test_all_reduce("Prob 05",root_tid, num_threads);
  test_all_gather("Prob 06",root_tid, num_threads);
  test_all_to_all("Prob 07",root_tid, num_threads);
}
