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
#define DArray_end(A) ((A)->end)
#define DArray_max(A) ((A)->max)
#define DArray_free(A) free((A))

#define DARRAY_DEFAULT_EXPAND_RATE 300

typedef struct DArray {
	size_t element_size;
	size_t expand_rate;
	unsigned int end;
	unsigned int max;
	void **contents;
} DArray;

DArray *DArray_create(size_t element_size, size_t initial_max);
void DArray_destroy(const DArray *array);
void DArray_clear(DArray *array);
void DArray_clear_destroy(DArray *array);
int DArray_expand(DArray *array);
int DArray_contract(DArray *array);
int DArray_resize(DArray *array, size_t new_size);
int DArray_push(DArray *array, void *value);
void *DArray_pop(DArray *array);
int DArray_contract(DArray *array);
void DArray_set(DArray *array, unsigned int index, void *value);
void *DArray_get(const DArray *array, unsigned int index);
void *DArray_remove(const DArray *array, unsigned int index);


static void *DArray_new(const DArray *array) {
	check_return(array->element_size > 0, "Cant use DArray_new on 0 size DArrays.", NULL);
	return calloc(1, array->element_size);
}


#endif //ARRAY_H
