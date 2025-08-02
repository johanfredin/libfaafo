//
// Created by johan on 2025-07-22.
//
#include "testutil.h"

#include <bstrlib.h>
#include <ptr_deref.h>

int TestUtil_sort_bstring(void **a, void **b) {
    return bstrcmp(*a, *b);
}

int TestUtil_sort_int(void **a, void **b) {
    const int a_val = *(int*)*a;
    const int b_val = *(int*)*b;
    return a_val - b_val;
}

size_t TestUtil_hash_fn_bstring(const void *key) {
    size_t hash = 0;
    bstring str = (bstring) key;
    for (int i = 0; i < blength(str); i++) {
        hash = 31 * hash + bchar(str, i);
    }
    return hash;
}

size_t TestUtil_hash_fn_int(const void *key) {
    return key ? (size_t)(deref_int(key)) : 0;
}
