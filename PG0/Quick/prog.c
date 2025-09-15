// Quick sort implementation in C
#include <stdio.h>
#include <stdlib.h>

void quickSort(int arr[], int low, int high) { // Quick sort function
  if (low < high) { // Check if the array has more than one element
    int pivot = arr[high]; // Choose the rightmost element as pivot
    int i = low - 1; // Initialize the smaller element index
    for (int j = low; j < high; j++) { // Iterate through the array
      if (arr[j] <= pivot) { // Compare elements
        i++; // Increment the smaller element index
        int temp = arr[i]; // Swap elements
        arr[i] = arr[j]; // Copy elements
        arr[j] = temp; // Copy elements
      }
    }
    int temp = arr[i + 1]; // Swap elements
    arr[i + 1] = arr[high]; // Copy elements
    arr[high] = temp; // Copy elements
    int pi = i + 1; // Partition index
    quickSort(arr, low, pi - 1); // Recursively sort the left subarray
    quickSort(arr, pi + 1, high); // Recursively sort the right subarray
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

  quickSort(nums, 0, n - 1); // Sort the array

  printf("Sorted array: ");
  for (int i = 0; i < n; i++) { // Print sorted array
    printf("%d ", nums[i]); // Print each element
  }
  printf("\n");

  return 0;
}