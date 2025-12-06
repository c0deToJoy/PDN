/*
  HW5 A: Shared Memory and Sparse Structures

  Instructions: find all instances of "STUDENT_TODO" and replace with 
                distributed memory code that makes the test corresponding 
                to that function pass.


		To compile and run your code locally use:								      
		  make compile-local 						      
		  make run-local						      
		
 TODO...
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
  NOTE: To run this code you will have at least three options.
  1. Locally: install mpi on your system
     https://rantahar.github.io/introduction-to-mpi/setup.html
  2. Remote on Schooner
  


  Problems:
  0. COO --> CSR
  1. COO --> BCSR
  2. CSR --> CSC
  
  3. Dense matvec 4 ways
  4. CSR matvec 2 ways
  5. BCSR matvec 2 ways


*/


#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <omp.h>

#include "helper.h"
#include "sparse.h"



#define ERROR_THRESHOLD (1e-4)



/*
  SPARSE FORMAT CONVERSION
*/


////////////////
////// PROB01 //
////////////////
void student_coo_matrix_to_csr_matrix( coo_matrix_t *coo_src,
 				          csr_matrix_t *csr_dst )
{
  // Copy over the metadata from the source to the dest
  csr_dst->m   = coo_src->m;
  csr_dst->n   = coo_src->n;
  csr_dst->nnz = coo_src->nnz;

  // Initialize the buffers
  // For the sake of the HW we will fill the unitialized values with zeros
  csr_dst->row_idx = (int *)calloc((csr_dst->m+1),sizeof(int));
  csr_dst->col_idx = (int *)calloc(csr_dst->nnz,sizeof(int));
  csr_dst->values  = (float *)calloc(csr_dst->nnz,sizeof(float));
  
  {
    // STUDENT_TODO: Do not convert to a dense matrix then back to csr.

    // Most important hint you will receive: draw this out on paper first.
    //
    // Minor Hint: take one pass to figure out the row_idx values
    //       then take a second pass to place the col_idx and values
    //       in the right location. You will need to keep an extra set of
    //       counters for each row.

    // PASS 1: figure out the number of non-zeros in each row
    for( int nnz_idx = 0; nnz_idx < coo_src->nnz; ++nnz_idx ) // Loop through all non-zeros
    {
        int row = coo_src->row_idx[nnz_idx]; // Get the row index of the current non-zero
        csr_dst->row_idx[row+1] += 1; // Increment the count of non-zeros in that row
    }
    // Cumulative sum to get the row_ptr
    for( int row = 0; row < csr_dst->m; ++row ) // Loop through each row
    {
      csr_dst->row_idx[row+1] += csr_dst->row_idx[row]; // Update to cumulative sum
    }
    // PASS 2: place the col_idx and values in the right location
    int *current_pos = (int *)calloc(csr_dst->m, sizeof(int)); // Array to track current position in each row
    for( int nnz_idx = 0; nnz_idx < coo_src->nnz; ++nnz_idx ) // Loop through all non-zeros again
    {
        int row = coo_src->row_idx[nnz_idx]; // Get the row index of the current non-zero
        int next_pos = csr_dst->row_idx[row] + current_pos[row]; // Calculate the position to insert
        csr_dst->col_idx[next_pos] = coo_src->col_idx[nnz_idx]; // Set the column index
        csr_dst->values[next_pos]  = coo_src->values[nnz_idx]; // Set the value
        current_pos[row] += 1; // Increment the current position for that row
    }
    free(current_pos); // Free the temporary position array
  }
}


