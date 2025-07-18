#ifndef libfaafo_HASHMAP_H
#define libfaafo_HASHMAP_H

#include "darray.h"
#include <stdint.h>

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*HashMap_compare_fn)(void *a, void *b);

typedef uint32_t (*HashMap_hash)(void *key);

typedef struct HashMap {
    DArray *buckets;
    HashMap_compare_fn compare;
    HashMap_hash hash;
} HashMap;

typedef struct HashMapNode {
    void *key;
    void *data;
    uint32_t hash;
} HashMapNode;

typedef int (*HashMap_traverse_cb)(HashMapNode *node);

HashMap *HashMap_create(HashMap_compare_fn compare_fn, HashMap_hash hash);

void HashMap_destroy(HashMap *map);

int HashMap_put(HashMap *map, void *key, void *data) __nonnull((1,2));

void *HashMap_get(HashMap *map, void *key) __nonnull((1, 2));

int HashMap_traverse(const HashMap *map, HashMap_traverse_cb traverse_cb) __nonnull((1, 2));

void *HashMap_delete(HashMap *map, void *key) __nonnull((2));
#endif //libfaafo_HASHMAP_H
