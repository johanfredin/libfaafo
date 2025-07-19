#include <bstrlib.h>
#include <unity.h>
#include <hashmap.h>

static HashMap *map;

void setUp(void) {
    map = HashMap_new();
}

void tearDown(void) {
    HashMap_destroy(map);
}

void test_create(void) {
    // Map already created in setup so simply verify
    TEST_ASSERT_NOT_NULL(map);
    TEST_ASSERT_NOT_NULL(map->buckets);
    TEST_ASSERT_EQUAL_INT(DEFAULT_NUMBER_OF_BUCKETS, DArray_cap(map->buckets));
    TEST_ASSERT_NOT_NULL(map->hash);
    TEST_ASSERT_NOT_NULL(map->compare);
}

void test_put_new_value(void) {
    // Set up
    bstring key = bfromcstr("key");
    bstring value = bfromcstr("value");

    TEST_ASSERT_EQUAL_INT(0, HashMap_put(map, key, value));

    // Act
    const const_bstring *val = HashMap_get(map, key);

    // Verify
    TEST_ASSERT_EQUAL_STRING(value, val);

    bdestroy(key);
    bdestroy(value);
}

void test_put_existing_key(void) {
    // Set up
    bstring key = bfromcstr("key");
    bstring value = bfromcstr("value");
    HashMap_put(map, key, value);

    bstring value2 = bfromcstr("value2");

    // Act
    HashMap_put(map, key, value2);
    const const_bstring *val = HashMap_get(map, key);

    // Verify
    TEST_ASSERT_EQUAL_STRING(value2, val);

    bdestroy(key);
    bdestroy(value);
    bdestroy(value2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_put_new_value);
    RUN_TEST(test_put_existing_key);
    return UNITY_END();
}
