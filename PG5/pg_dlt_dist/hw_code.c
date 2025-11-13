/*
  Data Layout Transformations and Data Distributions

  Instructions: find all instances of "STUDENT_TODO" and modify the code until
                the function passes.

                At the command prompt in the directory containing this code
                run 'make test'

  Submission: For this assignment you will upload three artifacts to canvas.
              1. [figures.pdf] containing pictures describing the movements
                               being performed for the Data Layout Transforms
			       and Data Distributions.
	      2. [results.txt] containing the test output of your code.
	      3. [code.c] Your modified version of this code.


  - richard.m.veras@ou.edu
*/


//#define DEBUG 0
#define DEBUG 1 // Will print out every index variable



/*

*/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <immintrin.h>


/*
  Helper functions
*/

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



void print_8xfloat_mem(char *name, float *src)
{
  const int vlen = 8;

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

void print_float_mem_as_vects(char *name, int size, int vlen, float *src)
{

  for( int i = 0; i < size; i+=vlen )
    {
      printf("%s[%2i:%2i] = [ ",name,i,i+vlen);
      for( int vid = 0; vid < vlen; ++vid )
	{
	  if ( src[vid+i] < 0.0f )
	    printf( " x, ", src[vid+i] );
	  else
	    printf( "%2.f, ", src[vid+i] );
	}
      printf("]\n");
    }
  printf("\n");
}


void fill_with_float(int n, float *dst, float value )
{
  for (int i = 0; i < n; ++i )
    dst[i]=value;
}

void fill_with_sequence_float(int n, float *dst )
{
  for (int i = 0; i < n; ++i )
    dst[i]=(float) (i+1);
}


/*



 */



// DLT 1
void reference_dlt_2d(int n, float *src, float *dst)
{
  for( int i = 0; i < n; ++i )
    {
      dst[i] = src[i];
    }
}


// 2d here means we have 2 levels of indexing that we are flattening.
void student_nodlt_2d(int n, int opt, int blk, float *src, float *dst)
{
  int bound0;
  int block0;
  int bound1;
  int block1;

  // NOTE: your code should be agnostic to which loop is the "inner" or "outer"
  switch(opt) 
    {
    case 0: 
      bound0 = n;
      block0 = blk;
      bound1 = block0;
      block1 = 1;
      break;
    case 1:
      bound1 = n;
      block1 = blk;
      bound0 = block1;
      block0 = 1;
      break;
    }
  if(DEBUG){printf("\n\nDBG> n: %i opt: %i, bound0: %i, block0: %i, bound1: %i, block1: %i,  \n",n,opt,bound0,block0,bound1,block1);}

  // COMPUTE LOOPS
  // loop 0
  for(int i0 = 0; i0 < bound0; i0 += block0 )
    // loop 1
    for(int i1 = 0; i1 < bound1; i1 += block1 )
      {
	/* 
	   STUDENT_TODO: Add student code below.
	*/
	
	// Only modify this block
	int idx_dst = i0 + i1;
	int idx_src = 0; // FIXME!

	/* 
	   End of student code.
	*/


	if(DEBUG){printf("DBG> idx_dst: %i, idx_src %i, i0: %i, i1: %i\n",idx_dst,idx_src,i0,i1);}
	dst[idx_dst] =src[idx_src];
      }
  
}


void student_dlt_c_array_2d(int n, int opt, int blk, float *src, float *dst)
{
  int bound0;
  int block0;
  int bound1;
  int block1;

  // NOTE: your code should be agnostic to which loop is the "inner" or "outer"
  switch(opt) 
    {
    case 0: 
      bound0 = n;
      block0 = blk;
      bound1 = block0;
      block1 = 1;
      break;
    case 1:
      bound1 = n;
      block1 = blk;
      bound0 = block1;
      block0 = 1;
      break;
    }
  if(DEBUG){printf("\n\nDBG> n: %i opt: %i, bound0: %i, block0: %i, bound1: %i, block1: %i,  \n",n,opt,bound0,block0,bound1,block1);}    

  const int num_elements0 = bound0/block0;
  const int num_elements1 = bound1/block1;
  

  // NOTE: DLT version of "in"
  // NOTE: With multidimension arrays in C:  BUFF[biggest stride][]..[][unit stride]
  float src_dlt[num_elements0][num_elements1];
  
  // DLT PACK LOOPS
  // loop 0
  for(int i0 = 0; i0 < bound0; i0 += block0 )
    // loop 1 
    for(int i1 = 0; i1 < bound1; i1 += block1 )
      {
	const int idx_src = i0 + i1;
	const int idx0_src_dlt   = i0 / block0;
	const int idx1_src_dlt   = i1 / block1;
	
	src_dlt[idx0_src_dlt][idx1_src_dlt] = src[idx_src];
      }
  
  

  // COMPUTE LOOPS
  // loop 0
  for(int i0 = 0; i0 < bound0; i0 += block0 )
    // loop 1 
    for(int i1 = 0; i1 < bound1; i1 += block1 )
      {
	/* 
	   STUDENT_TODO: Add student code below.
	*/
	
	// Only modify this block
	const int idx_dst        = i0 + i1;
	const int idx0_src_dlt   = 0;// FIXME!
	const int idx1_src_dlt   = 0;// FIXME!

	/* 
	   End of student code.
	*/


	if(DEBUG){printf("DBG> idx_dst: %i, idx0_src_dlt %i, idx1_src_dlt %i, i0: %i, i1: %i\n",idx_dst,idx0_src_dlt,idx1_src_dlt,i0,i1);}
	dst[idx_dst] = src_dlt[idx0_src_dlt][idx1_src_dlt];
      }  
}

void student_dlt_c_array_linearized_2d(int n, int opt, int blk, float *src, float *dst)
{
  int bound0;
  int block0;
  int bound1;
  int block1;

  // NOTE: your code should be agnostic to which loop is the "inner" or "outer"
  switch(opt) 
    {
    case 0: 
      bound0 = n;
      block0 = blk;
      bound1 = block0;
      block1 = 1;
      break;
    case 1:
      bound1 = n;
      block1 = blk;
      bound0 = block1;
      block0 = 1;
      break;
    }
  if(DEBUG){printf("\n\nDBG> n: %i opt: %i, bound0: %i, block0: %i, bound1: %i, block1: %i,  \n",n,opt,bound0,block0,bound1,block1);}

  
  const int num_elements0 = bound0/block0;
  const int num_elements1 = bound1/block1;
  

  // NOTE: DLT version of "in"
  // NOTE: With multidimension arrays in C:  BUFF[biggest stride][]..[][unit stride]
  float src_dlt[num_elements0*num_elements1];
  
  // DLT PACK LOOPS
  // loop 0
  for(int i0 = 0; i0 < bound0; i0 += block0 )
    // loop 1 
    for(int i1 = 0; i1 < bound1; i1 += block1 )
      {
	const int idx_src = i0 + i1;

	/* 
	   STUDENT_TODO: Add student code below.
	*/
	// Only modify this block
	const int idx0_src_dlt   = 0;// FIXME!
	const int idx1_src_dlt   = 0;// FIXME!
	
	src_dlt[idx0_src_dlt * num_elements1 + idx1_src_dlt] = src[idx_src];
	/* 
	   End of student code.
	*/

      }
  
  

  // COMPUTE LOOPS
  // loop 0
  for(int i0 = 0; i0 < bound0; i0 += block0 )
    // loop 1 
    for(int i1 = 0; i1 < bound1; i1 += block1 )
      {
	const int idx_dst        = i0 + i1;
	const int idx0_src_dlt   = i0 / block0;
	const int idx1_src_dlt   = i1 / block1;

	if(DEBUG){printf("DBG> idx_dst: %i, idx0_src_dlt %i, idx1_src_dlt %i, i0: %i, i1: %i\n",idx_dst,idx0_src_dlt,idx1_src_dlt,i0,i1);}
	dst[idx_dst] = src_dlt[idx0_src_dlt  * num_elements1 + idx1_src_dlt];
      }
  
}

void student_dlt_inc_2d(int n, int opt, int blk, float *src, float *dst)
{
  int bound0;
  int block0;
  int bound1;
  int block1;

  // NOTE: your code should be agnostic to which loop is the "inner" or "outer"
  switch(opt) 
    {
    case 0: 
      bound0 = n;
      block0 = blk;
      bound1 = block0;
      block1 = 1;
      break;
    case 1:
      bound1 = n;
      block1 = blk;
      bound0 = block1;
      block0 = 1;
      break;
    }
  if(DEBUG){printf("\n\nDBG> n: %i opt: %i, bound0: %i, block0: %i, bound1: %i, block1: %i,  \n",n,opt,bound0,block0,bound1,block1);}

  
  const int num_elements0 = bound0/block0;
  const int num_elements1 = bound1/block1;
  

  // NOTE: DLT version of "in"
  // NOTE: With multidimension arrays in C:  BUFF[biggest stride][]..[][unit stride]
  float src_dlt[num_elements0*num_elements1];
  int src_dlt_idx = 0;
  
  // DLT PACK LOOPS
  // loop 0
  for(int i0 = 0; i0 < bound0; i0 += block0 )
    // loop 1 
    for(int i1 = 0; i1 < bound1; i1 += block1 )
      {
	const int idx_src = i0 + i1;
	
	src_dlt[src_dlt_idx] = src[idx_src];

	/* 
	   STUDENT_TODO: Add student code below.
	*/
	
	// Only modify this block
	src_dlt_idx = 0; // FIXME!

	/* 
	   End of student code.
	*/

	
      }
  
  

  // COMPUTE LOOPS
  src_dlt_idx = 0;
  // loop 0
  for(int i0 = 0; i0 < bound0; i0 += block0 )
    // loop 1 
    for(int i1 = 0; i1 < bound1; i1 += block1 )
      {
	const int idx_dst = i0 + i1;


	if(DEBUG){printf("DBG> idx_dst: %i, src_dlt_idx %i, i0: %i, i1: %i\n",idx_dst,src_dlt_idx,i0,i1);}

	dst[idx_dst] = src_dlt[src_dlt_idx];

	src_dlt_idx = src_dlt_idx + 1;
      }
  
}


void test_dlt_2d(int student_case, int opt)
{
  const int n   = 8;
  const int blk = 4;
  
  float a[n];
  float bt[n];
  float br[n];

  fill_with_sequence_float(n,a);
  fill_with_float(n,bt,-1);
  fill_with_float(n,br,-1);

  reference_dlt_2d(n,a, bt);

  switch (student_case)
    {
    case 0:
      student_nodlt_2d(n,opt,blk,a, br);
      break;
    case 1:
      student_dlt_c_array_2d(n,opt,blk,a, br);
      break;
    case 2:
      student_dlt_c_array_linearized_2d(n,opt,blk,a, br);
      break;
    case 3:
      student_dlt_inc_2d(n,opt,blk,a, br);
      break;
    default:
      printf("bad case.\n");
      break;
    }


  float res = max_pair_wise_diff(n, bt, br);

  printf("test_dlt_2d: ");
  if( res > 1e-6 )
    {
      printf("FAIL");

      print_float_mem(" a", n, a);
      print_float_mem("bt", n, bt);
      print_float_mem("br", n, br);

      printf("\n");
    }
  else
    {
      printf("PASS");
    }
}


// 1d here means we only have one dimension of processes
// that we need to distribute over.
void reference_dist_1d(int n, float *src, float *dst)
{
  for( int i = 0; i < n; ++i )
    {
      dst[i] = src[i];
    }
}

// block
void student_dist_block_1d(int n, int num_of_ranks, float *src, float *dst)
{
  const int num_of_elems_per_rank = n / num_of_ranks;
  float src_dist[num_of_ranks][num_of_elems_per_rank];

  // NOTE: The above layout is arbitrary in that we are only concerned
  //       about mapping one logical layout to another, wheras for DLT
  //       we are worried about logical to physical mapping. So the
  //       following would work too.
  //float src_dist[num_of_elems_per_rank][num_of_ranks];
  //float src_dist[num_of_ranks*num_of_elems_per_rank];
  //float src_dist[num_of_elems_per_rank*num_of_ranks];
  // NOTE: Once we have a distribution determined, we can layer DLT
  //       on top of that.

  float dst_dist[num_of_ranks][num_of_elems_per_rank];

  // DISTRIBUTE the data
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int eid = 0; eid < num_of_elems_per_rank; eid += 1 )
      {
	const int i = rid*num_of_elems_per_rank + eid;

	// NOTE: In a distibuted memory system the rank owning src[i] would have
	//       to broadcast that to the rank rid needing that data.
	src_dist[rid][eid] = src[i];

	if(DEBUG){printf("DBG-DIST> rid: %i, eid: %i, i: %i\n",rid, eid, i);}
	
      }
    

  // COMPUTE on the distributed data
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int eid = 0; eid < num_of_elems_per_rank; eid += 1 )
      {
	dst_dist[rid][eid] = src_dist[rid][eid];

	if(DEBUG){printf("DBG-COMP> rid: %i, eid: %i\n",rid, eid);}
      }


  // COLLECT the results (why am I capitalizing certain words? Where
  // in the lab do you see those words)
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int eid = 0; eid < num_of_elems_per_rank; eid += 1 )
      {
	/* 
	   STUDENT_TODO: Add student code below.
	*/
	// Only modify this block
	const int i = 0; // FIXME!
	/* 
	   End of student code.
	*/
	
	// NOTE: In a distibuted memory system the rank owning src[i] would have
	//       to broadcast that to the rank rid needing that data.
	dst[i] = dst_dist[rid][eid];
	if(DEBUG){printf("DBG-COLL> rid: %i, eid: %i, i: %i\n",rid, eid, i);}
      }

}

