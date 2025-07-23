//
// Created by johan on 2025-07-23.
//
#include "hashmap.h"

#include <dbg.h>
#include <stdlib.h>

HashMap *HashMap_create(const size_t capacity, const hash_fn hash_fn, const equals_fn equals_fn) {
    HashMap *map = calloc(1, sizeof(HashMap));
    check_mem_return(map, NULL);

    List **buckets = calloc(capacity, sizeof(List *));
    check_mem(buckets, goto catch);

    map->buckets = buckets;
    map->size = 0;
    map->capacity = capacity;
    map->hash_fn = hash_fn;
    map->equals_fn = equals_fn;
    return map;
catch:
    HashMap_destroy(map);
    return NULL;
}

void *HashMap_put(const HashMap *map, void *key, void *value) {
    check_return(map, "Map is null", NULL);
    check_return(key, "Key is null", NULL);
    return NULL;
}

void *HashMap_get(const HashMap *map, void *key) {
    check_return(map, "Map is null", NULL);
    check_return(key, "Key is null", NULL);
    return NULL;
}

void HashMap_destroy(HashMap *map) {
    check(map, "Map is null", return);
    for (size_t i = 0; i < map->capacity; i++) {
        List_destroy(map->buckets[i]);
    }
    free(map->buckets);
    free(map);
}
