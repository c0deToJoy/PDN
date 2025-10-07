/*
  HW SIMD!

  Instructions: find all instances of "STUDENT_TODO" and replace with SIMD code
                that makes the test corresponding to that function pass.

                At the command prompt in the directory containing this code
                run 'make test'

		You can simulate the performance of each implementation using
		run 'make measure-osaca'

  Submission: For this assignment you will upload the following to canvas.
              1. [figures.pdf] containing pictures describing the movements
                               being performed by the SIMD instructions.
			       Additionally, you will include the OSACA table
			       for each reference and student implementation.

			       You are encouraged to use the provided slides.

	      2. [code.c] Your modified version of this code.


  - richard.m.veras@ou.edu
*/



/*
  NOTE: You can use any instructions from:
     https://software.intel.com/sites/landingpage/IntrinsicsGuide/#techs=SSE4_1,AVX,AVX2

  But, I strongly suggest the following instructions:


  // Load
  __m256 _mm256_loadu_ps (float const * mem_addr)

  // Store
  void _mm256_storeu_ps (float * mem_addr, __m256 a)

  // set
  __m256 _mm256_set1_ps (float a)

  // extract
  __m128 _mm256_extractf128_ps (__m256 a, const int imm8) // Also need __m128 version
  int _mm_extract_ps (__m128 a, const int imm8) // SSE 4.1

  // permute with one input
  __m256 _mm256_permute_ps (__m256 a, int imm8) // w/in lane
  __m256 _mm256_permutevar8x32_ps (__m256 a, __m256i idx) // across lane


  // Blend
  __m256 _mm256_blend_ps (__m256 a, __m256 b, const int imm8)


  // Shuffle with two inputs
  __m256 _mm256_shuffle_ps (__m256 a, __m256 b, const int imm8)


  // Math
  __m256 _mm256_fmadd_ps (__m256 a, __m256 b, __m256 c)

*/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <immintrin.h>

// NOTE: This include is only needed for osaca
#include "instruments.h"


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



// SIMD # 1
void reference_reverse_8xfloat(float *src, float *dst)
{
  const int vlen = 8;


  for( int vid = 0; vid < vlen; ++vid )
    {
      // This {BEGIN,END}_INSTRUMENTATION marks off a block to be simulated with 'make measure-osaca'
      // Only one block can be measured at a time, so comment out all the others.
      // NOTE: The OSACA report for the reference code is for one iteration or 1/vlen the amount of work
      //       that your student SIMD code will do.
      BEGIN_INSTRUMENTATION; // Mark the start of the code for osaca/mca

      dst[(vlen-1)-vid] = src[vid];

      END_INSTRUMENTATION; //
    }



}

void student_reverse_8xfloat(float *src, float *dst)
{

  __m256 output = _mm256_set1_ps(-1.0); // for debugging
  __m256 input = _mm256_loadu_ps(&src[0]);


  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;
  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }
  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[0],output);

}