void test_coo_to_csr( coo_matrix_t *coo_src )
{

  /*
    Initialize stuff.    
  */
  

  dense_matrix_t *dense_ref = create_dense_matrix_from_coo_matrix(coo_src);  
  csr_matrix_t *csr_ref  = create_csr_matrix_from_dense_matrix(dense_ref);
  
  /*
    Student Implementation
  */
  csr_matrix_t *csr_test = (csr_matrix_t *)malloc(sizeof(csr_matrix_t));
  student_coo_matrix_to_csr_matrix( coo_src,
				    csr_test );

  /*
    Compare
  */

  dense_matrix_t *dense_test = create_dense_matrix_from_csr_matrix(csr_test);
  
  float res = max_pair_wise_diff(coo_src->m*coo_src->n,
				 dense_ref->values,
				 dense_test->values);

  
  printf("COO to CSR: ");
  
  if( ERROR_THRESHOLD < res )
    {
      printf("FAIL with error = %f\n",res);
      
      printf("======================\n");
      printf("Original Data Stored in Coordinate (COO) Format\n");
      pretty_print_coo_matrix(coo_src);

      printf("======================\n");
      printf("Reference Data Stored in Compressed Sparse Row (CSR) Format\n");
      pretty_print_csr_matrix(csr_ref);

      printf("======================\n");
      printf("Student Data Stored in Compressed Sparse Row (CSR) Format\n");
      pretty_print_csr_matrix(csr_test);
      
    }
  else
    {
      printf("PASS\n");
    }
  
  
  destroy_dense_matrix( dense_test );
  destroy_dense_matrix( dense_ref );
  destroy_csr_matrix(csr_ref);
  destroy_csr_matrix(csr_test);
}

