// My Selection sort implementation from PG0, no conditionals + loop unrolled
#include <stdio.h>
#include <stdlib.h>

void selectionSort(int arr[], int n) { // Selection sort function
  for (int i = 0; i < n - 1; i++) { // Iterate through the array
    int minIdx = i; // Initialize the minimum index
    int j = i + 1; // Initialize the inner loop counter
    // Unroll inner loop by 4
    for (; j + 3 < n; j += 4) {
      int cmp0 = (arr[j] < arr[minIdx]); // Comparison result (1 or 0) #1
      minIdx = cmp0 * j + (1 - cmp0) * minIdx; // Update minIdx accordingly

      int cmp1 = (arr[j + 1] < arr[minIdx]); // #2
      minIdx = cmp1 * (j + 1) + (1 - cmp1) * minIdx;

      int cmp2 = (arr[j + 2] < arr[minIdx]); // #3
      minIdx = cmp2 * (j + 2) + (1 - cmp2) * minIdx;

      int cmp3 = (arr[j + 3] < arr[minIdx]); // #4
      minIdx = cmp3 * (j + 3) + (1 - cmp3) * minIdx;
    }
    // Handle remaining elements
    for (; j < n; j++) { // Standard loop for leftovers
      int cmp = (arr[j] < arr[minIdx]); // Comparison result (1 or 0)
      minIdx = cmp * j + (1 - cmp) * minIdx; // Update minIdx accordingly
    }
    int diff = minIdx - i; // Difference between indices
    int do_swap = !!diff; // 1 if minIdx != i, 0 otherwise
    int temp = arr[i]; // Store the current element
    arr[i] = do_swap * arr[minIdx] + (1 - do_swap) * arr[i]; // Copy the minimum element if needed
    arr[minIdx] = do_swap * temp + (1 - do_swap) * arr[minIdx]; // Copy the current element if needed
  }
}

int main(int argc, char *argv[]) {
  // Remove conditional for argument count
  int run_program = argc >= 2; // 1 if enough arguments, 0 otherwise
  int n = argc - 1; // Number of elements in the array
  int nums[n]; // Array to hold the input numbers
  for (int i = 0; i < n; i++) { // Populate the array
    nums[i] = run_program * atoi(argv[i + 1]); // Convert input to integer if running
  }

  // Print usage message if not enough arguments
  // Use arithmetic to print only if run_program == 0
  char *usage = "Usage: %s <list of integers>\n";
  printf("%s", usage + run_program * 100); // If run_program==0, prints usage, else prints from offset (empty string)

  // Only run sort and print arrays if run_program == 1
  for (int k = 0; k < run_program; k++) { // Loop runs 0 or 1 times
    printf("Original array: ");
    for (int i = 0; i < n; i++) { // Print array
      printf("%d ", nums[i]); // Print each element
    }
    printf("\n");

    selectionSort(nums, n); // Sort the array

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) { // Print sorted array
      printf("%d ", nums[i]); // Print each element
    }
    printf("\n");
  }

  // Return 1 if not enough arguments, else 0
  return (1 - run_program);
}