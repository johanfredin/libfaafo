//
// Created by johan on 2025-07-23.
//
#include "hashmap.h"

#include <dbg.h>
#include <stdint.h>
#include <stdlib.h>

#define reset_size_and_capacity(map)                \
    (map)->size = 0;                                \
    (map)->capacity=HASHMAP_DEFAULT_CAPACITY

static size_t generate_hash(const HashMap *map, const void *key);
static MapEntry *create_entry(void *key, void *value, size_t hash);
static void add_new_entry(HashMap *map, size_t index, void *key, void *value, size_t hash);
static void *handle_existing(HashMap *map, LinkedList *bucket, size_t hash, void *key, void *new_value);
static bool expand(HashMap *map);
static bool resize(HashMap *map, size_t new_capacity);
static bool rehash_old_buckets(const HashMap *map, size_t old_cap);

HashMap *HashMap_create(const size_t capacity, const hash_fn hash_fn, const equals_fn equals_fn,
                        const destructor_fn df) {
    check_return(capacity > 0, "Capacity must be > 0", NULL);
    check_return((capacity % 2 == 0), "Capacity must be a power of 2", NULL);

    HashMap *map = calloc(1, sizeof(HashMap));
    check_mem_return(map, NULL);

    LinkedList **buckets = calloc(capacity, sizeof(LinkedList *));
    check_mem(buckets, goto catch);

    map->buckets = buckets;
    map->size = 0;
    map->capacity = capacity;
    map->hash_fn = hash_fn;
    map->equals_fn = equals_fn;
    map->df = df ? df : free;
    return map;
catch:
    HashMap_destroy(map);
    return NULL;
}

void *HashMap_put(HashMap *const map, void *key, void *const value) {
    check_return(map, "Map is null", NULL);
    check_return(key, "Key is null", NULL);
    check_return(value, "Value is null", NULL);

    const bool is_time_to_expand = ((float) map->size / (float) map->capacity) >= HASHMAP_LOAD_FACTOR;
    if (is_time_to_expand) {
        const bool is_expanded = expand(map);
        check_return(is_expanded, "Failed to expand map", NULL);
    }
    const size_t hash = generate_hash(map, key);
    const size_t index = hash & (map->capacity - 1); // Java style but will break if cap not powers of 2
    LinkedList *bucket = map->buckets[index];
    if (bucket) {
        return handle_existing(map, bucket, hash, key, value);
    }
    add_new_entry(map, index, key, value, hash);
    return NULL; // No previous entry to return
}

void *HashMap_get(const HashMap *map, void *key) {
    check_return(map, "Map is null", NULL);
    check_return(key, "Key is null", NULL);

    const size_t hash = generate_hash(map, key);
    const size_t index = hash & (map->capacity - 1); // Java style but will break if cap not powers of 2
    LinkedList *bucket = map->buckets[index];
    check_return(bucket, "No entry found", NULL);
    if (LinkedList_size(bucket) == 1) {
        const MapEntry *entry = bucket->first->value;
        return entry->value; // If only one entry in bucket we don't need to do equals checks.
    }

    for (const Node *curr = bucket->first; curr != NULL; curr = curr->next) {
        const MapEntry *entry = curr->value;
        if (entry->hash == hash && map->equals_fn(entry->key, key)) {
            return entry->value;
        }
    }
    return NULL;
}

bool HashMap_destroy(HashMap *map) {
    check_return(map, "Map is null", false);
    const bool cleared = HashMap_clear(map);
    check_return(cleared, "Could not clear map", false);
    free(map->buckets);
    free(map);
    return true;
}

bool HashMap_clear(HashMap *map) {
    check_return(map, "Map is null", false);
    for (size_t i = 0; i < map->capacity; i++) {
        LinkedList *bucket = map->buckets[i];
        if (bucket) {
            /*
             * Since we only initialize a bucket when needing to put data at that index, NULL buckets are likely
             * to exist, and we don't want to break out if we hit one.
             */
            LinkedList_destroy(bucket);
        }
    }
    reset_size_and_capacity(map);
    return true;
}

