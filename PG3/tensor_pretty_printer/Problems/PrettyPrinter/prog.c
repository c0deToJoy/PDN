#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    // Check for correct number of command-line arguments
    if (argc != 3) {
        fprintf(stderr, "Error: Should be %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    // Open input file
    FILE *inputFileName = fopen(argv[1], "r");
    if (inputFileName == NULL) {
        fprintf(stderr, "Error: Could not open input file %s\n", argv[1]);
        return 1;
    }

    // Read matrix dimensions and strides
    int m, n, rs, cs, numElements;
    if (fscanf(inputFileName, "%d %d %d %d %d", &m, &n, &rs, &cs, &numElements) != 5) {
        fprintf(stderr, "Error: Failed to read matrix parameters\n");
        fclose(inputFileName);
        return 1;
    }
    printf("Debug: Read parameters: m=%d, n=%d, rs=%d, cs=%d, numElements=%d\n", m, n, rs, cs, numElements);

    // Validate input
    if (m <= 0 || n <= 0 || rs <= 0 || cs <= 0 || numElements <= 0) {
        fprintf(stderr, "Error: Invalid matrix parameters\n");
        fclose(inputFileName);
        return 1;
    }

    // Skip whitespace until the first non-whitespace character
    int c;
    while ((c = fgetc(inputFileName)) != EOF && isspace(c)) {
        printf("Debug: Skipped whitespace: %d ('%c')\n", c, c >= 32 && c <= 126 ? c : '?');
    }
    if (c == EOF) {
        fprintf(stderr, "Error: Unexpected EOF while skipping whitespace\n");
        fclose(inputFileName);
        return 1;
    }
    ungetc(c, inputFileName); // Put back the first non-whitespace character

    // Read characters into memory array
    char *memory = (char *)malloc(numElements * sizeof(char));
    if (memory == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(inputFileName);
        return 1;
    }

    // Read characters
    for (int i = 0; i < numElements; i++) {
        if (fscanf(inputFileName, " %c", &memory[i]) != 1) {
            fprintf(stderr, "Error: Failed to read character %d\n", i);
            free(memory);
            fclose(inputFileName);
            return 1;
        }
    }

    // Close input file
    fclose(inputFileName);

    // Open output file
    FILE *outputFileName = fopen(argv[2], "w");
    if (outputFileName == NULL) {
        fprintf(stderr, "Error: Could not open output file %s\n", argv[2]);
        free(memory);
        return 1;
    }

    printf("Memory contents:\n");
    for (int i = 0; i < numElements; i++) {
        printf("[%d] = '%c' (ascii=%d)\n", i, memory[i], memory[i]);
    }


    // Write header
    fprintf(outputFileName, " m\\n (j-->)\n");

    // Write column indices
    fprintf(outputFileName, "(i) \\ ");
    for (int j = 0; j < n; j++) {
        fprintf(outputFileName, "%02d", j);
        if (j < n - 1) {
            fprintf(outputFileName, "_");
        }
    }
    fprintf(outputFileName, "\n");

    // Write matrix rows
    for (int i = 0; i < m; i++) {
        // Print V for row start
        fprintf(outputFileName, " %c ", i == 0 ? '|' : 'V');
        // Print row index
        fprintf(outputFileName, "%02d|", i);
        // Print elements for the row
        for (int j = 0; j < n; j++) {
            int index = i * rs + j * cs;
            if (index < numElements) {
                fprintf(outputFileName, " %c ", memory[index]);
            } else {
                fprintf(outputFileName, " ? ");
            }
        }
        fprintf(outputFileName, "\n");
    }

    // Clean up
    free(memory);
    fclose(outputFileName);

    return 0;
}