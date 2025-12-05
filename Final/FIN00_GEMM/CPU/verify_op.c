#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ERROR_THRESHOLD 1e-4

extern void COMPUTE_NAME_REF( int m0, int n0, int k0,
                              int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
                              float *A_distributed,
                              float *B_distributed,
                              float *C_distributed );

extern void COMPUTE_NAME_TST( int m0, int n0, int k0,
                              int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
                              float *A_distributed,
                              float *B_distributed,
                              float *C_distributed );


extern void DISTRIBUTED_ALLOCATE_NAME_REF( int m0, int n0, int k0,
                                           int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
                                           float **A_distributed,
                                           float **B_distributed,
                                           float **C_distributed );

extern void DISTRIBUTED_ALLOCATE_NAME_TST( int m0, int n0, int k0,
                                           int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
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




extern void COLLECT_DATA_NAME_REF( int m0, int n0, int rs_C, int cs_C,
                                   float *C_distributed,
                                   float *C_sequential );

extern void COLLECT_DATA_NAME_TST( int m0, int n0, int rs_C, int cs_C,
                                   float *C_distributed,
                                   float *C_sequential );




extern void DISTRIBUTED_FREE_NAME_REF( int m0, int n0, int k0,
                                       int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
                                       float *A_distributed,
                                       float *B_distributed,
                                       float *C_distributed );

extern void DISTRIBUTED_FREE_NAME_TST( int m0, int n0, int k0,
                                       int rs_A, int cs_A, int rs_B, int cs_B, int rs_C, int cs_C,
                                       float *A_distributed,
                                       float *B_distributed,
                                       float *C_distributed );

void fill_buffer_with_random( int num_elems, float *buff )
{
  long long range = RAND_MAX;
  //long long range = 1000;

  for(int i = 0; i < num_elems; ++i)
    {
      buff[i] = ((float)((rand() % range) - (range/2)))/ ((float) range);
    }
}

void fill_buffer_with_value( int num_elems, float val, float *buff )
{
  for(int i = 0; i < num_elems; ++i)
    buff[i] = val;
}

/*
  This is a really strict definition of error that is better
  suited for doubles than floats, when the values in question are
  very small ( 1e-5 > |x| ).

  https://0.30000000000000004.com/
  
*/
float max_pair_wise_diff(int m, int n, int rs, int cs, float *a, float *b)
{
  float max_diff = 0.0;

  //if(m<32&n<32)printf("RESIDUE:\n");
  for(int i = 0; i < m; ++i)
    {
      for(int j = 0; j < n; ++j)
	{
	  double sum  = fabs(a[i*cs+j*rs]+b[i*cs+j*rs]);
	  double diff = fabs(a[i*cs+j*rs]-b[i*cs+j*rs]);

	  double res = 0.0f;

	  if(sum == 0.0f)
	    res = diff;
	  else
	    res = 2*diff/sum;

	  //if(m<32&n<32)printf("%f, ",res);

	  if( res > max_diff )
	    {
	      max_diff = res;
	      printf("ERROR: %3i, %3i, %3i, %f, %f, %f\n",m, i,j, max_diff,a[i*cs+j*rs],b[i*cs+j*rs]);
	    }
	}

      //if(m<32&n<32)printf("\n");
    }
#if 0
  if ( max_diff > ERROR_THRESHOLD )
  for(int i = 0; i < m; ++i)
    {
      for(int j = 0; j < n; ++j)
	{
	  float sum  = fabs(a[i*cs+j*rs]+b[i*cs+j*rs]);
	  float diff = fabs(a[i*cs+j*rs]-b[i*cs+j*rs]);

	  float res = 0.0f;

	  if(sum == 0.0f)
	    res = diff;
	  else
	    res = 2*diff/sum;

	  printf("err: %i, %f,  %f\n",m, max_diff, res);
	}

    }
#endif
  //exit(0);
  return (float) max_diff;
}

float avg_pair_wise_diff(int m, int n, int rs, int cs, float *a, float *b)
{
  double total = 0.0;

  for(int i = 0; i < m; ++i)
    {
      for(int j = 0; j < n; ++j)
	{
	  double sum  = fabs(a[i*cs+j*rs]+b[i*cs+j*rs]);
	  double diff = fabs(a[i*cs+j*rs]-b[i*cs+j*rs]);

	  double res = 0.0f;

	  if(sum == 0.0f)
	    res = diff;
	  else
	    res = 2*diff/sum;

	  total += res;
	}

    }
  return (float) total/(m*n);
}



void print_matrix(int m, int n, int rs, int cs, float *a)
{
  float max_diff = 0.0;

  for(int i = 0; i < m; ++i)
    {
    for(int j = 0; j < n; ++j)
      {
	printf("%f, ", a[i*cs+j*rs]);
      }
    printf("\n");
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

  // TODO: adjust to include k
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
      fprintf(result_file, "num_ranks,m0,n0,k0,rsa,csa,rsb,csb,rsc,csc,residue,result\n");
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
      int A_sequential_sz =m0*k0;
      int B_sequential_sz =k0*n0;
      int C_sequential_sz =m0*n0;

      float *A_sequential_ref = (float *)malloc(sizeof(float)*A_sequential_sz);
      float *C_sequential_ref = (float *)malloc(sizeof(float)*C_sequential_sz);
      float *B_sequential_ref = (float *)malloc(sizeof(float)*B_sequential_sz);

      float *A_sequential_tst = (float *)malloc(sizeof(float)*A_sequential_sz);
      float *C_sequential_tst = (float *)malloc(sizeof(float)*C_sequential_sz);
      float *B_sequential_tst = (float *)malloc(sizeof(float)*B_sequential_sz);


      // We only want to allocate the buffers on every node, but
      // we don't want to fill them with random data on every node
      // just from the root node.

      if( rid == 0)
	{ /* root node */

	  // fill src_ref with random values
	  fill_buffer_with_random( A_sequential_sz, A_sequential_ref );
	  //fill_buffer_with_value( A_sequential_sz, 1,  A_sequential_ref );
	  fill_buffer_with_random( B_sequential_sz, B_sequential_ref );
	  //fill_buffer_with_value( B_sequential_sz, 1, B_sequential_ref );
	  //fill_buffer_with_value( C_sequential_sz, -1, C_sequential_ref );
	  fill_buffer_with_value( C_sequential_sz, 0, C_sequential_ref );


	  // copy src_ref to src_tst
	  memcpy(A_sequential_tst,A_sequential_ref,A_sequential_sz*sizeof(float));
	  memcpy(B_sequential_tst,B_sequential_ref,B_sequential_sz*sizeof(float));
	  memcpy(C_sequential_tst,C_sequential_ref,C_sequential_sz*sizeof(float));

#if 0
	  if ( m0 < 32 & n0 < 32 )
	    {
	      print_matrix(m0, k0, rs_A, cs_A, A_sequential_ref);printf("\n");
	      print_matrix(k0, n0, rs_B, cs_B, B_sequential_ref);printf("\n");
		  
	      print_matrix(m0, n0, rs_C, cs_C, C_sequential_ref);printf("\n");
	      print_matrix(m0, n0, rs_C, cs_C, C_sequential_tst);printf("\n");
	    }
#endif
	  
	}
      else
	{/* all other nodes. */}

      /*
	Run the reference
      */

      float *A_distributed_ref;
      float *B_distributed_ref;
      float *C_distributed_ref;


      // Allocate distributed buffers for the reference
      DISTRIBUTED_ALLOCATE_NAME_REF( m0, n0, k0,
				     rs_A,  cs_A,
				     rs_B,  cs_B,
				     rs_C,  cs_C,
				     &A_distributed_ref,
				     &B_distributed_ref,
				     &C_distributed_ref );

      // Distribute the sequential buffers
      DISTRIBUTE_DATA_NAME_REF( m0, n0, k0,
				rs_A,  cs_A,
				rs_B,  cs_B,
				rs_C,  cs_C,				
				A_sequential_ref,
				B_sequential_ref,
				C_sequential_ref,
				A_distributed_ref,
				B_distributed_ref,
				C_distributed_ref );

      // Perform the computation
      COMPUTE_NAME_REF( m0, n0, k0,
			rs_A, cs_A,
			rs_B, cs_B,
			rs_C, cs_C,
			A_distributed_ref,
			B_distributed_ref,
			C_distributed_ref );


      // Collect the distributed data and write it to a sequential buffer
      COLLECT_DATA_NAME_REF( m0, n0, rs_C, cs_C,
			     C_distributed_ref,
			     C_sequential_ref );

      // Finally free the buffers
      DISTRIBUTED_FREE_NAME_REF( m0, n0, k0,
				 rs_A, cs_A,
				 rs_B, cs_B,
				 rs_C, cs_C,
				 A_distributed_ref,
				 B_distributed_ref,
				 C_distributed_ref );

      // run the test
      float *A_distributed_tst;
      float *B_distributed_tst;
      float *C_distributed_tst;

      // Allocate distributed buffers for the reference
      DISTRIBUTED_ALLOCATE_NAME_TST( m0, n0, k0,
				     rs_A,  cs_A,
				     rs_B,  cs_B,
				     rs_C,  cs_C,
				     &A_distributed_tst,
				     &B_distributed_tst,
				     &C_distributed_tst );

      // Distribute the sequential buffers
      DISTRIBUTE_DATA_NAME_TST( m0, n0, k0,
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
      COMPUTE_NAME_TST( m0, n0, k0,
			rs_A, cs_A,
			rs_B, cs_B,
			rs_C, cs_C,
			A_distributed_tst,
			B_distributed_tst,
			C_distributed_tst );


      // Collect the distributed data and write it to a sequential buffer
      COLLECT_DATA_NAME_TST( m0, n0, rs_C, cs_C,
			     C_distributed_tst,
			     C_sequential_tst );

      // Finally free the buffers
      DISTRIBUTED_FREE_NAME_TST( m0, n0, k0,
				 rs_A, cs_A,
				 rs_B, cs_B,
				 rs_C, cs_C,
				 A_distributed_tst,
				 B_distributed_tst,
				 C_distributed_tst );

      // We only need to verify the results sequentially
      if( rid == 0)
	{
	  /* root node */

	  //float res = max_pair_wise_diff(m0,n0,rs_C,cs_B, C_sequential_ref, C_sequential_tst);
	  float res = avg_pair_wise_diff(m0,n0,rs_C,cs_B, C_sequential_ref, C_sequential_tst);

	  fprintf(result_file, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%f,",
		  num_ranks,
		  m0,n0,k0,
		  rs_A, cs_A,
		  rs_B, cs_B,
		  rs_C, cs_C,
		  res);

	  // if our error is greater than some threshold
	  if( res > ERROR_THRESHOLD )
	    {
	      fprintf(result_file, "FAIL\n");
#if 1
	      if ( m0 < 32 & n0 < 32 )
		{
		  print_matrix(m0, n0, rs_C, cs_C, C_sequential_ref);printf("\n");
		  print_matrix(m0, n0, rs_C, cs_C, C_sequential_tst);
		}
#endif	      
	    }
	  else
	    fprintf(result_file, "PASS\n");
	}
      else
	{/* all other nodes */}

      // Free the sequential buffers
      free(A_sequential_ref);
      free(C_sequential_ref);
      free(B_sequential_ref);
      free(A_sequential_tst);
      free(C_sequential_tst);
      free(B_sequential_tst);

    }


  // Only needs to be done by root node
  if(rid == 0)
    {
      /* root node */
      fclose(result_file);
    }
  else
    {/* all other nodes */}



 MPI_Finalize();
}
