//
// Created by johan on 2025-07-22.
//
#include "testutil.h"

#include <bstrlib.h>

int testutil_sort_bstring(void **a, void **b) {
    return bstrcmp(*a, *b);
}

int testutil_sort_int(void **a, void **b) {
    const int a_val = *(int*)*a;
    const int b_val = *(int*)*b;
    return a_val - b_val;
}
