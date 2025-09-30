#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Make sure there are 3 arguments
    if (argc != 3) { // If not correct number of args
        fprintf(stderr, "Error: Should be %s <input_file> <output_file>\n", argv[0]); // Print error & return
        return 1;
    }

    // Open input
    FILE *inputFileName = fopen(argv[1], "r"); // Get input file name from args
    if (inputFileName == NULL) { // If input file is null
        fprintf(stderr, "Error: Could not open input file %s\n", argv[1]); // Print error & return
        return 1;
    }

    // Read matrix dimensions and strides
    int m, n, rs, cs, numElements; // Initialize number of rows, columns, row stride, column stride, and number of elements
    if (fscanf(inputFileName, "%d %d %d %d %d", &m, &n, &rs, &cs, &numElements) != 5) { // If there are not 5 integers
        fprintf(stderr, "Error: Failed to read matrix parameters\n"); // Print error & return
        fclose(inputFileName);
        return 1;
    }

    // Validate input
    if (m <= 0 || n <= 0 || rs <= 0 || cs <= 0 || numElements <= 0) { // If any parameters are non-positive
        fprintf(stderr, "Error: Invalid matrix parameters\n"); // Print error, close input file, & return
        fclose(inputFileName);
        return 1;
    }

    // Read characters into memory array
    char *memory = (char *)malloc(numElements * sizeof(char)); // Allocate memory for the array
    if (memory == NULL) { // If memory allocation fails
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(inputFileName);
        return 1;
    }

    // Skip newline after numElements
    fgetc(inputFileName); // Read and discard newline character
    for (int i = 0; i < numElements; i++) { // For each element
        memory[i] = fgetc(inputFileName); // Read character into memory array
        fgetc(inputFileName); // Read and discard newline character
    }

    // Close input file
    fclose(inputFileName);

    // Open output file
    FILE *outputFileName = fopen(argv[2], "w"); // Get output file name from args
    if (outputFileName == NULL) { // If output file is null
        fprintf(stderr, "Error: Could not open output file %s\n", argv[2]); // Print error, free memory, & return
        free(memory);
        return 1;
    }

    // Write matrix parameters
    fprintf(outputFileName, "M: %d\n", m); // Write number of rows
    fprintf(outputFileName, "N: %d\n", n); // Write number of columns
    fprintf(outputFileName, "RS: %d\n", rs); // Write row stride
    fprintf(outputFileName, "CS: %d\n", cs); // Write column stride

    // Write memory array
    fprintf(outputFileName, "MEMORY[%d] = {", numElements); // Open memory array
    for (int i = 0; i < numElements; i++) { // For each element
        fprintf(outputFileName, "'%c'", memory[i]); // Write character
        if (i < numElements - 1) { // If not the last element
            fprintf(outputFileName, ","); // Write comma
        }
    }
    fprintf(outputFileName, "}\n\n"); // Close memory array

    // Write row starts with dynamic spacing
    fprintf(outputFileName, "ROW_STARTS: ");  // Write row starts label
    int elementWidth = 4; // Width of each memory element
    for (int i = 0; i < numElements; i++) { // For each element
        int rowStart = 0; // Flag to indicate if it's a row start
        for (int row = 0; row < m; row++) { // For each row
            if (i == row * rs) { // Check if index i is a row start
                rowStart = 1; // Set flag & break
                break;
            }
        }
        if (rowStart) { // If it's a row start
            fprintf(outputFileName, "V"); // Write pointer 'V'
            for (int s = 0; s < elementWidth - 1; s++) { // Add spaces for alignment
                fprintf(outputFileName, " ");
            }
        } else { // If not a row start
            for (int s = 0; s < elementWidth; s++) { // Add spaces for alignment
                fprintf(outputFileName, " ");
            }
        }
    }
    fprintf(outputFileName, "\n"); //  New line after row starts

    // Write memory array with consistent spacing
    fprintf(outputFileName, "MEMORY:    "); // Write memory label with padding
    for (int i = 0; i < numElements; i++) { // For each element
        fprintf(outputFileName, " %c,", memory[i]); // Write character with leading space
        if (i < numElements - 1) { // If not the last element
            fprintf(outputFileName, " "); // Write space
        }
    }
    fprintf(outputFileName, "\n"); // New line after memory array

    // Write column starts with dynamic spacing
    fprintf(outputFileName, "COL_STARTS: "); // Write column starts label
    for (int i = 0; i < numElements; i++) { // For each element
        int colStart = 0; // Flag to indicate if it's a column start
        for (int col = 0; col < n; col++) { // For each column
            if (i == col * cs) { // Check if index i is a column start
                colStart = 1; // Set flag & break
                break;
            }
        }
        if (colStart) { // If it's a column start
            fprintf(outputFileName, "^"); // Write pointer '^'
            for (int s = 0; s < elementWidth - 1; s++) { // Add spaces for alignment
                fprintf(outputFileName, " ");
            }
        } else { // If not a column start
            for (int s = 0; s < elementWidth; s++) { // Add spaces for alignment
                fprintf(outputFileName, " ");
            }
        }
    }
    fprintf(outputFileName, "\n"); // New line after column starts

    // Clean
    free(memory); // Free allocated memory
    fclose(outputFileName); // Close output file

    return 0;
}