// Insertion sort implementation in C
#include <stdio.h>
#include <stdlib.h>

void insertionSort(int nums[], int n) { // Insertion sort function
  for (int i = 1; i < n; i++) { // Iterate through each element
    int key = nums[i]; // Current element to be inserted
    int j = i - 1; // Index of the last sorted element
    while (j >= 0 && nums[j] > key) { // Find the correct position for the key
      nums[j + 1] = nums[j]; // Shift elements to the right
      j--; // Move to the next element
    }
    nums[j + 1] = key; // Insert the key at the correct position
  }
}

int main(int argc, char *argv[]) // Main function
{
  if (argc < 2) { // Check for sufficient arguments
    printf("Usage: %s <list of integers>\n", argv[0]); // Print usage message
    return 1; // Indicate failure
  }

  int n = argc - 1; // Number of elements in the array
  int nums[n]; // Array to hold the input numbers
  for (int i = 0; i < n; i++) { // Populate the array
    nums[i] = atoi(argv[i + 1]); // Convert input to integer
  }

  printf("Original array: ");
  for (int i = 0; i < n; i++) { // Print original array
    printf("%d ", nums[i]); // Print each element
  }
  printf("\n");

  insertionSort(nums, n); // Sort the array

  printf("Sorted array: ");
  for (int i = 0; i < n; i++) { // Print sorted array
    printf("%d ", nums[i]); // Print each element
  }
  printf("\n");

  return 0;
}