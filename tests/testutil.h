//
// Created by johan on 2025-07-22.
//

#ifndef libfaafo_TESTUTIL_H
#define libfaafo_TESTUTIL_H
#include <bstrlib.h>
#include <hashmap.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

static inline int *TestUtil_allocate_int(const int i) {
    int *i_ptr = calloc(1, sizeof(int));
    *i_ptr = i;
    return i_ptr;
}

static inline float *TestUtil_allocate_float(const float f) {
    float *f_ptr = calloc(1, sizeof(float));
    *f_ptr = f;
    return f_ptr;
}

static inline char *TestUtil_allocate_char(const char c) {
    char *c_ptr = calloc(1, sizeof(char));
    *c_ptr = c;
    return c_ptr;
}

static inline char *TestUtil_allocate_string(const char *str) {
    char *str_ptr = malloc(strlen(str) + 1);
    strcpy(str_ptr, str);
    return str_ptr;
}

static inline bool testutil_equals_fn_bstring(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return a == b;  // Both NULL is equal, one NULL is not equal
    }
    return biseq((bstring)a, (bstring)b);
}

static inline bool TestUtil_equals_fn_string(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return a == b;  // Both NULL is equal, one NULL is not equal
    }
    return strcmp((char*)a, (char*)b) == 0;
}

static inline bool TestUtil_equals_fn_int(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return a == b;  // Both NULL is equal, one NULL is not equal
    }
    return *(int*)a == *(int*)b;
}

static inline bool TestUtil_equals_fn_char(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return a == b;  // Both NULL is equal, one NULL is not equal
    }
    return *(char*)a == *(char*)b;
}

static inline bool TestUtil_equals_fn_float(const void *a, const void *b) {
    if (a == NULL || b == NULL) {
        return a == b;  // Both NULL is equal, one NULL is not equal
    }
    float fa = *(float*)a;
    float fb = *(float*)b;

    // Use epsilon comparison instead of direct equality
    return fabsf(fa - fb) < 1e-6f;  // or whatever epsilon makes sense for your use case

}

size_t TestUtil_hash_fn_int(const void *key);
size_t TestUtil_hash_fn_bstring(const void *key);
int TestUtil_sort_bstring(void **a, void **b);
int TestUtil_sort_int(void **a, void **b);
#endif //libfaafo_TESTUTIL_H
