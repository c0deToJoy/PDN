// Bubble sort implementation in C
#include <stdio.h>

void bubbleSort(int nums[], int n) { // Bubble sort function
  for (int i = 0; i < n - 1; i++) { // Outer loop
    for (int j = 0; j < n - i - 1; j++) { // Inner loop
      if (nums[j] > nums[j + 1]) { // Compare adjacent elements
        int temp = nums[j]; // Swap elements
        nums[j] = nums[j + 1]; // Move larger element down
        nums[j + 1] = temp; // Move smaller element up
      }
    }
  }
}

int main(int argc, char *argv[]) // Take command line arguments
{
  if (argc < 2) { // Check for sufficient arguments
    printf("Usage: %s <list of integers>\n", argv[0]);
    return 1;
  }

  int n = argc - 1; // Number of elements in the array
  int nums[n]; // Array to hold the input integers
  for (int i = 0; i < n; i++) { // Populate the array with command line arguments
    nums[i] = atoi(argv[i + 1]); // Convert command line argument to integer
  }

  printf("Original array: ");
  for (int i = 0; i < n; i++) { // Print the original array
    printf("%d ", nums[i]);
  }
  printf("\n");

  bubbleSort(nums, n); // Call bubble sort function

  printf("Sorted array: ");
  for (int i = 0; i < n; i++) { // Print the sorted array
    printf("%d ", nums[i]);
  }
  printf("\n");

  return 0;
}