void test_reverse_8xfloat()
{
  float a[] = {0,1,2,3,4,5,6,7};
  float bt[8] = {-1,-1,-1,-1, -1,-1,-1,-1};
  float br[8] = {-1,-1,-1,-1, -1,-1,-1,-1};


  reference_reverse_8xfloat(a, bt);
  student_reverse_8xfloat(a, br);


  float res = max_pair_wise_diff(8, bt, br);

  printf("test_reverse_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_8xfloat_mem(" a", a);
      print_8xfloat_mem("bt", bt);
      print_8xfloat_mem("br", br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}

////////////////
// SIMD # 2.1 //
////////////////
void reference_rotate_by_4_8xfloat(float *src, float *dst)
{
  const int vlen = 8;
  const int shift = 4;

  for( int vid = 0; vid < vlen; ++vid )
    {
      // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
      //BEGIN_INSTRUMENTATION;

      dst[(vlen+(vid - shift))%(vlen)] = src[vid];

      //END_INSTRUMENTATION;
    }
}

void student_rotate_by_4_8xfloat(float *src, float *dst)
{

  __m256 output = _mm256_set1_ps(-1);
  __m256 input = _mm256_loadu_ps(&src[0]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;
  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }

    //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[0],output);
}

void test_rotate_by_4_8xfloat()
{
  float a[] = {0,1,2,3,4,5,6,7};
  float bt[8] = {-1,-1,-1,-1, -1,-1,-1,-1};
  float br[8] = {-1,-1,-1,-1, -1,-1,-1,-1};


  reference_rotate_by_4_8xfloat(a, bt);
  student_rotate_by_4_8xfloat(a, br);


  float res = max_pair_wise_diff(8, bt, br);

  printf("test_rotate_by_4_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_8xfloat_mem(" a", a);
      print_8xfloat_mem("bt", bt);
      print_8xfloat_mem("br", br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}

////////////////
// SIMD # 2.2 //
////////////////
void reference_rotate_by_2_8xfloat(float *src, float *dst)
{
  const int vlen = 8;
  const int shift = 2;

  for( int vid = 0; vid < vlen; ++vid )
    {
      // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
      //BEGIN_INSTRUMENTATION;

      dst[(vlen+(vid - shift))%(vlen)] = src[vid];

      //END_INSTRUMENTATION;
    }
}

void student_rotate_by_2_8xfloat(float *src, float *dst)
{

  __m256 output = _mm256_set1_ps(-1.0); // for debugging
  __m256 input = _mm256_loadu_ps(&src[0]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }
  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[0],output);
}

void test_rotate_by_2_8xfloat()
{
  float a[] = {0,1,2,3,4,5,6,7};
  float bt[8] = {-1,-1,-1,-1, -1,-1,-1,-1};
  float br[8] = {-1,-1,-1,-1, -1,-1,-1,-1};


  reference_rotate_by_2_8xfloat(a, bt);
  student_rotate_by_2_8xfloat(a, br);


  float res = max_pair_wise_diff(8, bt, br);

  printf("test_rotate_by_2_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_8xfloat_mem(" a", a);
      print_8xfloat_mem("bt", bt);
      print_8xfloat_mem("br", br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}


////////////////
// SIMD # 2.3 //
////////////////
void reference_rotate_by_1_8xfloat(float *src, float *dst)
{
  const int vlen = 8;
  const int shift = 1;

  for( int vid = 0; vid < vlen; ++vid )
    {
      // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
      //BEGIN_INSTRUMENTATION;

      dst[(vlen+(vid - shift))%(vlen)] = src[vid];

      //END_INSTRUMENTATION;
    }
}

void student_rotate_by_1_8xfloat(float *src, float *dst)
{

  __m256 output = _mm256_set1_ps(-1.0); // for debugging
  __m256 input = _mm256_loadu_ps(&src[0]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }
  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[0],output);
}


void test_rotate_by_1_8xfloat()
{
  float a[] = {0,1,2,3,4,5,6,7};
  float bt[8] = {-1,-1,-1,-1, -1,-1,-1,-1};
  float br[8] = {-1,-1,-1,-1, -1,-1,-1,-1};

    reference_rotate_by_1_8xfloat(a, bt);
  student_rotate_by_1_8xfloat(a, br);

  float res = max_pair_wise_diff(8, bt, br);

  printf("test_rotate_by_1_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_8xfloat_mem(" a", a);
      print_8xfloat_mem("bt", bt);
      print_8xfloat_mem("br", br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}

/*
  SIMD 3 Transpose 4x2 matrix in column major (i.e. row stride = 4)

 Matrix A                 Matrix A^T
   ___
  |0 4|                    ________
  |1 5|   --- becomes --> | 0 1 2 3|
  |2 6|                   | 4 5 6 7|
  |3 7|                    --------
   ---

  Stored in memory we have:

  [0,1,2,3,4,5,6,7] -- becomes --> [0,4,1,5,2,6,3,7]
*/

void reference_transpose_4x2_colmaj_8xfloat(float *src, float *dst)
{
  const int vlen = 8;
  const int m = 4;
  const int n = 2;
  const int rs_s = m;
  const int cs_s = 1;

  const int rs_d = n;
  const int cs_d = 1;

  for (int i = 0; i < m; ++i )
      for (int j = 0; j < n; ++j )
	dst[j*cs_d + i*rs_d] = src[i*cs_s + j*rs_s];

}

void student_transpose_4x2_colmaj_8xfloat(float *src, float *dst)
{

  __m256 output = _mm256_set1_ps(-1.0); // for debugging
  __m256 input = _mm256_loadu_ps(&src[0]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }
  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[0],output);
}


/*
  SIMD 4: rotate two concatenated vectors by 1
*/

void reference_rotate_by_1_16xfloat(float *src, float *dst)
{
  const int vlen = 16;
  const int shift = 1;

  for( int vid = 0; vid < vlen; ++vid )
    {
      // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
      //BEGIN_INSTRUMENTATION;

      dst[(vlen+(vid - shift))%(vlen)] = src[vid];

      //END_INSTRUMENTATION;
    }
}

void student_rotate_by_1_16xfloat(float *src, float *dst)
{

  __m256 output00_07 = _mm256_set1_ps(-1.0);
  __m256 output08_15 = _mm256_set1_ps(-1.0);
  __m256 input00_07 = _mm256_loadu_ps(&src[0]);
  __m256 input08_15 = _mm256_loadu_ps(&src[8]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }
  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[0],output00_07);
  _mm256_storeu_ps(&dst[8],output08_15);
}


void test_rotate_by_1_16xfloat()
{
  float a[] = {0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15 };
  float bt[16] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1};
  float br[16] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1};


  reference_rotate_by_1_16xfloat(a, bt);
  student_rotate_by_1_16xfloat(a, br);


  float res = max_pair_wise_diff(16, bt, br);

  printf("test_rotate_by_1_16xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_float_mem(" a", 16, a);
      print_float_mem("bt", 16, bt);
      print_float_mem("br", 16, br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}



void test_transpose_4x2_colmaj_8xfloat()
{
  float a[] = {0,1,2,3,4,5,6,7};
  float bt[8] = {-1,-1,-1,-1, -1,-1,-1,-1};
  float br[8] = {-1,-1,-1,-1, -1,-1,-1,-1};


  reference_transpose_4x2_colmaj_8xfloat(a, bt);
  student_transpose_4x2_colmaj_8xfloat(a, br);


  float res = max_pair_wise_diff(8, bt, br);

  printf("test_transpose_4x2_colmaj_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_8xfloat_mem(" a", a);
      print_8xfloat_mem("bt", bt);
      print_8xfloat_mem("br", br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}


/*
  SIMD 5.1 Transpose 4x4 matrix in column major (i.e. row stride = 4)

 Matrix A                 Matrix A^T
   __________                     ____________
  |0  4  8 12|                   |  0  1  2  3|
  |1  5  9 13|   --- becomes --> |  4  5  6  7|
  |2  6 10 14|                   |  8  9 10 11|
  |3  7 11 15|                   | 12 13 14 15|
   ----------                     ------------

  Stored in memory we have:

  [0,1,2,3,4,5,6,7,8,9,10,12,13,14,15] -- becomes --> [0,4,8,12, 1,5,9,13, 2,6,10,14 3,7,11,15]
*/

void reference_transpose_4x4_colmaj_16xfloat(float *src, float *dst)
{
  const int vlen = 8;
  const int m = 4;
  const int n = 4;
  const int rs_s = m;
  const int cs_s = 1;

  const int rs_d = n;
  const int cs_d = 1;

  for (int i = 0; i < m; ++i )
      for (int j = 0; j < n; ++j )
	{
	  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
	  //BEGIN_INSTRUMENTATION;

	  dst[j*cs_d + i*rs_d] = src[i*cs_s + j*rs_s];

	  //END_INSTRUMENTATION;
	}

}

void student_transpose_4x4_colmaj_8xfloat(float *src, float *dst)
{

  __m256 output00_07 = _mm256_set1_ps(-1.0); // for debugging
  __m256 output08_15 = _mm256_set1_ps(-1.0); // for debugging
  __m256 input00_07 = _mm256_loadu_ps(&src[0]);
  __m256 input08_15 = _mm256_loadu_ps(&src[8]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }

  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[0],output00_07);
  _mm256_storeu_ps(&dst[8],output08_15);
}


void test_transpose_4x4_colmaj_8xfloat()
{
  float a[] = {0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15};
  float bt[16] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1};
  float br[16] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1};


  reference_transpose_4x4_colmaj_16xfloat(a, bt);
  student_transpose_4x4_colmaj_8xfloat(a, br);


  float res = max_pair_wise_diff(16, bt, br);

  printf("test_transpose_4x4_colmaj_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_float_mem(" a", 16, a);
      print_float_mem("bt", 16, bt);
      print_float_mem("br", 16, br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}


/*
  SIMD 5.2 Transpose 8x4 matrix in column major (i.e. row stride = 8)

  STUDENT_TODO: Draw a picture!
  STUDENT_TODO: Write out the memory layout!

*/

void reference_transpose_8x4_colmaj_32xfloat(float *src, float *dst)
{
  const int vlen = 8;
  const int m = 8;
  const int n = 4;
  const int rs_s = m;
  const int cs_s = 1;

  const int rs_d = n;
  const int cs_d = 1;

  for (int i = 0; i < m; ++i )
      for (int j = 0; j < n; ++j )
	{
	  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
	  //BEGIN_INSTRUMENTATION;

	  dst[j*cs_d + i*rs_d] = src[i*cs_s + j*rs_s];

	  //END_INSTRUMENTATION;
	}

}

void student_transpose_8x4_colmaj_8xfloat(float *src, float *dst)
{
  __m256 output00_07 = _mm256_set1_ps(-1.0); // for debugging
  __m256 output08_15 = _mm256_set1_ps(-1.0); // for debugging
  __m256 output16_23 = _mm256_set1_ps(-1.0); // for debugging
  __m256 output24_31 = _mm256_set1_ps(-1.0); // for debugging

  __m256 input00_07 = _mm256_loadu_ps(&src[0]);
  __m256 input08_15 = _mm256_loadu_ps(&src[8]);
  __m256 input16_23 = _mm256_loadu_ps(&src[16]);
  __m256 input24_31 = _mm256_loadu_ps(&src[24]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }
  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[ 0],output00_07);
  _mm256_storeu_ps(&dst[ 8],output08_15);
  _mm256_storeu_ps(&dst[16],output16_23);
  _mm256_storeu_ps(&dst[24],output24_31);
}

void test_transpose_8x4_colmaj_8xfloat()
{
  float a[32] = {0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,
                 16,17,18,19,20,21,22,23,24, 25,26,27,28,29,30,31};
  float bt[32] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                  -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1 };

  float br[32] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                  -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1 };


  reference_transpose_8x4_colmaj_32xfloat(a, bt);
  student_transpose_8x4_colmaj_8xfloat(a, br);


  float res = max_pair_wise_diff(32, bt, br);

  printf("test_transpose_8x4_colmaj_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_float_mem_as_vects(" a", 32,8, a);
      print_float_mem_as_vects("bt", 32,8, bt);
      print_float_mem_as_vects("br", 32,8, br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}

//////////////////////////
//// GATHER AND SCATTER //
//////////////////////////

/*
  Gathering at a stride
*/
void reference_gather_at_stride_8xfloat(float *src, float *dst)
{
  const int vlen   = 8;
  const int stride = 4;

  for( int vid = 0; vid < vlen; ++vid )
    {
      // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
      //BEGIN_INSTRUMENTATION;

      dst[vid] = src[vid*stride];

        //END_INSTRUMENTATION;
    }

}


void student_gather_at_stride_8xfloat(float *src, float *dst)
{
  __m256 output00_07 = _mm256_set1_ps(-1.0); // for debugging

  __m256 input00_07 = _mm256_loadu_ps(&src[0]);
  __m256 input08_15 = _mm256_loadu_ps(&src[8]);
  __m256 input16_23 = _mm256_loadu_ps(&src[16]);
  __m256 input24_31 = _mm256_loadu_ps(&src[24]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }

    //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[ 0],output00_07);
}


void test_gather_at_stride_8xfloat()
{
  float a[32] = {0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15,
                 16,17,18,19,20,21,22,23,24, 25,26,27,28,29,30,31};
  float bt[8] = {-1,-1,-1,-1, -1,-1,-1,-1 };

  float br[8] = {-1,-1,-1,-1, -1,-1,-1,-1 };


  reference_gather_at_stride_8xfloat(a, bt);
  student_gather_at_stride_8xfloat(a, br);


  float res = max_pair_wise_diff(8, bt, br);

  printf("test_gather_at_stride_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_float_mem_as_vects(" a", 32,8, a);
      print_float_mem_as_vects("bt", 8,8, bt);
      print_float_mem_as_vects("br", 8,8, br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}




/*
  Scattering at a stride
*/

void reference_scatter_at_stride_8xfloat(float *src, float *dst)
{
  const int vlen   = 8;
  const int stride = 4;

  for( int vid = 0; vid < vlen; ++vid )
    {
      // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
      //BEGIN_INSTRUMENTATION;

      dst[vid*stride] = src[vid];

        //END_INSTRUMENTATION;
    }

}


void student_scatter_at_stride_8xfloat(float *src, float *dst)
{
  __m256 output00_07 = _mm256_set1_ps(-1.0); // for debugging
  __m256 output08_15 = _mm256_set1_ps(-1.0); // for debugging
  __m256 output16_23 = _mm256_set1_ps(-1.0); // for debugging
  __m256 output24_31 = _mm256_set1_ps(-1.0); // for debugging


  __m256 input00_07 = _mm256_loadu_ps(&src[0]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // __m256 XXX = ...
    // ...
    // ...
    // output = ...;

    /*
       End of student code.
    */
  }

  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&dst[ 0],output00_07);
  _mm256_storeu_ps(&dst[ 8],output08_15);
  _mm256_storeu_ps(&dst[16],output16_23);
  _mm256_storeu_ps(&dst[24],output24_31);

}


void test_scatter_at_stride_8xfloat()
{
  float a[8] = {0,1,2,3,4,5,6,7};

  float bt[32] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                  -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1 };

  float br[32] = {-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
                  -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1 };


  reference_scatter_at_stride_8xfloat(a, bt);
  student_scatter_at_stride_8xfloat(a, br);


  float res = max_pair_wise_diff(32, bt, br);

  printf("test_scatter_at_stride_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_float_mem_as_vects(" a", 8,8, a);
      print_float_mem_as_vects("bt", 32,8, bt);
      print_float_mem_as_vects("br", 32,8, br);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}



//////////////////
// DO SOME MATH //
//////////////////




// matvec 1
void reference_matvec_8x8_colmaj_64xfloat(float *A, float *x, float *y)
{
  const int vlen = 8;
  const int m = 8;
  const int n = 8;
  const int rs_s = m;
  const int cs_s = 1;

  for (int i = 0; i < m; ++i )
      for (int j = 0; j < n; ++j )
	{
	  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
	  //BEGIN_INSTRUMENTATION;

	  y[i] += A[i*cs_s + j*rs_s] * x[j];

	  //END_INSTRUMENTATION;
	}
}


void student_matvec_8x8_colmaj_8xfloat(float *A, float *x, float *y)
{
  __m256 y00_07 = _mm256_set1_ps(0.0f);

  __m256 A00_07 = _mm256_loadu_ps(&A[0]);
  __m256 A08_15 = _mm256_loadu_ps(&A[8]);
  __m256 A16_23 = _mm256_loadu_ps(&A[16]);
  __m256 A24_31 = _mm256_loadu_ps(&A[24]);
  __m256 A32_39 = _mm256_loadu_ps(&A[32]);
  __m256 A40_47 = _mm256_loadu_ps(&A[40]);
  __m256 A48_55 = _mm256_loadu_ps(&A[48]);
  __m256 A56_63 = _mm256_loadu_ps(&A[56]);

  // STUDENT_TODO: Uncomment this instrumentation block, comment out the others, then run 'make measure-osaca'
  //BEGIN_INSTRUMENTATION;

  {
    /*
       STUDENT_TODO: Add student code below.
    */

    // ...
    // ... __m256 x_j = _mm256_set1_ps(x[j]);
    // ...
    // ... __m256 temp = _mm256_fmadd_ps(....)
    // ...

    /*
       End of student code.
    */
  }

  //END_INSTRUMENTATION;

  // Store the result back to memory.
  _mm256_storeu_ps(&y[ 0],y00_07);
}

void test_matvec_8x8_colmaj_8xfloat()
{
  float x[8] = {8,7,6,5, 4,3,2,1 };

  float yt[8] = {0,0,0,0, 0,0,0,0 };
  float yr[8] = {0,0,0,0, 0,0,0,0 };

  float a[64];

  for(int i=0; i < 8; ++i)
      for(int j=0; j < 8; ++j)
	a[i+j*8]= i+j*8 + 1;


  reference_matvec_8x8_colmaj_64xfloat(a,x, yt);
  student_matvec_8x8_colmaj_8xfloat(a, x, yr);


  float res = max_pair_wise_diff(8, yt, yr);

  printf("test_matvec_8x8_colmaj_8xfloat: ");
  if( res > 1e-6 )
    {
      printf("FAIL\n");

      print_float_mem_as_vects(" a", 64,8, a);
      print_float_mem_as_vects(" x", 8,8, x);
      print_float_mem_as_vects("yt", 8,8, yt);
      print_float_mem_as_vects("yr", 8,8, yr);

      printf("\n");
    }
  else
    {
      printf("PASS\n");
    }
}


int main( int argc, char *argv[] )
{

  printf("01: ");test_reverse_8xfloat();
  printf("02: ");test_rotate_by_4_8xfloat();
  printf("03: ");test_rotate_by_2_8xfloat();
  printf("04: ");test_rotate_by_1_8xfloat();

  printf("05: ");test_transpose_4x2_colmaj_8xfloat();
  printf("06: ");test_rotate_by_1_16xfloat();
  printf("07: ");test_transpose_4x4_colmaj_8xfloat();
  printf("08: ");test_transpose_8x4_colmaj_8xfloat();

  printf("09: ");test_gather_at_stride_8xfloat();
  printf("10: ");test_scatter_at_stride_8xfloat();
  printf("11: ");test_matvec_8x8_colmaj_8xfloat();

  return 0;
}