// cyclic
void student_dist_cyclic_1d(int n, int num_of_ranks, float *src, float *dst)
{
  const int num_of_elems_per_rank = n / num_of_ranks;
  float src_dist[num_of_ranks][num_of_elems_per_rank];

  // NOTE: The above layout is arbitrary in that we are only concerned
  //       about mapping one logical layout to another, wheras for DLT
  //       we are worried about logical to physical mapping. So the
  //       following would work too.
  //float src_dist[num_of_elems_per_rank][num_of_ranks];
  //float src_dist[num_of_ranks*num_of_elems_per_rank];
  //float src_dist[num_of_elems_per_rank*num_of_ranks];
  // NOTE: Once we have a distribution determined, we can layer DLT
  //       on top of that.

  float dst_dist[num_of_ranks][num_of_elems_per_rank];

  // DISTRIBUTE the data
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int eid = 0; eid < num_of_elems_per_rank; eid += 1 )
      {
	const int i = rid + eid * num_of_ranks;

	// NOTE: In a distibuted memory system the rank owning src[i] would have
	//       to broadcast that to the rank rid needing that data.
	src_dist[rid][eid] = src[i];

	if(DEBUG){printf("DBG-DIST> rid: %i, eid: %i, i: %i\n",rid, eid, i);}
	
      }
    

  // COMPUTE on the distributed data
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int eid = 0; eid < num_of_elems_per_rank; eid += 1 )
      {
	dst_dist[rid][eid] = src_dist[rid][eid];

	if(DEBUG){printf("DBG-COMP> rid: %i, eid: %i\n",rid, eid);}
      }


  // COLLECT the results (why am I capitalizing certain words? Where
  // in the lab do you see those words)
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int eid = 0; eid < num_of_elems_per_rank; eid += 1 )
      {
	/* 
	   STUDENT_TODO: Add student code below.
	*/
	// Only modify this block
	const int i = 0; // FIXME! 
	/* 
	   End of student code.
	*/
	
	// NOTE: In a distibuted memory system the rank owning src[i] would have
	//       to broadcast that to the rank rid needing that data.
	dst[i] = dst_dist[rid][eid];
	if(DEBUG){printf("DBG-COLL> rid: %i, eid: %i, i: %i\n",rid, eid, i);}
      }

}


