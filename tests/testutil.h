//
// Created by johan on 2025-07-22.
//

#ifndef TESTUTIL_H
#define TESTUTIL_H
#include <bstrlib.h>
#include <hashmap.h>
#include <stdbool.h>
#include <stdlib.h>

static inline int *TestUtil_allocate_int(const int i) {
    int *i_ptr = malloc(sizeof(int));
    *i_ptr = i;
    return i_ptr;
}

static inline float *TestUtil_allocate_float(const float f) {
    float *f_ptr = malloc(sizeof(float));
    *f_ptr = f;
    return f_ptr;
}

static inline bool testutil_equals_fn_bstring(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return a == b;  // Both NULL is equal, one NULL is not equal
    }
    return bstrcmp((bstring)a, (bstring)b) == 0;
}

static inline bool testutil_equals_fn_int(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return a == b;  // Both NULL is equal, one NULL is not equal
    }
    return *(int*)a == *(int*)b;
}

size_t testutil_hash_fn_int(const void *key);
size_t testutil_hash_fn_bstring(const void *key);
int testutil_sort_bstring(void **a, void **b);
int testutil_sort_int(void **a, void **b);
#endif //TESTUTIL_H
