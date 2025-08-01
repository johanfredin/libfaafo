//
// Created by johan on 2025-07-23.
//
#include <unity.h>
#include <hashmap.h>
#include "testutil.h"

static HashMap *map;

static void test_destroy_function(void *ptr) {
    MapEntry *me = ptr;
    Commons_bstring_destroy(me->key);
    Commons_bstring_destroy(me->value);
    free(me);
}

void setUp(void) {
    map = NULL;
}

void tearDown(void) {
    HashMap_destroy(map);
}

void test_create(void) {
    map = HashMap_create(
        HASHMAP_DEFAULT_CAPACITY,
        testutil_hash_fn_bstring,
        testutil_equals_fn_bstring,
        test_destroy_function
    );

    TEST_ASSERT_NOT_NULL(map);
    TEST_ASSERT_EQUAL_INT(map->capacity, HASHMAP_DEFAULT_CAPACITY);
    TEST_ASSERT_EQUAL_INT(map->size, 0);
    TEST_ASSERT_EQUAL_PTR(testutil_equals_fn_bstring, map->equals_fn);
    TEST_ASSERT_EQUAL_PTR(testutil_hash_fn_bstring, map->hash_fn);
    TEST_ASSERT_NOT_NULL(map->buckets);
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; i++) {
        TEST_ASSERT_NULL(map->buckets[i]);
    }
}

void test_put(void) {
    // Set up
    map = HashMap_create(HASHMAP_DEFAULT_CAPACITY, testutil_hash_fn_bstring, testutil_equals_fn_bstring, test_destroy_function);
    bstring key = bfromcstr("key");
    bstring value = bfromcstr("value");
    bstring key2 = bfromcstr("key2");
    bstring value2 = bfromcstr("value2");

    // Act
    bstring old_val = HashMap_put(map, key, value);
    TEST_ASSERT_NULL(old_val);
    TEST_ASSERT_EQUAL_INT(1, map->size);

    old_val = HashMap_put(map, key2, value2);
    TEST_ASSERT_NULL_MESSAGE(old_val, "Should still be null");
    TEST_ASSERT_EQUAL_INT(2, map->size);

    old_val = HashMap_put(map, key2, value);
    TEST_ASSERT_EQUAL_PTR(value2, old_val);
    TEST_ASSERT_EQUAL_INT(2, map->size);

    bdestroy(old_val);
}

void test_get(void) {
    // Set up
    map = HashMap_create(HASHMAP_DEFAULT_CAPACITY, testutil_hash_fn_bstring, testutil_equals_fn_bstring, test_destroy_function);
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; i++) {
        bstring key = bformat("key %d", i + 1);
        bstring value = bformat("value %d", i + 1);
        HashMap_put(map, key, value);
    }
    TEST_ASSERT_EQUAL_INT(HASHMAP_DEFAULT_CAPACITY, map->size);
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; i++) {
        bstring key = bformat("key %d", i + 1);
        bstring value = HashMap_get(map, key);
        bstring expected_value = bformat("value %d", i + 1);
        TEST_ASSERT_EQUAL_STRING(expected_value->data, value->data);

        bdestroy(key);
        bdestroy(expected_value);
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(HASHMAP_DEFAULT_CAPACITY * 2, map->capacity, "Map capacity should have been doubled");
}

int main(void) {
    UNITY_BEGIN();
    // RUN_TEST(test_create);
    RUN_TEST(test_put);
    // RUN_TEST(test_get);
    return UNITY_END();
}
