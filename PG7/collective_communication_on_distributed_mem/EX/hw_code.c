/*
  Distributed Memory: MPI Basic Examples

  In this program we will run through a variety of examples to
  demonstrate how the Message Passing Interface (MPI) works.
  Pay particular attention to all of the 'NOTE' in the comments.
  Also, look into the Makefile because compiling and running
  distributed memory programs is non-trivial.

  - richard.m.veras@ou.edu
*/



/*
  NOTE: To run this code you will have at least three options.
  1. Locally: install mpi on your system
     https://rantahar.github.io/introduction-to-mpi/setup.html
  2. Remote on Schooner:
  3. Locally on a Vagrant box: to be released
  


  NOTE: Look at the MPI tutorial here:
  https://mpitutorial.com/tutorials/mpi-introduction/
  https://mpitutorial.com/tutorials/mpi-send-and-receive/

  You will complete the implementation of several Matrix Vector
  operations using Collective Communications (no Send and Recv).

*/


#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// NOTE: This is the required header for MPI.
#include <mpi.h>





/*
  Helper functions
*/

void printf_root(int root_rid, const char *format, ...)
{
  int rid;
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);
  if(rid == root_rid )
    {
      va_list args;
      va_start(args, format);
      vprintf(format, args);
      va_end(args);
    }
}

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



void print_float_matrix_mem(char *name, int m, int n, int rs, int cs, float *src)
{

  printf("%s =\n",name);
  for( int i = 0; i < m; ++i )
    {
      for( int j = 0; j < n; ++j )
	{
	  if ( src[i*cs + j*rs] < 0.0f )
	    printf( " x, ", src[i*cs + j*rs] );
	  else
	    printf( "%2.f, ", src[i*cs + j*rs] );
	}
      printf("\n");
    }
  printf("\n");
}



void print_distributed_data( int root_rid,
			     char *name,
			       int elems_per_node,
			       float *src_distributed )
{

  int rid;
  int num_ranks;
  int tag = 0;
  MPI_Status  status;

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
      printf("\n");
      
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
      // all other processes (ranks) are waiting for the identity of who will use
      // as the root process (rank)
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

void zero_out_sequences( int size, float *dst )
{
  for( int i = 0; i < size; ++i )
    {
      dst[i] = 0.0f;
    }
}





////////////////////////
////////////////////////
////////////////////////
////////////////////////
void example_mpi_hello_world()
{
  int num_ranks;
  int rid;

  /*
    NOTE: MPI_COMM_WORLD is the default 'communicator' which is
    the collection of all nodes involved in this distributed
    computation.

    For advanced programs (and distributed memory systems) we
    could split the default communicator into partitions.
   */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);


  /*
    Note: Rank ID is unique to each node in the communicator.
    This is what we will condition (dispatch) our code on.
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  
  printf("Hello World from node with rank=%i out of %i.\n",
	 rid,
	 num_ranks);
}


void example_barriers_serialize_execution()
{
  int num_ranks;
  int rid;

  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);



  /////////////////

  /*
    Note: MPI_Barrier is used to synchronize ranks that are accessing
    shared resources. In general you will probably not need these as
    the more advanced MPI constructs (like collectives) have implicit
    barriers inside of them. That being said play around with this code
    by commenting/uncommenting the barriers and moving them around. In
    this situation the contended resource is the terminal.

    The effect you shuld see with the barrier is:

    Node with rank=0 is saying A
    Node with rank=1 is saying A
    Node with rank=2 is saying A
    Node with rank=3 is saying A
    Node with rank=2 is saying B
    Node with rank=3 is saying B
    Node with rank=0 is saying B
    Node with rank=1 is saying B

    and without the barrier it should be a little scrambled
    However, there are some other threading effects in play
    so you might not see that. You can exagerate things by
    adding sleep timers.
   */

  
  for( int cur_rank = 0; cur_rank < num_ranks; ++cur_rank )
    {
      sleep(.1); // NOTE: comment/uncomment
      if(cur_rank == rid )
	{
	  printf("Node with rank=%i is saying A\n",rid);
	}
       MPI_Barrier(MPI_COMM_WORLD); // NOTE: comment/uncomment
    }

  for( int cur_rank = 0; cur_rank < num_ranks; ++cur_rank )
    {
      sleep(.1); // NOTE: comment/uncomment
      if(cur_rank == rid )
	{
	  printf("Node with rank=%i is saying B\n",rid);
	}
      MPI_Barrier(MPI_COMM_WORLD); // NOTE: comment/uncomment
    }
  
  /////////////////

}


