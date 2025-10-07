/*
  This program provides a set of avx/av2 SIMD examples for x86. Search for the
  instructions online and modify the STUDENT_TODO/NOTES.

  
  - richard.m.veras@ou.edu


  Examples:


  // Load
  __m256 _mm256_loadu_ps (float const * mem_addr)

  // Store
  void _mm256_storeu_ps (float * mem_addr, __m256 a)

  // set
  __m256 _mm256_set1_ps (float a)

  // permute with one input
  __m256 _mm256_permute_ps (__m256 a, int imm8) // w/in lane
  __m256 _mm256_permutevar8x32_ps (__m256 a, __m256i idx) // across lane


  // Blend
  __m256 _mm256_blend_ps (__m256 a, __m256 b, const int imm8)


  // Shuffle with two inputs
  __m256 _mm256_shuffle_ps (__m256 a, __m256 b, const int imm8)


  Note: There are gather and scatter instructions for sparse
  loads and stores. It is tempting to use these instructions
  for rearranging data, but they are complete garbage and
  should be avoided like the plague. My only theory as to
  why they exist is that someone from marketing thought it
  was a great idea, and the engineers tasked their interns
  with the most.... potential.... to implement them.

*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <immintrin.h>

// NOTE: This include is only needed for osaca
#include "instruments.h"

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

void print_8xfloat_vect(char *name, __m256 src)
{
  float buff[8];
  _mm256_storeu_ps(buff,src);
  
  print_float_mem(name, 8, buff);
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


void loadu_example()
{
  printf("load example\n");
  float a[] = {0,1,2,3,4,5,6,7};

  print_float_mem(" a", 8, a);

  // This {BEGIN,END}_INSTRUMENTATION marks off a block to be simulated with 'make measure-osaca'
  // Only one block can be measured at a time, so comment out all the others.
  //BEGIN_INSTRUMENTATION; // Mark the start of the code for osaca/mca  
  /////////////////////////
  // STUDENT_TODO: Note that we are using loadu/storeu instead of load/store which assume aligned data.
  //               Aligned data is 'aligned' to 64B boundaries, i.e. addr%64 == 0
  //               If you need aligned data, you can use the special malloc, memalign.
  __m256 avect = _mm256_loadu_ps(&a[0]);
  /////////////////////////
  //END_INSTRUMENTATION; // 
  
  print_8xfloat_vect("av",avect);
  printf("\n");
}


void storeu_example()
{
  printf("store example\n");
  
  float a[8] = {0,1,2,3,4,5,6,7};

  print_float_mem(" a", 8, a);

  __m256 avect = _mm256_loadu_ps(&a[0]);
  print_8xfloat_vect("av",avect);
  
  float b[8];
  
  //BEGIN_INSTRUMENTATION; // STUDENT_TODO: Uncomment this instrumentation block and comment out the others.
  /////////////////////////
  _mm256_storeu_ps(b,avect); // STUDENT_TODO: Note that we are using loadu/storeu instead of load/store which assume aligned
  /////////////////////////
  //END_INSTRUMENTATION;
  
  print_float_mem(" b", 8, b);
  printf("\n");
  
}


void set_example()
{
  printf("set example\n");

  //BEGIN_INSTRUMENTATION; // STUDENT_TODO: Uncomment this instrumentation block and comment out the others.
  /////////////////////////
  // STUDENT_TODO: Sometime this intrinsic is broken up into many instructions.
  //               in special cases, like when the value is '0', then this is
  //               replaced with an xor.   0 == a xor a
  __m256 avect = _mm256_set1_ps(3.0);
  /////////////////////////
  //END_INSTRUMENTATION;
  
  print_8xfloat_vect("av",avect);
  printf("\n");
}

void extract_example()
{
  printf("extract example\n");
  float a[] = {11,22,33,44,55,66,77,88};
  float b[8] ={-1,-1,-1,-1,-1,-1,-1,-1};

  //BEGIN_INSTRUMENTATION; // STUDENT_TODO: Uncomment this instrumentation block and comment out the others.
  /////////////////////////
  __m256 avect = _mm256_loadu_ps(&a[0]);
    
  __m256i index = _mm256_set_epi32(1,3,2,4,6,5,7,1); // STUDENT_TODO: Play with these values [1,7]
  __m256 bvect = _mm256_permutevar8x32_ps(avect,index); // STUDENT_TODO: This is an expensive instruction.


  // extract the bottom element out of the vector
  b[0] =  _mm256_cvtss_f32(bvect); // STUDENT_TODO: This is also expensive.
  /////////////////////////
  //END_INSTRUMENTATION;

  print_8xfloat_vect("av",avect);
  print_8xfloat_vect("bv",bvect);
  print_float_mem(" b",8,b);
  printf("\n");


  //   __m128 _mm256_extractf128_ps (__m256 a, const int imm8) // Also need __m128 version
  //    int _mm_extract_ps (__m128 a, const int imm8) // SSE 4.1


  
  
}

void permute_example()
{
  printf("permute example\n");
  float a[] = {0,1,2,3,4,5,6,7};

  //BEGIN_INSTRUMENTATION; // STUDENT_TODO: Uncomment this instrumentation block and comment out the others.
  /////////////////////////
  __m256 avect = _mm256_loadu_ps(&a[0]);
  int imm = 0b00011011; // STUDENT_TODO: Play with this
    
  __m256 bvect = _mm256_permute_ps(avect,imm); // STUDENT_TODO: This is a cheap instruction
  /////////////////////////
  //END_INSTRUMENTATION;
  
  print_8xfloat_vect("av",avect);
  print_8xfloat_vect("bv",bvect);
  printf("\n");

}

void permute_across_lanes()
{
  printf("permute across lanes example\n");
  float a[] = {0,1,2,3,4,5,6,7};

  //BEGIN_INSTRUMENTATION; // STUDENT_TODO: Uncomment this instrumentation block and comment out the others.
  /////////////////////////
  __m256 avect = _mm256_loadu_ps(&a[0]);
  __m256i index = _mm256_set_epi32(1,3,2,4,6,5,7,0); // STUDENT_TODO: Play with this
  __m256 bvect = _mm256_permutevar8x32_ps(avect,index);
  /////////////////////////
  //END_INSTRUMENTATION;
  
  print_8xfloat_vect("av",avect);
  print_8xfloat_vect("bv",bvect);
  printf("\n");

}

void blend_example()
{
  printf("blend example\n");
  float a[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  //BEGIN_INSTRUMENTATION; // STUDENT_TODO: Uncomment this instrumentation block and comment out the others.
  /////////////////////////
  __m256 a0vect = _mm256_loadu_ps(&a[0]);
  __m256 a1vect = _mm256_loadu_ps(&a[8]);
  int imm = 22; // STUDENT_TODO: Play with this! This is fun. Also, it's useful to use C binary notation. 'int imm = 0b11100001
  __m256 bvect = _mm256_blend_ps(a0vect,a1vect,imm); 
  /////////////////////////
  //END_INSTRUMENTATION;
  
  print_8xfloat_vect("a0",a0vect);
  print_8xfloat_vect("a1",a1vect);
  print_8xfloat_vect("bv",bvect);
  printf("\n");

}

void shuffle_example()
{
  printf("shuffle example\n");
  float a[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  //BEGIN_INSTRUMENTATION; // STUDENT_TODO: Uncomment this instrumentation block and comment out the others.
  /////////////////////////
  __m256 a0vect = _mm256_loadu_ps(&a[0]);
  __m256 a1vect = _mm256_loadu_ps(&a[8]);
  int imm = 22; // STUDENT_TODO: Play with this
  __m256 bvect = _mm256_shuffle_ps(a0vect,a1vect,imm);
  /////////////////////////
  //END_INSTRUMENTATION;
  
  print_8xfloat_vect("a0",a0vect);
  print_8xfloat_vect("a1",a1vect);
  print_8xfloat_vect("bv",bvect);
  printf("\n");

}


int main( int argc, char *argv[] )
{

  loadu_example();

  storeu_example();

  set_example();
  
  permute_example();

  permute_across_lanes();  

  blend_example();

  shuffle_example();
    
  extract_example();

  return 0;
}



