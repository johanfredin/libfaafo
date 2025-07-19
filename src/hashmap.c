//
// Created by johan on 2025-07-16.
//
#include "hashmap.h"

#include <bstrlib.h>
#include <dbg.h>

typedef enum Mode {
    CREATE,
    GET
} Mode;

static uint32_t default_hash(const void *a) __nonnull((1));
static int default_compare(void *a, void *b);
static HashMapNode *Hashmap_node_create(uint32_t hash, void *key, void *data) __nonnull((2));
static DArray *Hashmap_find_bucket(const HashMap *map, void *key, Mode mode, uint32_t *hash_out);
static int Hashmap_get_node(const HashMap *map, uint32_t hash, const DArray *bucket, void *key);
static int is_power_of_2(unsigned int n);

HashMap *HashMap_create(const HashMap_compare_fn compare_fn, const HashMap_hash hash, const unsigned int initial_capacity) {
    check_return(is_power_of_2(initial_capacity), "Initial capacity must be a power of 2", NULL);
    HashMap *map = calloc(1, sizeof(HashMap));
    check_mem_return(map, NULL);

    map->compare = compare_fn ? compare_fn : default_compare;
    map->hash = hash ? hash : (HashMap_hash) default_hash;
    DArray *buckets = DArray_create(initial_capacity);
    check(buckets, "Could not create array", goto catch);

    map->buckets = buckets;
    return map;
catch:
    HashMap_destroy(map);
    return NULL;
}

void HashMap_destroy(HashMap *map) {
    check(map, "Map is null", return);
    check(map->buckets, "Buckets is null", return);

    for (int i = 0; i < DArray_cap(map->buckets); ++i) {
        DArray_clear_destroy(DArray_get(map->buckets, i));
    }

    DArray_destroy(map->buckets);
    free(map);
}

int HashMap_put(HashMap *map, void *key, void *data) {
    check_return(map, "Map can not be null", -1);
    check_return(key, "Key can not be null", -1);

    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, CREATE, &hash);
    check_return(bucket, "Bucket is null", -1);

    HashMapNode *node = Hashmap_node_create(hash, key, data);
    check_return(node, "Could not create node", -1);

    return DArray_add(bucket, node);
}

void *HashMap_get(HashMap *map, void *key) {
    check_mem_return(map, NULL);
    check_mem_return(key, NULL);

    uint32_t hash = 0;
    const DArray *bucket = Hashmap_find_bucket(map, key, GET, &hash);
    check_return(bucket, "Bucket is null", NULL);

    const int node_index = Hashmap_get_node(map, hash, bucket, key);
    check_return(node_index >= 0, "Node not found for key", NULL);

    const HashMapNode *node = DArray_get(bucket, node_index);
    check_return(node, "No hashmap node found for key", NULL);

    return node->data;
}

int HashMap_traverse(const HashMap *map, const HashMap_traverse_cb traverse_cb) {
    check_return(map, "Map is null", -1);
    check_return(traverse_cb, "Callback is null", -1);
    check_return(map->buckets, "Map has no buckets", -1);

    for (int i = 0; i < DArray_size(map->buckets); i++) {
        const DArray *bucket = DArray_get(map->buckets, i);
        check_return(bucket, "Bucket is null", -1);
        for (int j = 0; j < DArray_size(bucket); j++) {
            const int rc = traverse_cb(DArray_get(bucket, j));
            if (rc != 0) {
                return rc;
            }
        }
    }
    return 0;
}

void *HashMap_delete(HashMap *map, void *key) {
    check_return(map, "Map is null", NULL);
    check_return(key, "Key is null", NULL);

    uint32_t hash = 0;
    DArray *bucket = Hashmap_find_bucket(map, key, GET, &hash);
    check_return(bucket, "Bucket is null", NULL);

    const int node_index = Hashmap_get_node(map, hash, bucket, key);
    check_return(node_index >= 0, "Node not found for key", NULL);

    HashMapNode *node = DArray_get(bucket, node_index);
    check_return(node, "No hashmap node found for key", NULL);
    void *data = node->data;
    free(node);

    HashMapNode *last_node = DArray_pop(bucket);
    if (last_node != node) {
        // Node is not the last one in the array, so swap it
        DArray_set(bucket, node_index, last_node);
    }

    return data;
}

static int default_compare(void *a, void *b) {
    return bstrcmp((bstring) a, (bstring) b);
}

/**
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t default_hash(const void *a) {
    check_return(a, "Null arg", -1);

    const size_t len = blength((bstring) a);
    const char *key = bdata((bstring) a);
    check_return(key, "Null key", -1);

    uint32_t hash = 0;

    for (uint32_t i = 0; i < len; ++i) {
        hash += key[i];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

static HashMapNode *Hashmap_node_create(const uint32_t hash, void *key, void *data) {
    HashMapNode *node = calloc(1, sizeof(HashMapNode));
    check_mem_return(node, NULL);

    node->key = key;
    node->data = data;
    node->hash = hash;

    return node;
}

static DArray *Hashmap_find_bucket(const HashMap *map, void *key, const Mode mode, uint32_t *hash_out) {
    const uint32_t hash = map->hash(key);
    const uint32_t bucket_index = hash & (DEFAULT_NUMBER_OF_BUCKETS - 1);

    // Store hash in the out param so the caller can use it
    *hash_out = hash;

    DArray *bucket = DArray_get(map->buckets, bucket_index);
    if (!bucket && mode == CREATE) {
        // We have a new bucket to set up
        bucket = DArray_create(DEFAULT_NUMBER_OF_BUCKETS);
        check_mem_return(bucket, NULL);
        DArray_set(map->buckets, bucket_index, bucket);
    }

    return bucket;
}

static int Hashmap_get_node(const HashMap *map, const uint32_t hash, const DArray *bucket, void *key) {
    for (int i = 0; i < DArray_size(bucket); i++) {
        const HashMapNode *node = DArray_get(bucket, i);
        if (node->hash == hash && map->compare(node->key, key) == 0) {
            return i;
        }
    }
    return -1;
}

static int is_power_of_2(const unsigned int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