void example_everyone_has_a_different_address_space()
{
  int num_ranks;
  int rid;

  /*
    NOTE: MPI_COMM_WORLD is the default 'communicator' which is
    the collection of all nodes involved in this distributed
    computation.

    For advanced programs (and distributed memory systems) we
    could split the default communicator into partitions.
   */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);


  /*
    Note: Rank ID is unique to each node in the communicator.
    This is what we will condition (dispatch) our code on.
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  // Just to keep things easy we will make the dimensions multiples
  // of the rank.
  int x_elems_per_node = 4;
  int n_sequential = 4*num_ranks;


  /* NOTE: Normally the root rid is 0, but to show that
     this choice is arbitrary we will pick a random one.
  */
  // We want a random root rid for each time you run this.
  int root_rid = get_root_rid_for_this_run();

  /////////////////
  /*
    Allocate the inputs and outputs

    Note: you will see in a lot of the code a 'sequential' and
    'distributed' version of each buffer. The 'distributed' version
    is the working dataset that each node will use. The 'sequential'
    version will only be used by the root rank (by convention the node
    with the rank id 0). The root rank will then distribute that data
    to the other ranks which will store that data in their 'distributed'
    buffer. In large scale applications you would not really have a
    'sequential' version because more often than not, the entire dataset
    will not fit on one node. Instead all nodes will load their portion of the
    data from some parallel filesystem. But for the sake of the assignments
    we will keep to this convention.
  */
  float *x_sequential  = (float *)malloc(sizeof(float)*n_sequential);
  float *x_distributed = (float *)malloc(sizeof(float)*x_elems_per_node);

  // STUDENT_TODO: swap these around.
  if(rid == root_rid )
    {
      //fill_sequences( n_sequential, 1, x_sequential );
      fill_sequences( n_sequential, rid*n_sequential, x_sequential );
    }


  /*
    NOTE: We have 4 ranks. So when we execute this program we have 4 copies
    of this program, with each program receiving a different rank.

    STUDENT_TODO: Play around with the values in these vectors.
   */
  if(rid == 0)
    {
      x_distributed[0] = 1;
      x_distributed[1] = -1;
      x_distributed[2] = -1;
      x_distributed[3] = -1;
    }
  else if(rid == 1)
    {
      x_distributed[0] = -1;
      x_distributed[1] = -1;
      x_distributed[2] = 2;
      x_distributed[3] = 5;
    }
  else if(rid == 2)
    {
      x_distributed[0] = -1;
      x_distributed[1] = 7;
      x_distributed[2] = -1;
      x_distributed[3] = 3;
    }
  else if(rid == 3)
    {
      x_distributed[0] = 2;
      x_distributed[1] = 4;
      x_distributed[2] = -1;
      x_distributed[3] = -1;
    }



  /*
    STUDENT_TODO: Uncomment the sequential one. In the programs you will
    use only the root will have useful stuff in it. And it is normally the
    entire dataset that will be split across the nodes.
  */
  //print_distributed_data(root_rid, "x_sequ", n_sequential, x_sequential);
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);

  

  free(x_sequential);
  free(x_distributed);
  
  /////////////////

}


/*
  Note: Everytime we want to communicate between ranks we have
  to send a message (ie we cannot simply write to the address
  space of the other rank.
 */
