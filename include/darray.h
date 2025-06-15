//
// Created by johan on 2025-06-15.
//

#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include "dbg.h"

#define DArray_is_empty(A) ((A)->end == 0)
#define DArray_first(A) ((A)->contents[0])
#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_count(A) ((A)->end)
#define DArray_max(A) ((A)->max)
#define DArray_free(A) free((A))

#define DEFAULT_EXPAND_RATE 300

typedef struct DArray {
	int end;
	int max;
	size_t element_size;
	size_t expand_rate;
	void **contents;
} DArray;

DArray *DArray_create(size_t element_size, size_t initial_max);
void DArray_destroy(DArray *array);
void DArray_clear(DArray *array);
void DArray_clear_destroy(DArray *array);
int DArray_push(DArray *array, void *value);
void *DArray_pop(DArray *array);
int DArray_contract(DArray *array);


static inline void DArray_set(DArray *array, const int index, void *value) {
	check(index < array->max, "Index out of bounds", return);
	array->contents[index] = value;
	if (index > array->end) {
		array->end = index;
	}
	array->contents[index] = value;
}

static inline void *DArray_get(const DArray *array, const int index) {
	check_return(index < array->max, "Index out of bounds", NULL);
	return array->contents[index];
}

static inline void *DArray_remove(const DArray *array, const int index) {
	check_return(index < array->max, "Index out of bounds", NULL);
	void *element = array->contents[index];
	array->contents[index] = NULL;
	return element;
}

static inline void *DArray_new(const DArray *array) {
	check_return(array->element_size > 0, "Cant use DArray_new on 0 size darrays.", NULL);
	return calloc(1, array->element_size);
}


#endif //ARRAY_H
