#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    // Make sure there are 3 arguments
    if (argc != 3) { // If not correct number of args
        fprintf(stderr, "Error: Should be %s <input_file> <output_file>\n", argv[0]); // Print error & return
        return 1;
    }

    // Open input file
    FILE *inputFileName = fopen(argv[1], "r"); // Get input file name from args
    if (inputFileName == NULL) { // If input file is null
        fprintf(stderr, "Error: Could not open input file %s\n", argv[1]); // Print error & return
        return 1;
    }

    // Read matrix parameters
    int m, n, rs, cs, mb, nb, iStart, jStart, numElements; // Initialize number of rows, columns, row stride, column stride, block rows, block columns, start row, start column, and number of elements
    if (fscanf(inputFileName, "%d %d %d %d %d %d %d %d %d", &m, &n, &rs, &cs, &mb, &nb, &iStart, &jStart, &numElements) != 9) { // If there are not 9 integers
        fprintf(stderr, "Error: Failed to read matrix parameters\n"); // Print error & return
        fclose(inputFileName);
        return 1;
    }

    // Validate input
    if (m <= 0 || n <= 0 || rs <= 0 || cs <= 0 || mb <= 0 || nb <= 0 || iStart < 0 || jStart < 0 || numElements <= 0) { // If any parameters are non-positive
        fprintf(stderr, "Error: Invalid matrix parameters\n"); // Print error, close input file, & return
        fclose(inputFileName);
        return 1;
    }

    // Skip whitespace until the first non-whitespace character
    int c; // Character variable
    while ((c = fgetc(inputFileName)) != EOF && isspace(c)) {  // While not EOF and is whitespace
        // Just skip
    }
    if (c == EOF) { // If EOF reached without finding a non-whitespace character
        fprintf(stderr, "Error: Unexpected EOF while skipping whitespace\n"); // Print error, close input file, & return
        fclose(inputFileName);
        return 1;
    }
    ungetc(c, inputFileName); // Put back the first non-whitespace character

    // Read characters into memory array
    char *memory = (char *)malloc(numElements * sizeof(char)); // Allocate memory for the array
    if (memory == NULL) { // If memory allocation fails
        fprintf(stderr, "Error: Memory allocation failed\n"); // Print error, close input file, & return
        fclose(inputFileName);
        return 1;
    }

    // Read characters
    for (int i = 0; i < numElements; i++) { // For each element
        if (fscanf(inputFileName, " %c", &memory[i]) != 1) { // Read character into memory array, skipping whitespace
            fprintf(stderr, "Error: Failed to read character %d\n", i); // Print error, free memory, close input file, & return
            free(memory);
            fclose(inputFileName);
            return 1;
        }
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

    // Write header
    fprintf(outputFileName, "mb\\nb (j-->)\n");

    // Write column indices
    fprintf(outputFileName, "(i) \\ "); // Row index label
    for (int j = jStart; j < jStart + nb && j < n; j++) { // For each column in the block
        fprintf(outputFileName, "%02d", j - jStart); // Print column index relative to block start
        if (j < jStart + nb - 1 && j < n - 1) { // If not the last column in the block
            fprintf(outputFileName, "_"); // Print underscore
        }
    }
    fprintf(outputFileName, "\n"); // New line after column indices

    // Write matrix rows for the specified block
    for (int i = iStart; i < iStart + mb && i < m; i++) { // For each row in the block
        fprintf(outputFileName, "%c ", i == iStart ? '|' : 'V'); // Print row indicator with single space after
        fprintf(outputFileName, "%02d|", i - iStart); // Print row index relative to block start
        for (int j = jStart; j < jStart + nb && j < n; j++) { // For each column in the block
            int index = i * rs + j * cs; // Calculate linear index
            if (index < numElements) { // If index is within bounds
                fprintf(outputFileName, " %c ", memory[index]); // Print character with spaces
            } else {
                fprintf(outputFileName, " ? "); // Print placeholder
            }
        }
        fprintf(outputFileName, "\n"); // New line after each row
    }

    // Clean
    free(memory); // Free allocated memory
    fclose(outputFileName); // Close output file

    return 0;
}