void example_send_and_receive()
{
  int num_ranks;
  int rid;

  /*
    NOTE: We have a few new values we need to keep track of.
    status is for error messages, and tag for the most part
    is not entirely useful. It is a bucket for a user defined
    value that you can optionally use in your code.
   */
  int tag = 0;
  MPI_Status  status;

  /*
    NOTE: MPI_COMM_WORLD is the default 'communicator' which is
    the collection of all nodes involved in this distributed
    computation.

    For advanced programs (and distributed memory systems) we
    could split the default communicator into partitions.
   */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);


  /*
    Note: Rank ID is unique to each node in the communicator.
    This is what we will condition (dispatch) our code on.
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  // Just to keep things easy we will make the dimensions multiples
  // of the rank.
  int x_elems_per_node = 4;
  int n_sequential = 4*num_ranks;


  /* NOTE: Normally the root rid is 0, but to show that
     this choice is arbitrary we will pick a random one.
  */
  // We want a random root rid for each time you run this.
  int root_rid = get_root_rid_for_this_run();

  float *x_distributed = (float *)malloc(sizeof(float)*x_elems_per_node);


  /*
    NOTE: We have 4 ranks. So when we execute this program we have 4 copies
    of this program, with each program receiving a different rank.

    STUDENT_TODO: Play around with the values in these vectors.
   */
  if(rid == 0)
    {
      x_distributed[0] = 1;
      x_distributed[1] = 2;
      x_distributed[2] = 3;
      x_distributed[3] = 4;
    }
  else if(rid == 1)
    {
      x_distributed[0] = -1;
      x_distributed[1] = -1;
      x_distributed[2] = -1;
      x_distributed[3] = -1;
    }
  else if(rid == 2)
    {
      x_distributed[0] = -1;
      x_distributed[1] = -1;
      x_distributed[2] = -1;
      x_distributed[3] = -1;
    }
  else if(rid == 3)
    {
      x_distributed[0] = -1;
      x_distributed[1] = -1;
      x_distributed[2] = -1;
      x_distributed[3] = -1;
    }
  // Note: this is before we setup the communication (send/recv)
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);

  /* Note: Now we are going to send one element from the node
     with rank == 0 to each of the other nodes.

     The thing to notice is that the send call on rank==0 and the
     corresponding receive on the other nodes must match in terms
     of which rank the receiver is waiting for and which rank the
     sender is sending to. Additionally, the size and type of the
     message must match.

     STUDENT_TODO: I want you to play around with sending different
     messages with different sizes.
  */


  
  if(rid == 0)
    {

      // send 1 element to rank 1
      MPI_Send(x_distributed, // the base address of what I want to send
	       1, // the number of elements
	       MPI_FLOAT, // the datatype
	       1, // The rank I am sending to
	       tag, // useless most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives

      // send 3 elements to rank 2 starting from element 1
      MPI_Send(&x_distributed[1], // the base address of what I want to send
	       3, // the number of elements
	       MPI_FLOAT, // the datatype
	       2, // The rank I am sending to
	       tag, // useless most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives

      // send 2 elements to rank 3 starting from element 2
      MPI_Send(&x_distributed[2], // the base address of what I want to send
	       2, // the number of elements
	       MPI_FLOAT, // the datatype
	       3, // The rank I am sending to
	       tag, // useless most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives

      
    }
  else if(rid == 1)
    {
      MPI_Recv(x_distributed, // The base address of where I am storing the message.
	       1, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // useless
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.
    }
  else if(rid == 2)
    {
      // I am going to store the 3 elements starting at position 0
      MPI_Recv(x_distributed, // The base address of where I am storing the message.
	       3, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // useless
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

    }
  else if(rid == 3)
    {
      // I am going to store the 2 elements starting at position 2
      MPI_Recv(&x_distributed[2], // The base address of where I am storing the message.
	       2, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // useless
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

    }

  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);
  

  
  free(x_distributed);
  
  /////////////////

}


/*
  NOTE:
  
  In real large scale distributed memory applications the dataset is typically
  too large to fit in one system. Instead the datasets are typically stored on
  massive distributed filesystems (look up Ceph [crazy cool] or luster/gluster).
  These datasets can be accessed piecemeal in parallel by each of the nodes.
  So they data starts distributed across many file servers and is loaded in a
  distributed fashion, computed on in a distributed manner, then written back
  to these parallel file servers.

  For the purpose of our assignments we are not starting with our data on
  distributed disks. For expediency, our datasets will start on the root
  rank. We will call this dataset the 'sequential' copy. The root rank will
  then distribute a portion of the data to each rank to store as their
  'distributed' copy. Each rank then computes its work on its 'distributed'
  copy, performing the necessary communications with other ranks to complete
  the task. Once the work is done, the root will collect the 'distributed'
  results into a 'sequential' result that is then checked against a serial
  baseline.

  A recap of what to expect in a lab. Including the COMPUTE function that
  you are familiar with, you will see the following:
  
  1. Distributed Allocate - A function where each rank can create all of
  its necessary 'distributed' input and output buffers.

  2. Distribute Data - This is the function responsible for distributing
  the 'sequential' input data from the root rank to the 'distributed' input
  on all of the ranks.

  3. Compute - This is where you transform the 'distributed' input buffer
  into the 'distributed' output buffer.

  4. Collect Data - This is where the root rank will collect the 'distributed'
  output buffers and combine it into a 'sequential' output buffer.

  5. Distributed Free - Free the distributed buffers that were allocated.
  

  Even if you do not do distributed memory programming, this is a
  standard workflow. You will see the same workflow with GPUs
  and other accelerators, the change being that distribute and
  collect will refer to moving data to and from the host and
  device.

 */
void example_pipeline_for_labs()
{
  int num_ranks;
  int rid;
  int tag = 0;
  MPI_Status  status;

  /*
    NOTE: MPI_COMM_WORLD is the default 'communicator' which is
    the collection of all nodes involved in this distributed
    computation.

    For advanced programs (and distributed memory systems) we
    could split the default communicator into partitions.
   */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

  

  /*
    Note: Rank ID is unique to each node in the communicator.
    This is what we will condition (dispatch) our code on.
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  // Just to keep things easy we will make the dimensions multiples
  // of the rank.
  int x_elems_per_node = 4;
  int n_sequential = 4*num_ranks;


  /* NOTE: Normally the root rid is 0..
  */
  int root_rid = 0;

  /////////////////
  /*
    Allocate the inputs and outputs

    Note: you will see in a lot of the code a 'sequential' and
    'distributed' version of each buffer. The 'distributed' version
    is the working dataset that each node will use. The 'sequential'
    version will only be used by the root rank (by convention the node
    with the rank id 0). The root rank will then distribute that data
    to the other ranks which will store that data in their 'distributed'
    buffer. In large scale applications you would not really have a
    'sequential' version because more often than not, the entire dataset
    will not fit on one node. Instead all nodes will load their portion of the
    data from some parallel filesystem. But for the sake of the assignments
    we will keep to this convention.
  */
  float *x_sequential  = (float *)malloc(sizeof(float)*n_sequential);
  // For illustration purposes we will negate this out on every node.
  neg_xout_sequences( n_sequential, x_sequential );


  if(rid == root_rid )
    {
      fill_sequences( n_sequential, 1, x_sequential );
    }

  print_distributed_data(root_rid, "x_sequ", n_sequential, x_sequential);


  /*
    ALLOCATE

    NOTE: this is the 'distributed' allocate.

   */
  float *x_distributed = (float *)malloc(sizeof(float)*x_elems_per_node);

  // For illustration purposes I am going to x-out this buffer
  neg_xout_sequences( x_elems_per_node, x_distributed );
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);
  
  /*
    DISTRIBUTE

    
    NOTE: This is where we distribute parts of the sequential buffer to
    each rank from the root.
   */
  if(rid == 0)
    {
      // We need to 'send' a copy to ourselves (root=0)
      for( int i = 0; i < 4; ++i )
	x_distributed[i] = x_sequential[i];
	

      // Sending x_seq[4-7] to rank=1
      MPI_Send(&x_sequential[4], // the base address of what I want to send
	       4, // the number of elements
	       MPI_FLOAT, // the datatype
	       1, // The rank I am sending to
	       tag, // unnecessary most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives

      
      // Sending x_seq[8-11] to rank=2
      MPI_Send(&x_sequential[8], // the base address of what I want to send
	       4, // the number of elements
	       MPI_FLOAT, // the datatype
	       2, // The rank I am sending to
	       tag, // unnecessary most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives

      // Sending x_seq[12-15] to rank=3
      MPI_Send(&x_sequential[12], // the base address of what I want to send
	       4, // the number of elements
	       MPI_FLOAT, // the datatype
	       3, // The rank I am sending to
	       tag, // unnecessary most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives
      
      
    }
  else if(rid == 1)
    {
      MPI_Recv(x_distributed, // The base address of where I am storing the message.
	       4, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // unnecessary
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

    }
  else if(rid == 2)
    {
      MPI_Recv(x_distributed, // The base address of where I am storing the message.
	       4, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // unnecessary
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

    }
  else if(rid == 3)
    {
      MPI_Recv(x_distributed, // The base address of where I am storing the message.
	       4, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // unnecessary
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

    }



  /*
    NOTE: This is what each node has after distributing the data.
  */
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);


  /*
    COMPUTE

    
    Note: Now each rank will do its COMPUTE.

    We will not condition this work
    on the rank id because they will all do the same thing to their
    respective 'distributed' data
  */
  


  for(int i = 0; i < 4; ++i)
    x_distributed[i]= x_distributed[i]*10+rid;


  /*
    Note: This is what the 'distributed' data looks like after each
    rank is done computing on it.
  */
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);


  /*
    COLLECT

    Note: The root rank will now collect all of the 'distributed' results.
  */
    if(rid == 0)
    {
      // We need to 'receive' a copy to ourselves (root=0)
      for( int i = 0; i < 4; ++i )
	x_sequential[i] = x_distributed[i];
	

      // Receive x_seq[4-7] from rank=1
      MPI_Recv(&x_sequential[4], // the base address of what I want to send
	       4, // the number of elements
	       MPI_FLOAT, // the datatype
	       1, // The rank I am sending to
	       tag, // unnecessary most of the time
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

      
      // Receiving x_seq[8-11] from rank=2
      MPI_Recv(&x_sequential[8], // the base address of what I want to send
	       4, // the number of elements
	       MPI_FLOAT, // the datatype
	       2, // The rank I am sending to
	       tag, // unnecessary most of the time
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

      // Receiving x_seq[12-15] from rank=3
      MPI_Recv(&x_sequential[12], // the base address of what I want to send
	       4, // the number of elements
	       MPI_FLOAT, // the datatype
	       3, // The rank I am sending to
	       tag, // unnecessary most of the time
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.
      
      
    }
    // All other ranks will send their data to rank=1
  else if(rid == 1)
    {
      MPI_Send(x_distributed, // The base address of where I am storing the message.
	       4, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // unnecessary
	       MPI_COMM_WORLD);

    }
  else if(rid == 2)
    {
      MPI_Send(x_distributed, // The base address of where I am storing the message.
	       4, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // unnecessary
	       MPI_COMM_WORLD); // where my sender rank lives

    }
  else if(rid == 3)
    {
      MPI_Send(x_distributed, // The base address of where I am storing the message.
	       4, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // unnecessary
	       MPI_COMM_WORLD ); // where my sender rank lives

    }

    // Note: rank=0 should now have all of the data.
    print_distributed_data(root_rid, "x_sequ", n_sequential, x_sequential);  



    /*
      FREE

      Note: here we free the buffers on all the ranks
      
     */
    
    free(x_sequential);
    free(x_distributed);
  
  /////////////////

}

///////////
/*
  Note: Here we have a hardcoded implementation of the collective
  communication broadcast using send and receive.
 */
void example_send_and_receive_based_broadcast()
{
  int num_ranks;
  int rid;

  /*
    NOTE: We have a few new values we need to keep track of.
    status is for error messages, and tag for the most part
    is not entirely useful. It is a bucket for a user defined
    value that you can optionally use in your code.
   */
  int tag = 0;
  MPI_Status  status;

  /*
    NOTE: MPI_COMM_WORLD is the default 'communicator' which is
    the collection of all nodes involved in this distributed
    computation.

    For advanced programs (and distributed memory systems) we
    could split the default communicator into partitions.
   */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);


  /*
    Note: Rank ID is unique to each node in the communicator.
    This is what we will condition (dispatch) our code on.
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  // Just to keep things easy we will make the dimensions multiples
  // of the rank.
  int x_elems_per_node = 4;
  int n_sequential = 4*num_ranks;


  /* NOTE: Normally the root rid is 0, but to show that
     this choice is arbitrary we will pick a random one.
  */
  // We want a random root rid for each time you run this.
  int root_rid = get_root_rid_for_this_run();

  float *x_distributed = (float *)malloc(sizeof(float)*x_elems_per_node);

  neg_xout_sequences( x_elems_per_node, x_distributed );
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);


  /*
    NOTE: We have 4 ranks. So when we execute this program we have 4 copies
    of this program, with each program receiving a different rank.

    STUDENT_TODO: Play around with the values in these vectors.
   */
  if(rid == 0)
    {
      x_distributed[0] = 99;
    }
  // Note: this is before we setup the communication (send/recv)
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);

  /* Note: Now we are going to send one element from the node
     with rank == 0 to each of the other nodes.

     The thing to notice is that the send call on rank==0 and the
     corresponding receive on the other nodes must match in terms
     of which rank the receiver is waiting for and which rank the
     sender is sending to. Additionally, the size and type of the
     message must match.

     STUDENT_TODO: I want you to play around with sending different
     messages with different sizes.
  */


  
  if(rid == 0)
    {
      // For completeness we need to send the value to ourself. or do nothing in this case
      x_distributed[0] = x_distributed[0];      

      
      // Rank==0 will send the value to all of the other ranks
      MPI_Send(&x_distributed[0], // the base address of what I want to send
	       1, // the number of elements
	       MPI_FLOAT, // the datatype
	       1, // The rank I am sending to
	       tag, // useless most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives

      MPI_Send(&x_distributed[0], // the base address of what I want to send
	       1, // the number of elements
	       MPI_FLOAT, // the datatype
	       2, // The rank I am sending to
	       tag, // useless most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives

      MPI_Send(&x_distributed[0], // the base address of what I want to send
	       1, // the number of elements
	       MPI_FLOAT, // the datatype
	       3, // The rank I am sending to
	       tag, // useless most of the time
	       MPI_COMM_WORLD); // where my receiving rank lives


      
    }
  else if(rid == 1)
    {
      MPI_Recv(&x_distributed[0], // The base address of where I am storing the message.
	       1, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // useless
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.
    }
  else if(rid == 2)
    {
      MPI_Recv(&x_distributed[0], // The base address of where I am storing the message.
	       1, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // useless
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

    }
  else if(rid == 3)
    {
      MPI_Recv(&x_distributed[0], // The base address of where I am storing the message.
	       2, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       0, // the rank of the sender
	       tag, // useless
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.

    }

  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);
  

  
  free(x_distributed);
  
  /////////////////

}


/*
  Note: Here we have a hardcoded implementation of the collective
  communication broadcast using send and receive. We can clean
  this code up a little more using a better looping structure.
 */
void example_send_and_receive_based_broadcast_clean()
{
  int num_ranks;
  int rid;

  /*
    NOTE: We have a few new values we need to keep track of.
    status is for error messages, and tag for the most part
    is not entirely useful. It is a bucket for a user defined
    value that you can optionally use in your code.
   */
  int tag = 0;
  MPI_Status  status;

  /*
    NOTE: MPI_COMM_WORLD is the default 'communicator' which is
    the collection of all nodes involved in this distributed
    computation.

    For advanced programs (and distributed memory systems) we
    could split the default communicator into partitions.
   */
  MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);


  /*
    Note: Rank ID is unique to each node in the communicator.
    This is what we will condition (dispatch) our code on.
  */
  MPI_Comm_rank(MPI_COMM_WORLD, &rid);

  // Just to keep things easy we will make the dimensions multiples
  // of the rank.
  int x_elems_per_node = 4;
  int n_sequential = 4*num_ranks;


  /* NOTE: Normally the root rid is 0, but to show that
     this choice is arbitrary we will pick a random one.
  */
  // We want a random root rid for each time you run this.
  int root_rid = get_root_rid_for_this_run();

  float *x_distributed = (float *)malloc(sizeof(float)*x_elems_per_node);

  neg_xout_sequences( x_elems_per_node, x_distributed );
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);


  /*
    NOTE: We have 4 ranks. So when we execute this program we have 4 copies
    of this program, with each program receiving a different rank.

    STUDENT_TODO: Play around with the values in these vectors.
   */
  if(rid == root_rid)
    {
      x_distributed[0] = 99;
    }
  // Note: this is before we setup the communication (send/recv)
  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);

  /* Note: This is the essence of a broadcast that sends one element
     from the root rank to everyone else.
  */



  
  if(rid == root_rid)
    {

      for( int dst_rank = 0; dst_rank < 4; ++dst_rank )
	{
	  if( dst_rank != rid )
	    MPI_Send(&x_distributed[0], // the base address of what I want to send
		     1, // the number of elements
		     MPI_FLOAT, // the datatype
		     dst_rank, // The rank I am sending to
		     tag, // useless most of the time
		     MPI_COMM_WORLD); // where my receiving rank lives
	  else
	    // For completeness we need to send the value to ourself. or do nothing in this case
	    x_distributed[0] = x_distributed[0];      
	    
	}
    }
  else
    {
      MPI_Recv(&x_distributed[0], // The base address of where I am storing the message.
	       1, // the number of elements being received
	       MPI_FLOAT, // the datatype
	       root_rid, // the rank of the sender
	       tag, // useless
	       MPI_COMM_WORLD, // where my sender rank lives
	       &status); // error codes.
    }

  print_distributed_data(root_rid, "x_dist", x_elems_per_node, x_distributed);
  

  
  free(x_distributed);
  
  /////////////////

}




void main( int argc, char **argv )
{
  int num_ranks;
  int rid;

  
  /*
    Note: this is how we start MPI. On each node your program
    will start with slightly different 'hidden' parameters for
    argv that are used by MPI to determine the id (rank) of a
    given node.
  */
  MPI_Init(&argc, &argv);


  /*
    STUDENT_TODO: Comment/Uncomment each of these tests *one at a time*
    and reason through the code. Also look through the Makefile. MPI
    uses some extra stuff when compiling and running code.
  */



  // A basic MPI program
  example_mpi_hello_world();

  // Using MPI_Barrier for syncronization
  example_barriers_serialize_execution();    

  // Every rank is a different process (possibly on a different machine)
  example_everyone_has_a_different_address_space();


  // send and receive
  example_send_and_receive();

  // distributing data from sequential/ full pipeline
  example_pipeline_for_labs();

  // a broadcast that hardcodes the communication
  example_send_and_receive_based_broadcast();

  
  // A cleaner broadcast that loops through the ranks
  // Note: This is essentially the first problem in the PG
  example_send_and_receive_based_broadcast_clean();

  // In future files
  // all of the collectives
  // distributing data (block, elemental, block cyclic)
  // communicators 
  
  /* Shut down MPI */
  MPI_Finalize();
  
}
