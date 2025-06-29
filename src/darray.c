//
// Created by johan on 2025-06-15.
//

#include "darray.h"

DArray *DArray_create(const size_t element_size, const size_t initial_max) {
    check_return(initial_max > 0, "initial_max must be > 0", NULL);
    DArray *array = malloc(sizeof(DArray));
    array->max = initial_max;
    array->contents = calloc(initial_max, sizeof(void *));
    check_mem(array->contents, goto catch);

    array->end = 0;
    array->element_size = element_size;
    array->expand_rate = DARRAY_DEFAULT_EXPAND_RATE;
    return array;
catch:
    DArray_free(array);
    return NULL;
}

void DArray_destroy(const DArray *array) {
    if (array) {
        if (array->contents) {
            free(array->contents);
        }
        free(array);
    }
}

int DArray_push(DArray *array, void *value) {
    check(array, "Can not push into a NULL array", return 0);
    array->contents[array->end] = value;
    array->end++;
    if (DArray_end(array) >= DArray_max(array)) {
        return DArray_expand(array);
    }
    return 0;
}

void *DArray_pop(DArray *array) {
    check(array, "Can not pop from a NULL array", return NULL);
    check(array->end > 0, "Array is empty", return NULL);
    void *element = DArray_remove(array, array->end - 1);
    array->end--;
    if (array->end > array->element_size && array->end % array->expand_rate) {
        DArray_contract(array);
    }
    return element;
}

int DArray_expand(DArray *array) {
    const unsigned int old_max = array->max;
    const unsigned int attempted_resize = array->max + array->expand_rate;
    check_return(DArray_resize(array, attempted_resize) == 0,
        "Failed to expand array to new size=%d", attempted_resize, -1);

    memset(array->contents + old_max, 0, array->expand_rate + 1);
    return 0;
}

int DArray_contract(DArray *array) {
    check_return(array, "Can not contract a NULL array", -1);
    check_return(array->end > 0, "Array is already empty", -1);
    int new_size;
    if (array->end < array->expand_rate) {
        new_size = DArray_resize(array, array->expand_rate + 1);
    } else {
        new_size = DArray_resize(array, array->end + 1);
    }
    check_return(new_size == 0, "Failed to contract array to new size=%d", new_size, -1);
    return new_size;
}

int DArray_resize(DArray *array, const size_t new_size) {
    check_return(new_size > 0, "new_size must be > 0", -1);
    void *contents = realloc(array->contents, new_size * sizeof(void*));
    check_mem_return(contents, -1);
    array->contents = contents;
    array->max = new_size;
    return 0;
}

void DArray_set(DArray *array, const unsigned int index, void *value) {
    check(index < array->max, "Index out of bounds", return);
    array->contents[index] = value;
    if (index > array->end) {
        array->end = index;
    }
    array->contents[index] = value;
}

void *DArray_get(const DArray *array, const unsigned int index) {
    check_return(index < array->max, "Index out of bounds", NULL);
    return array->contents[index];
}

void *DArray_remove(const DArray *array, const unsigned int index) {
    check_return(index < array->max, "Index out of bounds", NULL);
    void *element = array->contents[index];
    array->contents[index] = NULL;
    return element;
}

void DArray_clear_destroy(DArray *array) {
    DArray_clear(array);
    DArray_destroy(array);
}

void DArray_clear(DArray *array) {
    check(array, "Can not clear a NULL array", return);
    for (unsigned int i = 0; i < array->end; i++) {
        free(array->contents[i]);
    }
    array->end = 0;
}