// block cyclic
void student_dist_block_cyclic_1d(int n, int num_of_ranks, int num_of_elems_per_block, float *src, float *dst)
{
  const int num_of_blocks          = n / num_of_elems_per_block; // total number of blocks
  const int num_of_elems_per_rank  = n / num_of_ranks;
  const int num_of_blocks_per_rank = num_of_elems_per_rank / num_of_elems_per_block;

  if(DEBUG){
    printf("DBG-INIT> n: %i\n", n);
    printf("DBG-INIT> num_of_ranks: %i\n", num_of_ranks);
    printf("DBG-INIT> num_of_elems_per_block: %i\n", num_of_elems_per_block);
    printf("DBG-INIT> num_of_blocks: %i\n", num_of_blocks );
    printf("DBG-INIT> num_of_elems_per_rank: %i\n ", num_of_elems_per_rank );
    printf("DBG-INIT> num_of_blocks_per_rank: %i\n", num_of_blocks_per_rank );
  }

  float src_dist[num_of_ranks][num_of_blocks_per_rank][num_of_elems_per_block];

  // NOTE: The above layout is arbitrary in that we are only concerned
  //       about mapping one logical layout to another, wheras for DLT
  //       we are worried about logical to physical mapping. So the
  //       following would work too.
  // float src_dist[num_of_ranks][num_of_blocks_per_rank][num_of_elems_per_block];
  // float src_dist[num_of_ranks][num_of_elems_per_block][num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank][num_of_elems_per_block][num_of_ranks];
  // float src_dist[num_of_elems_per_block][num_of_blocks_per_rank][num_of_ranks];
  // float src_dist[num_of_elems_per_block][num_of_ranks][num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank][num_of_ranks][num_of_elems_per_block];
  // float src_dist[num_of_ranks*num_of_blocks_per_rank*num_of_elems_per_block];
  // float src_dist[num_of_ranks*num_of_elems_per_block*num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank*num_of_elems_per_block*num_of_ranks];
  // float src_dist[num_of_elems_per_block*num_of_blocks_per_rank*num_of_ranks];
  // float src_dist[num_of_elems_per_block*num_of_ranks*num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank*num_of_ranks*num_of_elems_per_block];

  
  // NOTE: Once we have a distribution determined, we can layer DLT
  //       on top of that.

  float dst_dist[num_of_ranks][num_of_blocks_per_rank][num_of_elems_per_block];

  // DISTRIBUTE the data
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int bid = 0; bid < num_of_blocks_per_rank; bid += 1 )
      for( int eid = 0; eid < num_of_elems_per_block; eid += 1 )
	{
	  const int i = bid * num_of_ranks * num_of_elems_per_block + rid * num_of_elems_per_block + eid;

	  // NOTE: In a distibuted memory system the rank owning src[i] would have
	  //       to broadcast that to the rank rid needing that data.
	  src_dist[rid][bid][eid] = src[i];

	  if(DEBUG){printf("DBG-DIST> rid: %i, bid: %i, eid: %i, i: %i\n",rid, bid, eid, i);}
	
	}
    

  // COMPUTE on the distributed data
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int bid = 0; bid < num_of_blocks_per_rank; bid += 1 )
      for( int eid = 0; eid < num_of_elems_per_block; eid += 1 )
	{
	  dst_dist[rid][bid][eid] = src_dist[rid][bid][eid];

	  if(DEBUG){printf("DBG-DIST> rid: %i, bid: %i, eid: %i\n",rid, bid, eid);}
	}


  // COLLECT the results (why am I capitalizing certain words? Where
  // in the lab do you see those words)
  for( int rid = 0; rid < num_of_ranks; rid += 1 ) // or pid, tid, vid, etc..
    for( int bid = 0; bid < num_of_blocks_per_rank; bid += 1 )
      for( int eid = 0; eid < num_of_elems_per_block; eid += 1 )
	{
	  /* 
	     STUDENT_TODO: Add student code below.
	  */
	  // Only modify this block
	  const int i = 0; // FIXME!
	  /* 
	     End of student code.
	  */
	
	  // NOTE: In a distibuted memory system the rank owning src[i] would have
	  //       to broadcast that to the rank rid needing that data.
	  dst[i] = dst_dist[rid][bid][eid];
	  if(DEBUG){printf("DBG-COLL> rid: %i, bid: %i, eid: %i, i: %i\n",rid, bid, eid, i);}
	}

}


