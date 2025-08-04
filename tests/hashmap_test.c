//
// Created by johan on 2025-07-23.
//
#include <unity.h>
#include <hashmap.h>
#include <ptr_deref.h>

#include "testutil.h"

static HashMap *map;

static void test_destroy_function(void *ptr) {
    MapEntry *me = ptr;
    Commons_bstring_destroy(me->key);
    Commons_bstring_destroy(me->value);
    free(me);
}

static void test_destroy_function_int(void *ptr) {
    MapEntry *me = ptr;
    free(me->key);
    free(me->value);
    free(me);
}

static HashMap *new_bstring_hashmap() {
    return HashMap_create(
        HASHMAP_DEFAULT_CAPACITY,
        TestUtil_hash_fn_bstring,
        (equals_fn)biseq,
        test_destroy_function
    );
}

void setUp(void) {
    map = NULL;
}

void tearDown(void) {
    if (map) {
        TEST_ASSERT_TRUE(HashMap_destroy(map));
    }
}

void test_create(void) {
    map = HashMap_create(
        HASHMAP_DEFAULT_CAPACITY,
        TestUtil_hash_fn_bstring,
        testutil_equals_fn_bstring,
        test_destroy_function
    );

    TEST_ASSERT_NOT_NULL(map);
    TEST_ASSERT_EQUAL_INT(map->capacity, HASHMAP_DEFAULT_CAPACITY);
    TEST_ASSERT_EQUAL_INT(map->size, 0);
    TEST_ASSERT_EQUAL_PTR(testutil_equals_fn_bstring, map->equals_fn);
    TEST_ASSERT_EQUAL_PTR(TestUtil_hash_fn_bstring, map->hash_fn);
    TEST_ASSERT_NOT_NULL(map->buckets);
    for (int i = 0; i < HASHMAP_DEFAULT_CAPACITY; i++) {
        TEST_ASSERT_NULL(map->buckets[i]);
    }
}

void test_put(void) {
    // Set up
    map = HashMap_create(HASHMAP_DEFAULT_CAPACITY, TestUtil_hash_fn_bstring, testutil_equals_fn_bstring,
                         test_destroy_function);
    bstring key1 = bfromcstr("key");
    bstring value1 = bfromcstr("value");
    bstring key2 = bfromcstr("key2");
    bstring value2 = bfromcstr("value2");

    // Test adding to empty map
    bstring old_val = HashMap_put(map, key1, value1);
    TEST_ASSERT_NULL_MESSAGE(old_val, "should be null since map is empty");
    TEST_ASSERT_EQUAL_INT(1, map->size);

    old_val = HashMap_put(map, key2, value2);
    TEST_ASSERT_NULL_MESSAGE(old_val, "Should still be null since nothing connected to key2");
    TEST_ASSERT_EQUAL_INT(2, map->size);

    // Test replace existing
    bstring new_player = bfromcstr("new player");
    old_val = HashMap_put(map, key2, new_player);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(value2, old_val,
                                  "new player should now be associated with key2 and old_value at key2 should have been returned");
    TEST_ASSERT_EQUAL_INT(2, map->size);

    bdestroy(old_val);
}

void test_get_no_resize_required(void) {
    // Set up
    map = HashMap_create(HASHMAP_DEFAULT_CAPACITY, TestUtil_hash_fn_bstring, testutil_equals_fn_bstring,
                         test_destroy_function);
    const size_t size = 10;
    for (int i = 0; i < size; i++) {
        bstring key = bformat("key %d", i + 1);
        bstring value = bformat("value %d", i + 1);
        HashMap_put(map, key, value);
    }
    TEST_ASSERT_EQUAL_INT(size, map->size);
    for (int i = 0; i < size; i++) {
        bstring key = bformat("key %d", i + 1);
        bstring value = HashMap_get(map, key);
        bstring expected_value = bformat("value %d", i + 1);
        TEST_ASSERT_EQUAL_STRING(expected_value->data, value->data);

        bdestroy(key);
        bdestroy(expected_value);
    }
    TEST_ASSERT_EQUAL_INT_MESSAGE(HASHMAP_DEFAULT_CAPACITY, map->capacity, "Map capacity should be the same");
}

void test_get_resize_required(void) {
    // Set up
    const size_t new_cap = HASHMAP_DEFAULT_CAPACITY << 1;

    map = HashMap_create(new_cap, TestUtil_hash_fn_int, TestUtil_equals_fn_int, test_destroy_function_int);
    for (int i = 0; i < new_cap; i++) {
        int *key = TestUtil_allocate_int(i + 1);
        int *value = TestUtil_allocate_int(i + 10);
        HashMap_put(map, key, value);
    }
    TEST_ASSERT_EQUAL_INT(new_cap, map->size);
    TEST_ASSERT_EQUAL_INT_MESSAGE(new_cap * 2, map->capacity, "Capacity should have doubled");
    for (int i = 0; i < new_cap; i++) {
        int key = i + 1;
        int *value = HashMap_get(map, &key);
        const int expected_value = i + 10;
        TEST_ASSERT_EQUAL_INT(expected_value, deref_int(value));
    }
}

void test_remove(void) {
    // Set up
    map = new_bstring_hashmap();
    bstring key = bfromcstr("key");
    bstring value = bfromcstr("value");
    bstring non_existing_key = bfromcstr("no good key");
    TEST_ASSERT_FALSE(HashMap_remove(map, non_existing_key));

    HashMap_put(map, key, value);
    TEST_ASSERT_FALSE_MESSAGE(HashMap_remove(map, non_existing_key), "Key should not in map");

    bool removed = HashMap_remove(map, key);
    TEST_ASSERT_TRUE_MESSAGE(removed, "Key should not be removed");

    bdestroy(non_existing_key);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_put);
    RUN_TEST(test_get_no_resize_required);
    RUN_TEST(test_get_resize_required);
    RUN_TEST(test_remove);
    return UNITY_END();
}
