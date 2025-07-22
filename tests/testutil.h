//
// Created by johan on 2025-07-22.
//

#ifndef TESTUTIL_H
#define TESTUTIL_H
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

int testutil_sort_bstring(void **a, void **b);
int testutil_sort_int(void **a, void **b);
#endif //TESTUTIL_H
