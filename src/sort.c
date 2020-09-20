#include<stdio.h>
#include "sort.h"
#include "triangle.h"

void int_swap (int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void void float_swap (float* a, float* b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

// A utility function to swap two elements
void swap_triangle(triangle_t* a, triangle_t* b)
{
    triangle_t tmp = *a;
    *a = *b;
    *b = tmp;
}


int partition_triangle_array(triangle_t arr[], int first_index, int last_index)
{
  // Set the pivot value to the last element in the array
  float pivot = arr[last_index].avg_depth;

  // Store a swap index
  int i = first_index;

  // Iterate through the current partition, swapping elements larger than the pivot
  for (int j = first_index; j < last_index; j++) {
      if (arr[j].avg_depth > pivot) {
          // Swap if necessary...
          if (i != j) {
              swap_triangle(&arr[i], &arr[j]);
          }

          // Increment our swap index since the current element is larger than our pivot
          i++;
      }
  }

  // Swap our pivot element into place
  swap_triangle(&arr[i], &arr[last_index]);

  // All elements up to arr[i] should be larger than the pivot so return the pivot index
  return i;
}

  void quicksort_triangles(triangle_t arr[], int first_index, int last_index)
  {
      if (first_index < last_index) {
          int pivot_index = partition_triangle_array(arr, first_index, last_index);
          quicksort_triangles(arr, first_index, pivot_index - 1);
          quicksort_triangles(arr, pivot_index + 1, last_index);
      }
  }
