// Merge sort implementation in C
#include <stdio.h>
#include <stdlib.h>

void merge(int arr[], int l, int m, int r) { // Merge function
  int n1 = m - l + 1; // Size of left subarray
  int n2 = r - m; // Size of right subarray
  int L[n1], R[n2]; // Temporary arrays
  for (int i = 0; i < n1; i++) // Copy data to temporary arrays
    L[i] = arr[l + i]; // Copy elements to left subarray
  for (int j = 0; j < n2; j++) // Copy elements to right subarray
    R[j] = arr[m + 1 + j]; // Copy elements to right subarray
  int i = 0, j = 0, k = l; // Initialize pointers
  while (i < n1 && j < n2) { // Merge the temporary arrays
    if (L[i] <= R[j]) { // Compare elements
      arr[k++] = L[i++]; // Copy element from left subarray
    } else {
      arr[k++] = R[j++]; // Copy element from right subarray
    }
  }
  while (i < n1) arr[k++] = L[i++]; // Copy remaining elements from left subarray
  while (j < n2) arr[k++] = R[j++]; // Copy remaining elements from right subarray
}

void mergeSort(int arr[], int l, int r) { // Merge sort function
  if (l < r) { // Check if the array has more than one element
    int m = l + (r - l) / 2; // Find the middle point
    mergeSort(arr, l, m); // Sort the left half
    mergeSort(arr, m + 1, r); // Sort the right half
    merge(arr, l, m, r); // Merge the sorted halves
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

  mergeSort(nums, 0, n - 1); // Sort the array

  printf("Sorted array: ");
  for (int i = 0; i < n; i++) { // Print sorted array
    printf("%d ", nums[i]); // Print each element
  }
  printf("\n");

  return 0;
}