/*
  HW3 Distributed Memory!

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
                               being performed by the SIMD instructions.
	      2. [results.txt] containing the test output of your code.
	      3. [code.c] Your modified version of this code.


  Peer Review: Unless you opt out by contacting me, your assignment will be
               reviewed by your peers. They will provide useful feedback, but
               will not determine your grade. I will ultimately determine the
               grade for the assignment.

  - richard.m.veras@ou.edu
*/




/*
  You will be using MPI_Send and MPI_Recv to implement the following operations:

  1. Broadcast
  2. Reduce
  3. Scatter
  4. Gather
  5. All Reduce
  6. All gather
  7. All to All


  NOTE: To run this code you will have at least three options.
  1. Locally: install mpi on your system
     https://rantahar.github.io/introduction-to-mpi/setup.html
  2. Remote on Schooner:
  3. Locally on a Vagrant box: to be released
  


  NOTE: Look at the MPI tutorial here:
  https://mpitutorial.com/tutorials/mpi-introduction/
  https://mpitutorial.com/tutorials/mpi-send-and-receive/


  NOTE: Carefully look through these operations and make sure you can
        describe and diagram what they are doing.

	distribute_1d_data_by_blocks
	collect_1d_data_by_blocks

	
  
*/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <mpi.h>


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



void print_distributed_data( char *name,
			       int elems_per_node,
			       float *src_distributed )
{

  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if (rid == root_rid )
    {
      float *dst_sequential = (float *)malloc(sizeof(float)*elems_per_node*num_ranks);
      // Gather everything up
      for( int src_rid = 0; src_rid < num_ranks; src_rid++ )
	{	  
	  if( src_rid != root_rid )
	    {

	      	      MPI_Recv(&dst_sequential[src_rid*elems_per_node],
	      		       elems_per_node, MPI_FLOAT, src_rid, tag, 
	      		       MPI_COMM_WORLD, &status);

	    }
	  else
	    {
	      for( int i = 0; i < elems_per_node; ++i )
		dst_sequential[src_rid*elems_per_node+i] = src_distributed[i];
	    }
	}
      
      for( int src_rid = 0; src_rid < num_ranks; src_rid++ )
	{
	  char name_buff[100];

	  sprintf(name_buff,"%s[rid=%2i]",name,src_rid);
	  
	  	  print_float_mem(name_buff,
	  			  elems_per_node,
	  			  &dst_sequential[src_rid*elems_per_node]);
	}
      
      free(dst_sequential);
    }
  else
    {

      MPI_Send(src_distributed, elems_per_node, MPI_FLOAT, 
	       root_rid, tag, MPI_COMM_WORLD);
    }  
}



