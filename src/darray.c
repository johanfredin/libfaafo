//
// Created by johan on 2025-06-15.
//

#include "darray.h"

static int DArray_expand(DArray *array);
static int DArray_resize(DArray *array, size_t new_size);

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

static int DArray_expand(DArray *array) {
    const size_t old_max = array->max;
    const size_t attempted_resize = array->max + array->expand_rate;
    check_return(DArray_resize(array, attempted_resize) == 0,
        "Failed to expand array to new size=%d", attempted_resize, -1);

    memset(array->contents + old_max, 0, array->expand_rate + 1);
    return 0;
}

static int DArray_resize(DArray *array, const size_t new_size) {
    check_return(new_size > 0, "new_size must be > 0", -1);
    void *contents = realloc(array->contents, new_size * sizeof(void*));
    check_mem_return(contents, -1);
    array->contents = contents;
    array->max = new_size;
    return 0;
}