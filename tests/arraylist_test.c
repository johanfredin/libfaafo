#include <unity.h>
#include <arraylist.h>
#include <bstrlib.h>

static ArrayList *list;

void setUp(void) {
    list = ArrayList_new();
}

void tearDown(void) {
    ArrayList_clear_destroy(list, bdestroy_wrapper);
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
    bdestroy(values[0]);
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

void test_set_get(void) {
    // Set up
    bstring initial_val = bfromcstr("value1");
    ArrayList_add(list, initial_val);
    TEST_ASSERT_EQUAL_PTR(initial_val, ArrayList_get(list, 0));

    // Try when index out of bounds
    TEST_ASSERT_NULL(ArrayList_get(list, 1));
    TEST_ASSERT_NULL(ArrayList_set(list, 1, initial_val));
    TEST_ASSERT_EQUAL_INT(1, list->size);

    // Test updating an existing
    bstring new_val = bfromcstr("value2");
    bstring old_val = ArrayList_set(list, 0, new_val);

    // Old value should be val1
    TEST_ASSERT_EQUAL_PTR(initial_val, old_val);
    TEST_ASSERT_EQUAL_PTR(new_val, ArrayList_get(list, 0));
    TEST_ASSERT_EQUAL_INT(1, list->size);

    bdestroy(old_val);
}

void test_remove(void) {
    // Set up
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        ArrayList_add(list, bformat("value %d", i));
    }

    // Act
    bstring removed = ArrayList_remove(list, 5);
    TEST_ASSERT_EQUAL_STRING("value 5", removed->data);
    TEST_ASSERT_EQUAL_INT_MESSAGE(ARRAYLIST_DEFAULT_CAPACITY, list->capacity, "Capacity changed should not be the case");
    TEST_ASSERT_EQUAL_INT_MESSAGE(ARRAYLIST_DEFAULT_CAPACITY - 1, list->size, "Size should be reduced by 1");
    TEST_ASSERT_FALSE(ArrayList_contains(list, removed));

    TEST_ASSERT_EQUAL_STRING("value 0", ((bstring)ArrayList_get(list, 0))->data);
    TEST_ASSERT_EQUAL_STRING("value 1", ((bstring)ArrayList_get(list, 1))->data);
    TEST_ASSERT_EQUAL_STRING("value 2", ((bstring)ArrayList_get(list, 2))->data);
    TEST_ASSERT_EQUAL_STRING("value 3", ((bstring)ArrayList_get(list, 3))->data);
    TEST_ASSERT_EQUAL_STRING("value 4", ((bstring)ArrayList_get(list, 4))->data);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("value 6", ((bstring)ArrayList_get(list, 5))->data, "This should now be value 6 since shift");
    TEST_ASSERT_EQUAL_STRING("value 7", ((bstring)ArrayList_get(list, 6))->data);
    TEST_ASSERT_EQUAL_STRING("value 8", ((bstring)ArrayList_get(list, 7))->data);
    TEST_ASSERT_EQUAL_STRING("value 9", ((bstring)ArrayList_get(list, 8))->data);


    bdestroy(removed);
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
    RUN_TEST(test_set_get);
    RUN_TEST(test_remove);
    return UNITY_END();
}