// We need to pick a random number between 0 and num_ranks-1
int get_root_rid_for_this_run()
{
  int root_rid;
  int rid;
  int num_ranks;
  MPI_Status  status;
  int         tag = 0;
  
  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* NOTE: we want to randomize what our root_rid's are so
           that you have to generalize.
  */
  if( rid == 0 )
    {
      srand(time(NULL));
      int rand_root = (rand())%(num_ranks);

      printf("The random root for this run is: %i\n",rand_root);
      for(int dest_rid = 0; dest_rid < num_ranks; ++dest_rid )
	if(dest_rid != 0)
	  {
	    MPI_Send(&rand_root, 1, MPI_INT, dest_rid, tag, 
		     MPI_COMM_WORLD);
	  }

      root_rid = rand_root;
    }
  else
    {
      // all other nodes are waiting for the identity of who will use
      // as the root node
      MPI_Recv(&root_rid, 1, MPI_INT, 
	       0, tag, MPI_COMM_WORLD, &status);
    }

  return root_rid;
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

void distribute_1d_data_by_blocks( int elems_per_node,
				      float *input_sequential,
				      float *output_distributed )
{
  int rid;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  for( int i = 0; i < elems_per_node; ++i )
    output_distributed[i] = input_sequential[rid*elems_per_node+i];
  
}


/*
  This will collect all of the data back to one node.
*/
void collect_1d_data_by_blocks( int elems_per_node,
			          float *src_distributed,
			          float *dst_sequential)
{

  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;
  int root_rid = 0;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  if (rid == root_rid )
    {

      // Gather everything up
      for( int src_rid = 0; src_rid < num_ranks; src_rid++ )
	{	  
	  if( src_rid != root_rid )
	    {
	      float *buffer = (float *)malloc(sizeof(float)*elems_per_node);
	      
	      MPI_Recv(buffer, elems_per_node, MPI_FLOAT, src_rid, tag, 
		       MPI_COMM_WORLD, &status);

	      for( int i = 0; i < elems_per_node; ++i )
		dst_sequential[src_rid*elems_per_node+i] = buffer[i];

	      free(buffer);
	    }
	  else
	    {
	      for( int i = 0; i < elems_per_node; ++i )
		dst_sequential[src_rid*elems_per_node+i] = src_distributed[i];
	    }
	}
    }
  else
    {
      MPI_Send(src_distributed, elems_per_node, MPI_FLOAT, 
	       root_rid, tag, MPI_COMM_WORLD);
    }  
}


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
void student_bcast(int root_rid, float *input_distributed, float *output_distributed)
{

  int         rid;
  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* Implement broadcast: root sends its value to everyone, others receive. */
  if (rid == root_rid) { // root sending
    for (int dest = 0; dest < num_ranks; ++dest) { // loop over destinations
      if (dest == root_rid) { // sending to self
        *output_distributed = *input_distributed; // copy locally
      } 
      else { // sending to others
        MPI_Send(input_distributed, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD); // send
      }
    }
  } 
  else { // non-root receiving
    MPI_Recv(output_distributed, 1, MPI_FLOAT, root_rid, tag, MPI_COMM_WORLD, &status); // receive
  }

}

void reference_bcast(int root_rid, float *input_sequential, float *output_sequential )
{

  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  for( int rid = 0; rid < num_ranks; ++rid )
    {
      output_sequential[rid] = input_sequential[root_rid];
    }
}


void test_broadcast(char *prob, int root_rid)
{
  int rid;
  int num_ranks;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  float *input_sequential = (float *)malloc(sizeof(float)*num_ranks);
  float input_distributed = -1;
  float output_distributed_test = -1;
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_ranks);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_ranks);

  fill_sequences( num_ranks, 1, input_sequential );
  neg_xout_sequences( num_ranks, output_sequential_test );
  neg_xout_sequences( num_ranks, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */
  reference_bcast(root_rid, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // input_sequential is our starting data and we need to distribute it
  // across all of the nodes.
  distribute_1d_data_by_blocks( 1, input_sequential, &input_distributed );

  // This is the function being implemented
  student_bcast(root_rid, &input_distributed, &output_distributed_test); 

  // Once the student function is computed we need to collect the data
  // across all of the nodes back to one.
  collect_1d_data_by_blocks( 1, &output_distributed_test, output_sequential_test);

  /* 
     Test the answer
  */

  if (rid == 0 )
    {
      float res = max_pair_wise_diff(num_ranks,
                                     output_sequential_test,
                                     output_sequential_reference);
      printf("%s: test_broadcast: ",prob);
      if( res > 1e-6 )
        {
          printf("FAIL\n");

          print_float_mem("     in_seq", num_ranks, input_sequential);
          print_float_mem("out_seq_tst", num_ranks, output_sequential_test);
          print_float_mem("out_seq_ref", num_ranks, output_sequential_reference);

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

void reference_reduction(int root_rid, float *input_sequential, float *output_sequential )
{

  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  output_sequential[root_rid] = 0.0f;
  for( int rid = 0; rid < num_ranks; ++rid )
    {
      output_sequential[root_rid] += input_sequential[rid];
    }
}

void student_reduction( int root_rid,
			  float *src_distributed,
			  float *output_distributed)
{

  int         rid;
  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* Reduction implemented with root receiving all contributions and summing */
  if (rid == root_rid) { // root receiving
    float sum = *src_distributed; // start with own value
    for (int s = 0; s < num_ranks; ++s) { // loop over senders
      if (s == root_rid) continue; // skip self
      float tmp = 0.0f; // temp storage
      MPI_Recv(&tmp, 1, MPI_FLOAT, s, tag, MPI_COMM_WORLD, &status); // receive
      sum += tmp; // accumulate
    }
    *output_distributed = sum; // store result
  } 
  else { // non-root sending
    MPI_Send(src_distributed, 1, MPI_FLOAT, root_rid, tag, MPI_COMM_WORLD); // send
  }

}


void test_reduction(char *prob, int root_rid)
{
  int rid;
  int num_ranks;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  float *input_sequential = (float *)malloc(sizeof(float)*num_ranks);
  float input_distributed = -1;
  float output_distributed_test = -1;
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_ranks);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_ranks);

  fill_sequences( num_ranks, 1, input_sequential );
  neg_xout_sequences( num_ranks, output_sequential_test );
  neg_xout_sequences( num_ranks, output_sequential_reference );

  /*
    Reference Sequential Implementation
  */

  reference_reduction(root_rid, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // input_sequential is our starting data and we need to distribute it
  // across all of the nodes.
  distribute_1d_data_by_blocks( 1, input_sequential, &input_distributed );

  // This is the function being implemented
  student_reduction(root_rid, &input_distributed, &output_distributed_test); 

  // Once the student function is computed we need to collect the data
  // across all of the nodes back to one.
  collect_1d_data_by_blocks( 1, &output_distributed_test, output_sequential_test);

  /* 
     Test the answer
  */

  if (rid == 0 )
    {

      float res = max_pair_wise_diff(num_ranks,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_reduction: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_ranks, input_sequential);
	  print_float_mem("out_seq_tst", num_ranks, output_sequential_test);
	  print_float_mem("out_seq_ref", num_ranks, output_sequential_reference);

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
void student_scatter(int root_rid, float *src, float *dst)
{

  int         rid;
  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* Scatter: root sends src[dest] to each dest; others receive */
  if (rid == root_rid) { // root sending
    for (int dest = 0; dest < num_ranks; ++dest) { // loop over destinations
      if (dest == root_rid) { // sending to self
        *dst = src[dest]; // copy locally
      } 
      else { // sending to others
        MPI_Send(&src[dest], 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD); // send
      }
    }
  }
  else { // non-root receiving
      MPI_Recv(dst, 1, MPI_FLOAT, root_rid, tag, MPI_COMM_WORLD, &status); // receive
  }

}

void reference_scatter(int root_rid, float *input_sequential, float *output_sequential )
{

  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  for( int rid = 0; rid < num_ranks; ++rid )
    {      
      output_sequential[rid] = input_sequential[rid+root_rid*num_ranks];
    }
}


void test_scatter(char *prob, int root_rid)
{
  int rid;
  int num_ranks;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  float *input_sequential = (float *)malloc(sizeof(float)*num_ranks*num_ranks);
  float *input_distributed = (float *)malloc(sizeof(float)*num_ranks);
  float *output_distributed_test = (float *)malloc(sizeof(float));
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_ranks);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_ranks);

  fill_sequences( num_ranks*num_ranks, 1, input_sequential );

  neg_xout_sequences( 1, input_distributed );
  neg_xout_sequences( 1, output_distributed_test );
  neg_xout_sequences( num_ranks, output_sequential_test );
  neg_xout_sequences( num_ranks, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */
  reference_scatter(root_rid, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // input_sequential is our starting data and we need to distribute it
  // across all of the nodes.
  distribute_1d_data_by_blocks( num_ranks, input_sequential, input_distributed );

  // This is the function being implemented
  student_scatter(root_rid, input_distributed, output_distributed_test); 

  // Once the student function is computed we need to collect the data
  // across all of the nodes back to one.
  collect_1d_data_by_blocks( 1, output_distributed_test, output_sequential_test);

  /* 
     Test the answer
  */

  if (rid == 0 )
    {

      float res = max_pair_wise_diff(num_ranks,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_scatter: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_ranks*num_ranks, input_sequential);
	  print_float_mem("out_seq_tst", num_ranks, output_sequential_test);
	  print_float_mem("out_seq_ref", num_ranks, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  free(input_sequential);
  free(input_distributed);
  free(output_distributed_test);
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

void reference_gather(int root_rid, float *input_sequential, float *output_sequential )
{

  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  for( int rid = 0; rid < num_ranks; ++rid )
    {
      output_sequential[rid+root_rid*num_ranks] = input_sequential[rid];
    }
}

void student_gather( int root_rid,
		     float *src_distributed,
		     float *output_distributed)
{

  int         rid;
  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* Gather: root receives one scalar from each rank and places it into
     output_distributed[src] at index src; non-root ranks send their scalar. */
  if (rid == root_rid) { // root receiving
    output_distributed[rid] = *src_distributed; // store own value
    for (int s = 0; s < num_ranks; ++s) { // loop over senders
      if (s == root_rid) continue; // skip self
      float tmp = 0.0f; // temp storage
      MPI_Recv(&tmp, 1, MPI_FLOAT, s, tag, MPI_COMM_WORLD, &status); // receive
      output_distributed[s] = tmp; // store received value
    }
  } 
  else { // non-root sending
    MPI_Send(src_distributed, 1, MPI_FLOAT, root_rid, tag, MPI_COMM_WORLD); // send
    // non-roots leave their output_distributed untouched
  }

}


void test_gather(char *prob, int root_rid)
{
  int rid;
  int num_ranks;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  float *input_sequential = (float *)malloc(sizeof(float)*num_ranks);
  float input_distributed = -1;
  float *output_distributed_test = (float *)malloc(sizeof(float)*num_ranks);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_ranks*num_ranks);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_ranks*num_ranks);

  fill_sequences( num_ranks, 1, input_sequential );
  neg_xout_sequences( num_ranks, output_distributed_test );
  neg_xout_sequences( num_ranks*num_ranks, output_sequential_test );
  neg_xout_sequences( num_ranks*num_ranks, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */
  reference_gather(root_rid, input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // input_sequential is our starting data and we need to distribute it
  // across all of the nodes.
  distribute_1d_data_by_blocks( 1, input_sequential, &input_distributed );

  // This is the function being implemented
  student_gather(root_rid, &input_distributed, output_distributed_test); 

  // Once the student function is computed we need to collect the data
  // across all of the nodes back to one.
  collect_1d_data_by_blocks( num_ranks, output_distributed_test, output_sequential_test);

  /* 
     Test the answer
  */
  
  //  print_distributed_data( "bt_dist",
  //			  num_ranks,
  //			  output_distributed_test );

  
  if (rid == 0 )
    {

      float res = max_pair_wise_diff(num_ranks*num_ranks,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_gather: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_ranks, input_sequential);
	  print_float_mem("out_seq_tst", num_ranks*num_ranks, output_sequential_test);
	  print_float_mem("out_seq_ref", num_ranks*num_ranks, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  
  free(input_sequential);
  free(output_distributed_test);
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
void reference_all_reduce( float *input_sequential, float *output_sequential )
{

  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  float sum = 0.0f;
  for( int rid = 0; rid < num_ranks; ++rid )
    {
      sum += input_sequential[rid];
    }

  for( int rid = 0; rid < num_ranks; ++rid )
    {
      output_sequential[rid] = sum;
    }

}

void student_all_reduce(float *input_distributed,
			  float *output_distributed)
{

  int         rid;
  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* Implement all-reduce by reducing at root 0 then broadcasting the result */
  int root = 0; // define root
  if (rid == root) { // root
    float sum = *input_distributed; // start with own value
    for (int s = 0; s < num_ranks; ++s) { // loop over senders
      if (s == root) continue;
      float tmp = 0.0f; // temp storage
      MPI_Recv(&tmp, 1, MPI_FLOAT, s, tag, MPI_COMM_WORLD, &status); // receive
      sum += tmp; // accumulate
    }
    // send sum to everyone
    for (int d = 0; d < num_ranks; ++d) { // loop over destinations
      if (d == root) *output_distributed = sum; // copy locally
      else MPI_Send(&sum, 1, MPI_FLOAT, d, tag, MPI_COMM_WORLD); // send
    }
  } 
  else { // non-root
    MPI_Send(input_distributed, 1, MPI_FLOAT, root, tag, MPI_COMM_WORLD); // send
    MPI_Recv(output_distributed, 1, MPI_FLOAT, root, tag, MPI_COMM_WORLD, &status); // receive
  }
}


void test_all_reduce(char *prob, int root_rid)
{
  int rid;
  int num_ranks;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);



  float *input_sequential = (float *)malloc(sizeof(float)*num_ranks);
  float input_distributed = -1;
  float output_distributed_test = -1;
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_ranks);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_ranks);

  fill_sequences( num_ranks, 1, input_sequential );
  neg_xout_sequences( num_ranks, output_sequential_test );
  neg_xout_sequences( num_ranks, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */

  reference_all_reduce( input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // input_sequential is our starting data and we need to distribute it
  // across all of the nodes.
  distribute_1d_data_by_blocks( 1, input_sequential, &input_distributed );

  // This is the function being implemented
  student_all_reduce( &input_distributed, &output_distributed_test); 

  // Once the student function is computed we need to collect the data
  // across all of the nodes back to one.
  collect_1d_data_by_blocks( 1, &output_distributed_test, output_sequential_test);

  /* 
     Test the answer
  */

  if (rid == 0 )
    {

      float res = max_pair_wise_diff(num_ranks,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_all_reduce: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_ranks, input_sequential);
	  print_float_mem("out_seq_tst", num_ranks, output_sequential_test);
	  print_float_mem("out_seq_ref", num_ranks, output_sequential_reference);

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

void reference_all_gather(float *input_sequential, float *output_sequential )
{

  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  for( int rid = 0; rid < num_ranks; ++rid )
    for( int i = 0; i < num_ranks; ++i )
      {
	output_sequential[i+rid*num_ranks] = input_sequential[i];
      }
}

void student_all_gather( float *src_distributed,
			 float *output_distributed)
{

  int         rid;
  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* Implement all-gather by having root(0) collect all values then distribute the full array */
  int root = 0; // define root
  if (rid == root) { // root
    float *buf = (float *)malloc(sizeof(float) * num_ranks); // buffer to hold all values
    buf[rid] = *src_distributed; // store own value
    for (int s = 0; s < num_ranks; ++s) { // loop over senders
      if (s == root) continue; // skip self
      MPI_Recv(&buf[s], 1, MPI_FLOAT, s, tag, MPI_COMM_WORLD, &status); // receive
    }
    // copy to our output and send to others
    for (int i = 0; i < num_ranks; ++i) output_distributed[i] = buf[i]; // copy to own output
    for (int d = 0; d < num_ranks; ++d) { // loop over destinations
      if (d == root) continue; // skip self
      MPI_Send(buf, num_ranks, MPI_FLOAT, d, tag, MPI_COMM_WORLD); // send
    }
    free(buf); // free buffer
  } 
  else { // non-root
    MPI_Send(src_distributed, 1, MPI_FLOAT, root, tag, MPI_COMM_WORLD); // send value to root
    MPI_Recv(output_distributed, num_ranks, MPI_FLOAT, root, tag, MPI_COMM_WORLD, &status); // receive
  }
}

void test_all_gather(char *prob, int root_rid)
{
  int rid;
  int num_ranks;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);



  float *input_sequential = (float *)malloc(sizeof(float)*num_ranks);
  float input_distributed = -1;
  float *output_distributed_test = (float *)malloc(sizeof(float)*num_ranks);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_ranks*num_ranks);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_ranks*num_ranks);

  fill_sequences( num_ranks, 1, input_sequential );
  neg_xout_sequences( num_ranks, output_distributed_test );
  neg_xout_sequences( num_ranks*num_ranks, output_sequential_test );
  neg_xout_sequences( num_ranks*num_ranks, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */

  reference_all_gather( input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // input_sequential is our starting data and we need to distribute it
  // across all of the nodes.
  distribute_1d_data_by_blocks( 1, input_sequential, &input_distributed );

  // This is the function being implemented
  student_all_gather( &input_distributed, output_distributed_test); 

  // Once the student function is computed we need to collect the data
  // across all of the nodes back to one.
  collect_1d_data_by_blocks( num_ranks, output_distributed_test, output_sequential_test);

  /* 
     Test the answer
  */

  if (rid == 0 )
    {

      float res = max_pair_wise_diff(num_ranks*num_ranks,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_all_gather: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_ranks, input_sequential);
	  print_float_mem("out_seq_tst", num_ranks*num_ranks, output_sequential_test);
	  print_float_mem("out_seq_ref", num_ranks*num_ranks, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  
  free(input_sequential);
  free(output_distributed_test);
  free(output_sequential_test);
  free(output_sequential_reference);
}

////////////////////////
////////////////////////
////////////////////////
////////////////////////


/*

  All to All

*/

void reference_all_to_all(float *input_sequential, float *output_sequential )
{

  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  for( int rid = 0; rid < num_ranks; ++rid )
    for( int i = 0; i < num_ranks; ++i )
      {
	output_sequential[i+rid*num_ranks] = input_sequential[rid+i*num_ranks];
      }
}

void student_all_to_all( float *src_distributed,
			 float *output_distributed)
{

  int         rid;
  int         num_ranks;
  int         tag = 0;
  MPI_Status  status;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  /* Implement all-to-all by having each rank send its value to every other rank */
  for (int peer = 0; peer < num_ranks; ++peer) { // loop over peers
    if (peer == rid) { // sending to self
      output_distributed[peer] = src_distributed[peer]; // copy locally
    } 
    else { // sending to others
      float sendv = src_distributed[peer]; // value to send
      float recvv = 0.0f; // value to receive
      MPI_Sendrecv(&sendv, 1, MPI_FLOAT, peer, tag, &recvv, 1, MPI_FLOAT, peer, tag, MPI_COMM_WORLD, &status); // send and receive
      output_distributed[peer] = recvv; // store received value
    }
  }
}


void test_all_to_all(char *prob, int root_rid)
{
  int rid;
  int num_ranks;

  /* Find out process rank  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  /* Find out number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);


  float *input_sequential = (float *)malloc(sizeof(float)*num_ranks*num_ranks);
  float *input_distributed = (float *)malloc(sizeof(float)*num_ranks);
  float *output_distributed_test = (float *)malloc(sizeof(float)*num_ranks);
  float *output_sequential_test = (float *)malloc(sizeof(float)*num_ranks*num_ranks);
  float *output_sequential_reference = (float *)malloc(sizeof(float)*num_ranks*num_ranks);

  fill_sequences( num_ranks*num_ranks, 1, input_sequential );
  neg_xout_sequences( num_ranks, output_distributed_test );
  neg_xout_sequences( num_ranks*num_ranks, output_sequential_test );
  neg_xout_sequences( num_ranks*num_ranks, output_sequential_reference );


  /*
    Reference Sequential Implementation
  */

  reference_all_to_all( input_sequential, output_sequential_reference );

  /*
    Test Distributed Parallel Implementation
  */
  
  // input_sequential is our starting data and we need to distribute it
  // across all of the nodes.
  distribute_1d_data_by_blocks( num_ranks, input_sequential, input_distributed );

  // This is the function being implemented
  student_all_to_all( input_distributed, output_distributed_test); 

  // Once the student function is computed we need to collect the data
  // across all of the nodes back to one.
  collect_1d_data_by_blocks( num_ranks, output_distributed_test, output_sequential_test);

  /* 
     Test the answer
  */

  if (rid == 0 )
    {

      float res = max_pair_wise_diff(num_ranks*num_ranks,
				     output_sequential_test,
				     output_sequential_reference);
      printf("%s: test_all_to_all: ",prob);
      if( res > 1e-6 )
	{
	  printf("FAIL\n");

	  print_float_mem("     in_seq", num_ranks*num_ranks, input_sequential);
	  print_float_mem("out_seq_tst", num_ranks*num_ranks, output_sequential_test);
	  print_float_mem("out_seq_ref", num_ranks*num_ranks, output_sequential_reference);

	  printf("\n");
	}
      else
	{
	  printf("PASS\n");
	}
    }

  free(input_sequential);
  free(input_distributed);
  free(output_distributed_test);
  free(output_sequential_test);
  free(output_sequential_reference);
}

void main( int argc, char **argv )
{
  /* Start up MPI */
  MPI_Init(&argc, &argv);

  // We want a random root rid for each time you run this.
  int root_rid = get_root_rid_for_this_run();

  test_broadcast("Prob 01",root_rid);
  test_reduction("Prob 02",root_rid);
  test_scatter("Prob 03",root_rid);
  test_gather("Prob 04",root_rid);
  test_all_reduce("Prob 05",root_rid);
  test_all_gather("Prob 06",root_rid);
  test_all_to_all("Prob 07",root_rid);
  
  /* Shut down MPI */
  MPI_Finalize();
  
}