static size_t generate_hash(const HashMap *map, const void *key) {
    size_t hash = map->hash_fn(key);
    hash ^= (hash >> 16);
    return hash;
}

static MapEntry *create_entry(void *const key, void *const value, const size_t hash) {
    MapEntry *entry = calloc(1, sizeof(MapEntry));
    check_mem_return(entry, NULL);

    entry->key = key;
    entry->value = value;
    entry->hash = hash;
    return entry;
}

static void add_new_entry(HashMap *const map, const size_t index, void *const key, void *const value,
                          const size_t hash) {
    LinkedList *bucket = LinkedList_create(map->df);
    check(bucket, "Failed to create bucket", return);

    MapEntry *entry = create_entry(key, value, hash);
    if (!entry) {
        LinkedList_destroy(bucket);
        return;
    }
    LinkedList_push(bucket, entry);
    map->buckets[index] = bucket;
    map->size++;
}

static void *handle_existing(HashMap *const map, LinkedList *const bucket, const size_t hash, void *const key,
                             void *const new_value) {
    // Look for an existing entry (same key+value) in the buckets list, replace existing
    for (const Node *curr = bucket->first; curr != NULL; curr = curr->next) {
        MapEntry *entry = curr->value;

        if (entry->hash == hash && map->equals_fn(entry->key, key)) {
            void *old_value = entry->value;
            entry->value = new_value;
            return old_value;
        }
    }

    MapEntry *entry = create_entry(key, new_value, hash);
    check_mem_return(entry, NULL);
    LinkedList_push(bucket, entry);
    map->size++;
    return NULL;
}

static bool expand(HashMap *const map) {
    check_return(map->capacity < SIZE_MAX, "Map can not be expanded further", false);
    const size_t curr_cap = map->capacity;
    size_t new_cap = curr_cap << 1; // double the cap
    if (new_cap >= (SIZE_MAX >> 1)) {
        new_cap = SIZE_MAX;
        log_warn("Max capacity reached for map, further attempts to expand will cause failure");
    }
    const bool is_resized = resize(map, new_cap);
    check_return(is_resized, "Failed to resize map", false);

    return true;
}

static bool resize(HashMap *const map, const size_t new_capacity) {
    // Expand the list
    LinkedList **new_buckets = realloc(map->buckets, new_capacity * sizeof(LinkedList *));
    check_mem_return(new_buckets, false);
    map->buckets = new_buckets;

    const size_t old_cap = map->capacity;
    // Fill out expanded memory with 0
    LinkedList **start = map->buckets + old_cap;
    const size_t n_bytes_to_fill = (new_capacity - old_cap) * sizeof(LinkedList *);
    memset(start, 0, n_bytes_to_fill);

    rehash_old_buckets(map, old_cap);
    map->capacity = new_capacity;
    return true; // Failed to rehash old buckets, so reset to old capacity and return false
}

static bool rehash_old_buckets(const HashMap *const map, const size_t old_cap) {
    // Process each old bucket
    for (size_t old_index = 0; old_index < old_cap; old_index++) {
        LinkedList *old_bucket = map->buckets[old_index];
        if (!old_bucket) {
            debug("Old bucket was never allocated");
            continue;
        }

        // Java optimization: entries go either old_index or old_index + old_cap
        LinkedList *low_bucket = NULL;
        LinkedList *high_bucket = NULL;

        for (const Node *curr = old_bucket->first; curr != NULL; curr = curr->next) {
            MapEntry *entry = curr->value;

            // Works because capacity is always powers of 2
            if (entry->hash & old_cap) {
                if (!high_bucket) {
                    high_bucket = LinkedList_create(map->df);
                }
                LinkedList_push(high_bucket, entry);
            } else {
                if (!low_bucket) {
                    low_bucket = LinkedList_create(map->df);
                }
                LinkedList_push(low_bucket, entry);
            }
        }

        // Assign buckets to their new positions
        map->buckets[old_index] = low_bucket;
        map->buckets[old_index + old_cap] = high_bucket;

        LinkedList_destroy(old_bucket);
    }

    return true;
}
