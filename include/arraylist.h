//
// Attempt at a javalike ArrayList
//

#ifndef libfaafo_ARRAYLIST_H
#define libfaafo_ARRAYLIST_H

#include <stdbool.h>
#include <stdlib.h>
#include "commons.h"

#define ARRAYLIST_DEFAULT_CAPACITY 10
#define ArrayList_new() (ArrayList_create(ARRAYLIST_DEFAULT_CAPACITY))

typedef struct ArrayList {
    void **data;
    unsigned int size;
    unsigned int capacity;
} ArrayList;

ArrayList *ArrayList_create(unsigned int capacity);
bool ArrayList_add(ArrayList *list, void *value) __nonnull((1));
bool ArrayList_contains(const ArrayList *list, const void *value) __nonnull((1, 2));
bool ArrayList_contains_all(const ArrayList *list, void **data, unsigned int data_count) __nonnull((1, 2));
int ArrayList_index_of(const ArrayList *list, const void *value) __nonnull((1, 2));
void *ArrayList_set(const ArrayList *list, unsigned int index, void *value) __nonnull((1, 3));
void *ArrayList_get(const ArrayList *list, unsigned int index) __nonnull((1));
void *ArrayList_remove(ArrayList *list, unsigned int index) __nonnull((1));
void **ArrayList_clear(ArrayList *list, destructor_fn df) __nonnull((1));
bool ArrayList_destroy(ArrayList *list) __nonnull((1));
void **ArrayList_clear_destroy(ArrayList *list, destructor_fn df) __nonnull((1));
#endif