// NOTE: This is hard, but once you figure out these mappings, everything will become easier.
// block cyclic - but deriving rid, bid, and eid from i
void student_dist_block_cyclic_1d_the_hard_one(int n, int num_of_ranks, int num_of_elems_per_block, float *src, float *dst)
{
  const int num_of_blocks          = n / num_of_elems_per_block; // total number of blocks
  const int num_of_elems_per_rank  = n / num_of_ranks;
  const int num_of_blocks_per_rank = num_of_elems_per_rank / num_of_elems_per_block;

  if(DEBUG){
    printf("DBG-INIT> n: %i\n", n);
    printf("DBG-INIT> num_of_ranks: %i\n", num_of_ranks);
    printf("DBG-INIT> num_of_elems_per_block: %i\n", num_of_elems_per_block);
    printf("DBG-INIT> num_of_blocks: %i\n", num_of_blocks );
    printf("DBG-INIT> num_of_elems_per_rank: %i\n ", num_of_elems_per_rank );
    printf("DBG-INIT> num_of_blocks_per_rank: %i\n", num_of_blocks_per_rank );
  }

  float src_dist[num_of_ranks][num_of_blocks_per_rank][num_of_elems_per_block];

  // NOTE: The above layout is arbitrary in that we are only concerned
  //       about mapping one logical layout to another, wheras for DLT
  //       we are worried about logical to physical mapping. So the
  //       following would work too.
  // float src_dist[num_of_ranks][num_of_blocks_per_rank][num_of_elems_per_block];
  // float src_dist[num_of_ranks][num_of_elems_per_block][num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank][num_of_elems_per_block][num_of_ranks];
  // float src_dist[num_of_elems_per_block][num_of_blocks_per_rank][num_of_ranks];
  // float src_dist[num_of_elems_per_block][num_of_ranks][num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank][num_of_ranks][num_of_elems_per_block];
  // float src_dist[num_of_ranks*num_of_blocks_per_rank*num_of_elems_per_block];
  // float src_dist[num_of_ranks*num_of_elems_per_block*num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank*num_of_elems_per_block*num_of_ranks];
  // float src_dist[num_of_elems_per_block*num_of_blocks_per_rank*num_of_ranks];
  // float src_dist[num_of_elems_per_block*num_of_ranks*num_of_blocks_per_rank];
  // float src_dist[num_of_blocks_per_rank*num_of_ranks*num_of_elems_per_block];

  
  // NOTE: Once we have a distribution determined, we can layer DLT
  //       on top of that.

  float dst_dist[num_of_ranks][num_of_blocks_per_rank][num_of_elems_per_block];

  // DISTRIBUTE the data
  for( int i = 0; i < n; i+=1 )
    {
      /* 
	 STUDENT_TODO: Add student code below.
      */
      // Only modify this block
      const int bid = i / (num_of_ranks * num_of_elems_per_block); 
      const int rid = 0; // FIXME (i % ((XX * ZZ)) ) / YY; // where XX, ZZ, and YY are something?
      const int eid = i % num_of_elems_per_block;
      /* 
	 End of student code.
      */

      // NOTE: In a distibuted memory system the rank owning src[i] would have
      //       to broadcast that to the rank rid needing that data.
      src_dist[rid][bid][eid] = src[i];

      if(DEBUG){printf("DBG-DIST> rid: %i, bid: %i, eid: %i, i: %i\n",rid, bid, eid, i);}
	
    }
    

  // COMPUTE on the distributed data
  for( int i = 0; i < n; i+=1 )
    {
      /* 
	 STUDENT_TODO: Add student code below.
      */
      // Only modify this block
      const int bid = i / (num_of_ranks * num_of_elems_per_block);
      const int rid = 0; // FIXME (i % ((XX * ZZ)) ) / YY;
      const int eid = i % num_of_elems_per_block;
      /* 
	 End of student code.
      */

      dst_dist[rid][bid][eid] = src_dist[rid][bid][eid];

      if(DEBUG){printf("DBG-DIST> rid: %i, bid: %i, eid: %i\n",rid, bid, eid);}
    }


  // COLLECT the results (why am I capitalizing certain words? Where
  // in the lab do you see those words)
  for( int i = 0; i < n; i+=1 )
    {
      /* 
	 STUDENT_TODO: Add student code below.
      */
      // Only modify this block
      const int bid = i / (num_of_ranks * num_of_elems_per_block);
      const int rid = 0; // FIXME (i % ((XX * ZZ)) ) / YY;
      const int eid = i % num_of_elems_per_block;

      /* 
	 End of student code.
      */
	
      // NOTE: In a distibuted memory system the rank owning src[i] would have
      //       to broadcast that to the rank rid needing that data.
      dst[i] = dst_dist[rid][bid][eid];
      if(DEBUG){printf("DBG-COLL> rid: %i, bid: %i, eid: %i, i: %i\n",rid, bid, eid, i);}
    }

}