////////////////
////// PROB02 //
////////////////
void student_coo_matrix_to_bcsr_matrix( int mb, int nb,
					    int bcs, int brs,
					    coo_matrix_t *coo_src,
					    bcsr_matrix_t *bcsr_dst )
{
  // Copy over the metadata from the source to the dest
  bcsr_dst->m   = coo_src->m;
  bcsr_dst->n   = coo_src->n;

  bcsr_dst->mb  = mb;
  bcsr_dst->nb  = nb;
  bcsr_dst->bcs = bcs;
  bcsr_dst->brs = brs;

  
  {
    // STUDENT_TODO: Do not convert to a dense matrix then back to bcsr.

    // Most important hint you will receive: draw this out on paper first.
    //
    // Minor Hint: Do this in three passes of the coo data. It's not very
    //             efficient, but it's a starting point that will work.
    //             PASS 1: figure out the number of non-zero blocks of
    //                     size MB x NB.
    //
    //             PASS 2: determine the values for block_row_idx
    //
    //             PASS 3: place the appropriate values in block_col_idx
    //                     and block_values. You will need to keep a counter
    //                     for each row block.
    
    // PASS 1: Count the number of non-zero blocks
    int num_block_rows = (bcsr_dst->m) / mb; // Initialize number of block rows to number of rows divided by block size
    int num_block_cols = (bcsr_dst->n) / nb; // Initialize number of block cols to number of cols divided by block size
    
    int *block_has_nnz = (int *)calloc(num_block_rows * num_block_cols, sizeof(int)); // Track non-zero blocks
    
    for( int nnz_idx = 0; nnz_idx < coo_src->nnz; ++nnz_idx ) // Loop through all non-zeros
    {
        int row = coo_src->row_idx[nnz_idx]; // Get the row index of the current non-zero
        int col = coo_src->col_idx[nnz_idx]; // Get the column index of the current non-zero
        
        int block_row = row / mb; // Determine the block row
        int block_col = col / nb; // Determine the block column
        int block_id = block_row * num_block_cols + block_col; // Calculate the block ID
        
        block_has_nnz[block_id] = 1; // Mark this block as having a non-zero
    }
    
    int nnz_blocks = 0; // Initialize non-zero block count
    for( int i = 0; i < num_block_rows * num_block_cols; ++i ) // Loop through all blocks
    {
        if( block_has_nnz[i] ) // If the block has a non-zero
            nnz_blocks++; // Increment the non-zero block count
    }
    
    bcsr_dst->nnz_blocks = nnz_blocks; // Set the total number of non-zero blocks!!
    
    // Initialize the buffers
    // For the sake of the HW we will fill the unitialized values with zeros
    bcsr_dst->block_row_idx = (int *)calloc(((bcsr_dst->m)/mb+1),
					    sizeof(int));
    bcsr_dst->block_col_idx = (int *)calloc(bcsr_dst->nnz_blocks,
					    sizeof(int));
    bcsr_dst->block_values  = (float *)calloc(bcsr_dst->nnz_blocks*(mb*nb),
					      sizeof(float));

    // PASS 2: find out how many non-zero blocks are in each block row and modify
    //         bcsr_dst->block_row_idx
    int *blocks_per_row = (int *)calloc(num_block_rows, sizeof(int)); // Track number of blocks per row
    
    for( int block_row = 0; block_row < num_block_rows; ++block_row ) // Loop through all block rows
    {
        for( int block_col = 0; block_col < num_block_cols; ++block_col ) // Loop through all block columns
        {
            int block_id = block_row * num_block_cols + block_col; // Calculate the block ID
            if( block_has_nnz[block_id] ) // If the block has a non-zero
                blocks_per_row[block_row]++; // Increment the count of blocks in that row
        }
    }
    
    bcsr_dst->block_row_idx[0] = 0; // First entry is always 0
    for( int block_row = 0; block_row < num_block_rows; ++block_row ) // Loop through all block rows
    {
        bcsr_dst->block_row_idx[block_row+1] = bcsr_dst->block_row_idx[block_row] + blocks_per_row[block_row]; // Cumulative sum of blocks per row
    }

    // PASS 3: Place the coo values into their right location in the bcsr matrix
    int *current_block_count = (int *)calloc(num_block_rows, sizeof(int)); // Track current block count per row
    
    for( int block_row = 0; block_row < num_block_rows; ++block_row ) // Loop through all block rows
    {
        for( int block_col = 0; block_col < num_block_cols; ++block_col ) // Loop through all block columns
        {
            int block_id = block_row * num_block_cols + block_col; // Calculate the block ID
            if( block_has_nnz[block_id] ) // If the block has a non-zero
            {
                int block_idx = bcsr_dst->block_row_idx[block_row] + current_block_count[block_row]; // Calculate the block index
                bcsr_dst->block_col_idx[block_idx] = block_col * nb; // Set the block column index
                current_block_count[block_row]++; // Increment the current block count for that row
            }
        }
    }
    
    for( int nnz_idx = 0; nnz_idx < coo_src->nnz; ++nnz_idx ) // Loop through all non-zeros
    {
        int row = coo_src->row_idx[nnz_idx]; // Get the row index of the current non-zero
        int col = coo_src->col_idx[nnz_idx]; // Get the column index of the current non-zero
        float val = coo_src->values[nnz_idx]; // Get the value of the current non-zero
        
        int block_row = row / mb; // Calculate the block row
        int block_col = col / nb; // Calculate the block column
        int in_block_row = row % mb; // Calculate the in-block row
        int in_block_col = col % nb; // Calculate the in-block column
        
        int block_start = bcsr_dst->block_row_idx[block_row]; // Start index of the block row
        int block_end = bcsr_dst->block_row_idx[block_row+1]; // End index of the block row
        int block_pos = -1; // Initialize block position
        
        for( int i = block_start; i < block_end; ++i ) // Loop through the blocks in the block row
        {
            if( bcsr_dst->block_col_idx[i] == block_col * nb ) // If the block column matches
            {
                block_pos = i; // Set the block position
                break;
            }
        }
        
        if( block_pos >= 0 ) // If a valid block position was found
        {
            int block_idx = block_pos * mb * nb; // Calculate the starting index of the block in the values array
            bcsr_dst->block_values[block_idx + in_block_row * bcs + in_block_col * brs] = val; // Set the value in the block
        }
    }
    free(block_has_nnz); // Free the memory allocated for block_has_nnz
    free(blocks_per_row); // Free the memory allocated for blocks_per_row
    free(current_block_count); // Free the memory allocated for current_block_count
  }
}


