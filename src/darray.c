//
// Created by johan on 2025-06-15.
//

#include "darray.h"
#include "dbg.h"

int DArray_sort(DArray *array, const DArray_compare_func compare_func) {
    check_mem(array, return -1);
    check(array->contents, "Array is empty", return -1);
    qsort(array->contents, DArray_size(array), sizeof(void *), compare_func);
    return 0;
}

static int DArray_resize(DArray *array, size_t new_capacity);

static int DArray_expand(DArray *array);

DArray *DArray_create(const unsigned int initial_capacity) {
    check_return(initial_capacity > 0, "initial size must be > 0", NULL);
    DArray *array = malloc(sizeof(DArray));
    check_mem_return(array, NULL);
    array->contents = calloc(initial_capacity, sizeof(void *));
    check_mem(array->contents, goto catch);

    array->size = 0;
    array->capacity = initial_capacity;
    return array;
catch:
    DArray_destroy(array);
    return NULL;
}

int DArray_add(DArray *array, void *value) {
    check(array, "Can not push into a NULL array", return -1);
    if (array->size >= array->capacity) {
        const unsigned int expand = DArray_expand(array);
        check_return(expand == 0, "Failed to expand array", -1);
    }
    array->contents[array->size] = value;
    array->size++;
    return 0;
}

void *DArray_pop(DArray *array) {
    check_return(array, "Can not pop from a NULL array", NULL);
    check_return(array->size > 0, "Array is already empty", NULL);
    void *element = DArray_remove(array, array->size - 1);
    array->size--;
    return element;
}

void *DArray_set(DArray *array, const unsigned int index, void *value) {
    check_return(index < array->size, "Index out of bounds", NULL);
    void *old_value = array->contents[index];
    array->contents[index] = value;
    if (index > array->size) {
        array->size = index;
    }
    array->contents[index] = value;
    return old_value;
}

void *DArray_get(const DArray *array, const unsigned int index) {
    check_return(index < array->size, "Index out of bounds", NULL);
    return array->contents[index];
}

void *DArray_remove(const DArray *array, const unsigned int index) {
    check_return(index < array->size, "Index out of bounds", NULL);
    void *element = array->contents[index];
    array->contents[index] = NULL;
    return element;
}

int DArray_trim_to_size(DArray *array) {
    check_return(array, "Can not trim a NULL array", -1);
    if (array->size < array->capacity) {
        return DArray_resize(array, array->size > 0 ? array->size : 1);
    }
    return 0;
}

void DArray_destroy(DArray *array) {
    if (array) {
        if (array->contents) {
            free(array->contents);
        }
        free(array);
    }
}

void DArray_clear(DArray *array) {
    check(array, "Can not clear a NULL array", return);
    for (unsigned int i = 0; i < array->size; i++) {
        free(array->contents[i]);
    }
    array->size = 0;
}

void DArray_clear_destroy(DArray *array) {
    DArray_clear(array);
    DArray_destroy(array);
}

static int DArray_expand(DArray *array) {
    check_return(array, "Can not expand a NULL array", -1);
    const unsigned int curr_cap = array->capacity;
    unsigned int new_cap = curr_cap + (curr_cap >> 1); // multiply by 1.5
    if (new_cap <= curr_cap) {
        new_cap = curr_cap + 1; // if overflow, just add 1
    }

    check_return(DArray_resize(array, new_cap) == 0,
                 "Failed to expand array to new size=%d", new_cap, -1);

    memset(
        array->contents + curr_cap,
        0,
        (new_cap - curr_cap) * sizeof(void *)
    ); // zero out new space (new_cap - curr_cap

    return 0;
}

static int DArray_resize(DArray *array, const size_t new_capacity) {
    check_return(array, "Can not resize a NULL array", -1);
    check_return(new_capacity > 0, "new_size must be > 0", -1);

    void *contents = realloc(array->contents, new_capacity * sizeof(void *));
    check_mem_return(contents, -1);
    array->contents = contents;
    array->capacity = new_capacity;
    return 0;
}
