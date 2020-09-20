#ifndef SORT_H
#define SORT_H

#include "triangle.h"

enum sort_method
{
	SORT_NONE,
	SORT_QUICK
} sort_method;

void int_swap (int* a, int* b);
void void float_swap (float* a, float* b);
void quicksort_triangles(triangle_t arr[], int first_index, int last_index);

#endif