void test_coo_to_bcsr( coo_matrix_t *coo_src )
{

  /*
    Initialize stuff.    
  */

  int mb  = 3; // blocksize number of rows
  int nb  = 2; // blocksize number of cols
  int bcs = 2; // column stride in the block
  int brs = 1; // row stride in the block
  

  dense_matrix_t *dense_ref = create_dense_matrix_from_coo_matrix(coo_src);  
  bcsr_matrix_t  *bcsr_ref  = create_bcsr_matrix_from_dense_matrix(mb,nb,
								   bcs,brs,
								   dense_ref);
  
  /*
    Student Implementation
  */
  bcsr_matrix_t *bcsr_test = (bcsr_matrix_t *)malloc(sizeof(bcsr_matrix_t));
  student_coo_matrix_to_bcsr_matrix( mb,nb,
				     bcs,brs,
				     coo_src,
				     bcsr_test );

  /*
    Compare
  */

  dense_matrix_t *dense_test = create_dense_matrix_from_bcsr_matrix(bcsr_test);
  
  float res = max_pair_wise_diff(coo_src->m*coo_src->n,
				 dense_ref->values,
				 dense_test->values);

  
  printf("COO to BCSR: ");
  
  if( ERROR_THRESHOLD < res )
    {
      printf("FAIL with error = %f\n",res);
      
      printf("======================\n");
      printf("Original Data Stored in Coordinate (COO) Format\n");
      pretty_print_coo_matrix(coo_src);

      printf("======================\n");
      printf("Reference Data Stored in Blocked Compressed Sparse Row (BCSR) Format\n");
      pretty_print_bcsr_matrix(bcsr_ref);

      printf("======================\n");
      printf("Student Data Stored in Blocked Compressed Sparse Row (BCSR) Format\n");
      pretty_print_bcsr_matrix(bcsr_test);
      
    }
  else
    {
      printf("PASS\n");
    }
  
  
  destroy_dense_matrix( dense_test );
  destroy_dense_matrix( dense_ref );
  destroy_bcsr_matrix(bcsr_ref);
  destroy_bcsr_matrix(bcsr_test);
}

////////////////
////// PROB03 //
////////////////
void student_csr_matrix_to_csc_matrix( csr_matrix_t *csr_src,
 				          csc_matrix_t *csc_dst )
{
  // Copy over the metadata from the source to the dest
  csc_dst->m   = csr_src->m;
  csc_dst->n   = csr_src->n;
  csc_dst->nnz = csr_src->nnz;

  // Initialize the buffers
  // For the sake of the HW we will fill the unitialized values with zeros
  csc_dst->col_idx = (int *)calloc((csc_dst->n+1),sizeof(int));
  csc_dst->row_idx = (int *)calloc(csc_dst->nnz,sizeof(int));
  csc_dst->values  = (float *)calloc(csc_dst->nnz,sizeof(float));
  
  {
    
    // STUDENT_TODO: Do not convert to a dense matrix then back to csr.

    // Most important hint you will receive: draw this out on paper first.

    // PASS 1: figure out the number of non-zeros in each column
    for( int nnz_idx = 0; nnz_idx < csr_src->nnz; ++nnz_idx ) // Loop through all non-zeros
    {
        int col = csr_src->col_idx[nnz_idx]; // Get the column index of the current non-zero
        csc_dst->col_idx[col+1] += 1; // Increment the count of non-zeros in that column
    }
    // Cumulative sum to get the col_ptr
    for( int col = 0; col < csc_dst->n; ++col ) // Loop through each column
    {
      csc_dst->col_idx[col+1] += csc_dst->col_idx[col]; // Update to cumulative sum
    }

    // PASS 2: place the row_idx and values in the right location
    int *current_pos = (int *)calloc(csc_dst->n, sizeof(int)); // Array to track current position in each column
    for( int row = 0; row < csr_src->m; ++row ) // Loop through each row
    {
        for( int idx = csr_src->row_idx[row]; idx < csr_src->row_idx[row+1]; ++idx ) // Loop through non-zeros in that row
        {
            int col = csr_src->col_idx[idx]; // Get the column index of the current non-zero
            int next_pos = csc_dst->col_idx[col] + current_pos[col]; // Calculate the position to insert
            csc_dst->row_idx[next_pos] = row; // Set the row index
            csc_dst->values[next_pos]  = csr_src->values[idx]; // Set the value
            current_pos[col] += 1; // Increment the current position for that column
        }
    }
    free(current_pos); // Free the temporary position array
  }
}


