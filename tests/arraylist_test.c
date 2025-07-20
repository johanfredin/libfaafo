#include <unity.h>
#include <arraylist.h>
#include <bstrlib.h>

static ArrayList *list;

void setUp(void) {
    list = ArrayList_new();
}

void tearDown(void) {
    ArrayList_destroy(list);
}

void test_new(void) {
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, list->capacity);
    TEST_ASSERT_EQUAL_INT(0, list->size);
    TEST_ASSERT_NOT_NULL(list->data);
}

void test_create(void) {
    ArrayList *list2 = ArrayList_create(20);
    TEST_ASSERT_NOT_NULL(list2);
    TEST_ASSERT_EQUAL_INT(20, list2->capacity);
    TEST_ASSERT_EQUAL_INT(0, list2->size);
    TEST_ASSERT_NOT_NULL(list2->data);
    ArrayList_destroy(list2);
}

void test_add_no_resize_needed(void) {
    // Act
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        TEST_ASSERT_TRUE(ArrayList_add(list, bformat("value %d", i)));
    }

    // Verify
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, list->size);
    TEST_ASSERT_EQUAL_INT_MESSAGE(ARRAYLIST_DEFAULT_CAPACITY, list->capacity, "List capacity was altered");
    for (int i = 0; i < list->size; i++) {
        bstring val = ArrayList_get(list, i);
        bstring expected = bformat("value %d", i);
        TEST_ASSERT_EQUAL_STRING(expected->data, val->data);
        bdestroy(expected);
    }
}

void test_add_resize_needed(void) {
    // Setup
    const unsigned int new_cap = ARRAYLIST_DEFAULT_CAPACITY * 4;
    const unsigned int expected_new_size = new_cap - 5;

    // Act
    for (int i = 0; i < expected_new_size; i++) {
        TEST_ASSERT_TRUE(ArrayList_add(list, bformat("value %d", i)));
    }

    // Verify
    TEST_ASSERT_EQUAL_INT(expected_new_size, list->size);
    TEST_ASSERT_EQUAL_INT(new_cap + 9, list->capacity);
    for (int i = 0; i < list->size; i++) {
        bstring val = ArrayList_get(list, i);
        bstring expected = bformat("value %d", i);
        TEST_ASSERT_EQUAL_STRING(expected->data, val->data);
        bdestroy(expected);
    }
}

void test_contains(void) {
    // Set up
    bstring val1 = bfromcstr("value1");
    bstring val2 = bfromcstr("value2");

    ArrayList_add(list, val1);

    // Verify
    TEST_ASSERT_TRUE(ArrayList_contains(list, val1));
    TEST_ASSERT_FALSE(ArrayList_contains(list, val2));

    bdestroy(val2);
}

void test_contains_all_returns_true(void) {
    // Set up
    void *values[] = {
        bfromcstr("value1"),
        bfromcstr("value2")
    };

    // Act
    for (int i = 0; i < 2; i++) {
        ArrayList_add(list, values[i]);
    }

    // Verify
    TEST_ASSERT_TRUE(ArrayList_contains_all(list, values, 2));
}

void test_contains_all_returns_false(void) {
    // Set up
    void *values[] = {
        bfromcstr("value1"),
        bfromcstr("value2")
    };

    // Act (add only one out of the 2)
    ArrayList_add(list, values[1]);

    // Verify
    TEST_ASSERT_FALSE(ArrayList_contains_all(list, values, 1));
}

void test_index_of(void) {
    // Set up
    bstring val1 = bfromcstr("value1");
    bstring non_existing = bfromcstr("value2");
    ArrayList_add(list, val1);

    // Act
    const int index_found = ArrayList_index_of(list, val1);
    const int index_not_found = ArrayList_index_of(list, non_existing);

    // Verify
    TEST_ASSERT_EQUAL_INT(0, index_found);
    TEST_ASSERT_EQUAL_INT(-1, index_not_found);

    bdestroy(non_existing);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_new);
    RUN_TEST(test_create);
    RUN_TEST(test_add_no_resize_needed);
    RUN_TEST(test_add_resize_needed);
    RUN_TEST(test_contains);
    RUN_TEST(test_contains_all_returns_true);
    RUN_TEST(test_contains_all_returns_false);
    RUN_TEST(test_index_of);
    return UNITY_END();
}