void test_dist_1d(int student_case, int opt)
{
  const int n                    = 16;
  const int num_ranks            = 4; // or num_procs, num_threads, num_simd_lanes, num_some_parallel_units
  const int num_elems_per_block  = 2;
  
  float a[n];
  float bt[n];
  float br[n];

  fill_with_sequence_float(n,a);
  fill_with_float(n,bt,-1);
  fill_with_float(n,br,-1);

  reference_dlt_2d(n,a, bt);

  switch (student_case)
    {
    case 0:
      student_dist_block_1d(n, num_ranks, a, br);
      break;
    case 1:
      student_dist_cyclic_1d(n, num_ranks, a, br);
      break;

    case 2:
      if (opt == 0) // block-cyclic with block size of 2
	student_dist_block_cyclic_1d(n, num_ranks, 2, a, br);
      else if (opt == 1) // block (block size =n/num_ranks)
	student_dist_block_cyclic_1d(n, num_ranks, n/num_ranks, a, br);
      else // cyclic (block size = 1)
	student_dist_block_cyclic_1d(n, num_ranks, 1, a, br);
      break;

    case 3:
      if (opt == 0) // block-cyclic with block size of 2
	student_dist_block_cyclic_1d_the_hard_one(n, num_ranks, 2, a, br);
      else if (opt == 1) // block (block size =n/num_ranks)
	student_dist_block_cyclic_1d_the_hard_one(n, num_ranks, n/num_ranks, a, br);
      else // cyclic (block size = 1)
	student_dist_block_cyclic_1d_the_hard_one(n, num_ranks, 1, a, br);
      break;

    default:
      printf("bad case.\n");
      break;
    }


  float res = max_pair_wise_diff(n, bt, br);

  printf("test_dist_1d: ");
  if( res > 1e-6 )
    {
      printf("FAIL");

      print_float_mem(" a", n, a);
      print_float_mem("bt", n, bt);
      print_float_mem("br", n, br);

      printf("\n");
    }
  else
    {
      printf("PASS");
    }
}



int main( int argc, char *argv[] )
{
  // DLT
  test_dlt_2d(0,0);printf(": 01.0\n");
  test_dlt_2d(0,1);printf(": 01.1\n");
  test_dlt_2d(1,0);printf(": 02.0\n");
  test_dlt_2d(1,1);printf(": 02.1\n");
  test_dlt_2d(2,0);printf(": 03.0\n");
  test_dlt_2d(2,1);printf(": 03.1\n");
  test_dlt_2d(3,0);printf(": 04.0\n");
  test_dlt_2d(3,1);printf(": 04.1\n");

  
  // DATA Distribution
  test_dist_1d(0, 0);printf(": 05.0\n");            
  test_dist_1d(1, 0);printf(": 06.0\n");            
  test_dist_1d(2, 0);printf(": 07.0 block-cyclic\n");
  test_dist_1d(2, 1);printf(": 07.1 block\n");       
  test_dist_1d(2, 2);printf(": 07.2 cyclic\n");      
  test_dist_1d(3, 0);printf(": 08.0 block-cyclic\n");
  test_dist_1d(3, 1);printf(": 08.1 block\n");       
  test_dist_1d(3, 2);printf(": 08.2 cyclic\n");      

  return 0;
}



