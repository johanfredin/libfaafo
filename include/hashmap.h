//
// Created by johan on 2025-07-23.
//

#ifndef libfaafo_HASHMAP_H
#define libfaafo_HASHMAP_H
#include <commons.h>
#include <stddef.h>
#include <stdbool.h>
#include "list.h"

#define HASHMAP_DEFAULT_CAPACITY 16

#define HashMap_new_type_bstring(hash) (HashMap_create(HASHMAP_DEFAULT_CAPACITY, (hash), bstrcmp))

typedef size_t (*hash_fn)(const void *key);
typedef bool (*equals_fn)(const void *key1, const void *key2);

typedef struct MapEntry {
    void *key;
    void *value;
    size_t hash;
} MapEntry;

typedef struct HashMap {
    List **buckets;
    size_t capacity;
    size_t size;
    hash_fn hash_fn;
    equals_fn equals_fn;
} HashMap;

HashMap *HashMap_create(size_t capacity, hash_fn hash_fn, equals_fn equals_fn);
void *HashMap_put(const HashMap *map, void *key, void *value);
void *HashMap_get(const HashMap *map, void *key);
// void *HashMap_remove(const HashMap *map, void *key);
void HashMap_destroy(HashMap *map);
// bool HashMap_contains_key(const HashMap *map, void *key);
// bool HashMap_is_empty(const HashMap *map);



#endif //libfaafo_HASHMAP_H
