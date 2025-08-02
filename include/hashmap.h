//
// Created by johan on 2025-07-23.
//

#ifndef libfaafo_HASHMAP_H
#define libfaafo_HASHMAP_H
#include <commons.h>
#include <stddef.h>
#include <stdbool.h>
#include "linkedlist.h"

#define HASHMAP_DEFAULT_CAPACITY 16
#define HASHMAP_LOAD_FACTOR 0.75f

typedef size_t (*hash_fn)(const void *key);

typedef bool (*equals_fn)(const void *key1, const void *key2);

typedef struct MapEntry {
    void *key;
    void *value;
    size_t hash;
} MapEntry;

typedef struct HashMap {
    LinkedList **buckets;
    size_t capacity;
    size_t size;
    hash_fn hash_fn;
    equals_fn equals_fn;
    destructor_fn df;
} HashMap;

HashMap *HashMap_create(size_t capacity, hash_fn hash_fn, equals_fn equals_fn, destructor_fn df);
void *HashMap_put(HashMap *map, void *key, void *value) __nonnull((1, 2, 3));
void *HashMap_get(const HashMap *map, void *key) __nonnull((1, 2));
// void *HashMap_remove(const HashMap *map, void *key);
bool HashMap_destroy(HashMap *map) __nonnull((1));
bool HashMap_clear(HashMap *map) __nonnull((1));
// bool HashMap_contains_key(const HashMap *map, void *key);
// bool HashMap_is_empty(const HashMap *map);


#endif //libfaafo_HASHMAP_H