void test_csr_to_csc( csr_matrix_t *csr_src )
{

  /*
    Initialize stuff.    
  */
  

  dense_matrix_t *dense_ref = create_dense_matrix_from_csr_matrix(csr_src);  
  csc_matrix_t *csc_ref  = create_csc_matrix_from_dense_matrix(dense_ref);
  
  /*
    Student Implementation
  */
  csc_matrix_t *csc_test = (csc_matrix_t *)malloc(sizeof(csc_matrix_t));
  student_csr_matrix_to_csc_matrix( csr_src,
				    csc_test );

  /*
    Compare
  */

  dense_matrix_t *dense_test = create_dense_matrix_from_csc_matrix(csc_test);
  
  float res = max_pair_wise_diff(csr_src->m*csr_src->n,
				 dense_ref->values,
				 dense_test->values);

  
  printf("CSR to CSC: ");
  
  if( ERROR_THRESHOLD < res )
    {
      printf("FAIL with error = %f\n",res);
      
      printf("======================\n");
      printf("Original Data Stored in Coordinate (CSR) Format\n");
      pretty_print_csr_matrix(csr_src);

      printf("======================\n");
      printf("Reference Data Stored in Compressed Sparse Column (CSC) Format\n");
      pretty_print_csc_matrix(csc_ref);

      printf("======================\n");
      printf("Student Data Stored in Compressed Sparse Row (CSC) Format\n");
      pretty_print_csc_matrix(csc_test);
      
    }
  else
    {
      printf("PASS\n");
    }
  
  
  destroy_dense_matrix( dense_test );
  destroy_dense_matrix( dense_ref );
  destroy_csc_matrix(csc_ref);
  destroy_csc_matrix(csc_test);
}



int main( int argc, char **argv )
{
  /*
    Sparse Conversions.
  */
  float A_buf[] = {1,0,0,2,
		   0,3,4,0,
		   5,0,6,0,
		   0,0,0,7,
		   0,8,0,9,
		   10,11,0,0
  };

  dense_matrix_t *A_dense = create_dense_matrix_and_attach_existing_array(6,4,
								          4,1,
									  A_buf);

  // Print out the dense matrix
  printf("======================\n");
  printf("Original Dense Data\n");
  pretty_print_dense_matrix( A_dense );


  // Create a coo matrix
  coo_matrix_t *A_coo = create_coo_matrix_from_dense_matrix(A_dense);


  //////////////////
  printf("PROB01 -- "); test_coo_to_csr( A_coo ); printf("\n");
  printf("PROB02 -- "); test_coo_to_bcsr( A_coo ); printf("\n");

  // Free up that coo matrix
  destroy_coo_matrix(A_coo);


  csr_matrix_t *A_csr = create_csr_matrix_from_dense_matrix(A_dense);  
  printf("PROB03 -- "); test_csr_to_csc( A_csr ); printf("\n");
  

  // Free up that csr matrix
  destroy_csr_matrix(A_csr);


  // Free up the original dense matrix
  destroy_dense_matrix_and_detach_existing_array( A_dense );



  
  return 0;
}
