// Histogram (Counting) sort implementation in C
#include <stdio.h>
#include <stdlib.h>

void histogramSort(int nums[], int n) { // Histogram sort function
  if (n == 0) return; // Check for empty array
  int min = nums[0], max = nums[0]; // Find min and max values
  for (int i = 1; i < n; i++) { // Find min and max values
    if (nums[i] < min) min = nums[i]; // Update min
    if (nums[i] > max) max = nums[i]; // Update max
  }
  int range = max - min + 1; // Calculate range
  int count[range]; // Create count array
  for (int i = 0; i < range; i++) count[i] = 0; // Initialize count array
  for (int i = 0; i < n; i++) count[nums[i] - min]++; // Populate count array
  int idx = 0; // Index for the original array
  for (int i = 0; i < range; i++) { // Iterate over the range
    while (count[i]-- > 0) { // While there are elements to place
      nums[idx++] = i + min; // Place elements in the original array
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

  histogramSort(nums, n); // Sort the array

  printf("Sorted array: ");
  for (int i = 0; i < n; i++) { // Print sorted array
    printf("%d ", nums[i]); // Print each element
  }
  printf("\n");

  return 0;
}
