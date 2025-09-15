// Selection sort implementation in C
#include <stdio.h>
#include <stdlib.h>

void selectionSort(int arr[], int n) { // Selection sort function
  for (int i = 0; i < n - 1; i++) { // Iterate through the array
    int minIdx = i; // Initialize the minimum index
    for (int j = i + 1; j < n; j++) { // Find the minimum element
      if (arr[j] < arr[minIdx]) { // Compare elements
        minIdx = j; // Update the minimum index
      }
    }
    if (minIdx != i) { // Swap elements if needed
      int temp = arr[i]; // Store the current element
      arr[i] = arr[minIdx]; // Copy the minimum element
      arr[minIdx] = temp; // Copy the current element
    }
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

  selectionSort(nums, n); // Sort the array

  printf("Sorted array: ");
  for (int i = 0; i < n; i++) { // Print sorted array
    printf("%d ", nums[i]); // Print each element
  }
  printf("\n");

  return 0;
}