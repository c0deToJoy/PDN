// My Selection sort implementation from PG0, no conditionals
#include <stdio.h>
#include <stdlib.h>

void selectionSort(int arr[], int n) {
  for (int i = 0; i < n - 1; i++) {
    int minIdx = i;
    int j = i + 1;
    // Unroll inner loop by 4
    for (; j + 3 < n; j += 4) {
      int cmp0 = (arr[j] < arr[minIdx]);
      minIdx = cmp0 * j + (1 - cmp0) * minIdx;

      int cmp1 = (arr[j + 1] < arr[minIdx]);
      minIdx = cmp1 * (j + 1) + (1 - cmp1) * minIdx;

      int cmp2 = (arr[j + 2] < arr[minIdx]);
      minIdx = cmp2 * (j + 2) + (1 - cmp2) * minIdx;

      int cmp3 = (arr[j + 3] < arr[minIdx]);
      minIdx = cmp3 * (j + 3) + (1 - cmp3) * minIdx;
    }
    // Handle remaining elements
    for (; j < n; j++) {
      int cmp = (arr[j] < arr[minIdx]);
      minIdx = cmp * j + (1 - cmp) * minIdx;
    }
    // Swap using arithmetic, only if minIdx != i
    int diff = minIdx - i;
    int do_swap = !!diff; // 1 if minIdx != i, 0 otherwise
    int temp = arr[i];
    arr[i] = do_swap * arr[minIdx] + (1 - do_swap) * arr[i];
    arr[minIdx] = do_swap * temp + (1 - do_swap) * arr[minIdx];
  }
}

int main(int argc, char *argv[]) {
  // Remove conditional for argument count
  int run_program = argc >= 2;
  int n = argc - 1;
  int nums[n];
  for (int i = 0; i < n; i++) {
    nums[i] = run_program * atoi(argv[i + 1]);
  }

  // Print usage message if not enough arguments
  // Use arithmetic to print only if run_program == 0
  char *usage = "Usage: %s <list of integers>\n";
  printf("%s", usage + run_program * 100); // If run_program==0, prints usage, else prints from offset (empty string)

  // Only run sort and print arrays if run_program == 1
  for (int k = 0; k < run_program; k++) {
    printf("Original array: ");
    for (int i = 0; i < n; i++) {
      printf("%d ", nums[i]);
    }
    printf("\n");

    selectionSort(nums, n);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
      printf("%d ", nums[i]);
    }
    printf("\n");
  }

  // Return 1 if not enough arguments, else 0
  return (1 - run_program);
}