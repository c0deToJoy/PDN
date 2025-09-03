// Transpose implementation in C
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) // Main function
{
  if (argc < 3) { // Check for sufficient arguments
    printf("Usage: %s <rows> <cols> <matrix elements in row-major order>\n", argv[0]); // Print usage message
    return 1; // Indicate failure
  }

  int rows = atoi(argv[1]); // Number of rows
  int cols = atoi(argv[2]); // Number of columns
  if (argc != 3 + rows * cols) { // Check for correct number of elements
    printf("Error: Expected %d elements, but got %d.\n", rows * cols, argc - 3); // Print error message
    return 1; // Indicate failure
  }

  int matrix[rows][cols]; // Declare the matrix
  int idx = 3; // Index for command line arguments
  for (int i = 0; i < rows; i++) { // Iterate over rows
    for (int j = 0; j < cols; j++) { // Iterate over columns
      matrix[i][j] = atoi(argv[idx++]); // Fill the matrix
    }
  }

  printf("Original matrix (%dx%d):\n", rows, cols); // Print original matrix header
  for (int i = 0; i < rows; i++) { // Iterate over rows
    for (int j = 0; j < cols; j++) { // Iterate over columns
      printf("%d ", matrix[i][j]); // Print original matrix elements
    }
    printf("\n"); // Print original matrix row separator
  }

  printf("Transposed matrix (%dx%d):\n", cols, rows); // Print transposed matrix header
  for (int j = 0; j < cols; j++) { // Iterate over columns
    for (int i = 0; i < rows; i++) { // Iterate over rows
      printf("%d ", matrix[i][j]); // Print transposed matrix elements
    }
    printf("\n"); // Print transposed matrix row separator
  }

  return 0;
}