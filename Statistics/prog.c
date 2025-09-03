// Statistics implementation in C
#include <stdio.h>
#include <stdlib.h>

// Helper function to sort the array (for median/mode)
void selectionSort(int arr[], int n) { // Selection sort function
  for (int i = 0; i < n - 1; i++) { // Iterate through the array
    int minIdx = i; // Initialize the minimum index
    for (int j = i + 1; j < n; j++) { // Find the minimum element
      if (arr[j] < arr[minIdx]) minIdx = j; // Update the minimum index
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

  // Calculate min and max
  int min = nums[0], max = nums[0]; // Initialize min and max
  double sum = nums[0]; // Initialize sum
  for (int i = 1; i < n; i++) { // Iterate through the array
    if (nums[i] < min) min = nums[i]; // Update min if needed
    if (nums[i] > max) max = nums[i]; // Update max if needed
    sum += nums[i]; // Update sum
  }
  double mean = sum / n; // Calculate mean

  // Sort for median and mode
  selectionSort(nums, n); // Sort the array

  // Median
  double median; // Declare median
  if (n % 2 == 1) // If odd length
    median = nums[n / 2]; // Middle element
  else
    median = (nums[n / 2 - 1] + nums[n / 2]) / 2.0; // Average of middle elements

  // Mode
  int mode = nums[0], modeCount = 1, current = nums[0], currentCount = 1; // Initialize mode variables
  for (int i = 1; i < n; i++) { // Iterate through the array
    if (nums[i] == current) { // If current element is the same as the last
      currentCount++; // Increment count
    } else {
      if (currentCount > modeCount) { // Update mode if needed
        mode = current; // Update mode
        modeCount = currentCount; // Update mode count
      }
      current = nums[i]; // Update current
      currentCount = 1; // Reset current count
    }
  }
  if (currentCount > modeCount) { // Final check for mode
    mode = current; // Update mode
    modeCount = currentCount; // Update mode count
  }

  printf("Statistics:\n"); // Print statistics header
  printf("Min: %d\n", min); // Print minimum
  printf("Max: %d\n", max); // Print maximum
  printf("Mean: %.2f\n", mean); // Print mean
  printf("Median: %.2f\n", median); // Print median
  printf("Mode: %d\n", mode); // Print mode

  return 0;
}