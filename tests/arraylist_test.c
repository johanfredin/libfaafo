#include <unity.h>
#include <arraylist.h>
#include <bstrlib.h>
#include <ptr_deref.h>
#include <stdlib.h>

#include "testutil.h"

static ArrayList *static_list;

void setUp(void) {
    static_list = ArrayList_new();
}

void tearDown(void) {
    ArrayList_clear_destroy(static_list, bstring_destroy);
}

void test_new(void) {
    TEST_ASSERT_NOT_NULL(static_list);
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_capacity(static_list));
    TEST_ASSERT_EQUAL_INT(0, ArrayList_size(static_list));
}

void test_create(void) {
    ArrayList *list2 = ArrayList_create(20);
    TEST_ASSERT_NOT_NULL(list2);
    TEST_ASSERT_EQUAL_INT(20, ArrayList_capacity(list2));
    TEST_ASSERT_EQUAL_INT(0, ArrayList_size(list2));
    ArrayList_destroy(list2);
}

void test_add_no_resize_needed(void) {
    // Act
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        TEST_ASSERT_TRUE(ArrayList_add(static_list, bformat("value %d", i)));
    }

    // Verify
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_size(static_list));
    TEST_ASSERT_EQUAL_INT_MESSAGE(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_capacity(static_list), "List capacity was altered");
    for (int i = 0; i < ArrayList_size(static_list); i++) {
        bstring val = ArrayList_get(static_list, i);
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
        TEST_ASSERT_TRUE(ArrayList_add(static_list, bformat("value %d", i)));
    }

    // Verify
    TEST_ASSERT_EQUAL_INT(expected_new_size, ArrayList_size(static_list));
    TEST_ASSERT_EQUAL_INT(new_cap + 9, ArrayList_capacity(static_list));
    for (int i = 0; i < ArrayList_size(static_list); i++) {
        bstring val = ArrayList_get(static_list, i);
        bstring expected = bformat("value %d", i);
        TEST_ASSERT_EQUAL_STRING(expected->data, val->data);
        bdestroy(expected);
    }
}

void test_add_all(void) {
    // Set up
    void *values[] = {
        TestUtil_allocate_float(69.5f),
        TestUtil_allocate_float(420.69f),
        TestUtil_allocate_float(1337.0f)
    };

    ArrayList *new_list = ArrayList_new();

    // Test errors
    TEST_ASSERT_FALSE(ArrayList_add_all(new_list, values, 0));

    // Act
    ArrayList_add_all(new_list, values, 3);

    // Verify
    TEST_ASSERT_EQUAL_INT(3, ArrayList_size(new_list));

    TEST_ASSERT_EQUAL_FLOAT(69.5f, deref_float(ArrayList_get(new_list, 0)));
    TEST_ASSERT_EQUAL_FLOAT(420.69f, deref_float(ArrayList_get(new_list, 1)));
    TEST_ASSERT_EQUAL_FLOAT(1337.0f, deref_float(ArrayList_get(new_list, 2)));

    ArrayList_clear_destroy(new_list, free);

}

void test_contains(void) {
    // Set up
    bstring val1 = bfromcstr("value1");
    bstring val2 = bfromcstr("value2");

    ArrayList_add(static_list, val1);

    // Verify
    TEST_ASSERT_TRUE(ArrayList_contains(static_list, val1));
    TEST_ASSERT_FALSE(ArrayList_contains(static_list, val2));

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
        ArrayList_add(static_list, values[i]);
    }

    // Verify
    TEST_ASSERT_TRUE(ArrayList_contains_all(static_list, values, 2));
}

void test_contains_all_returns_false(void) {
    // Set up
    void *values[] = {
        bfromcstr("value1"),
        bfromcstr("value2")
    };

    // Act (add only one out of the 2)
    ArrayList_add(static_list, values[1]);

    // Verify
    TEST_ASSERT_FALSE(ArrayList_contains_all(static_list, values, 1));
    bdestroy(values[0]);
}

void test_index_of(void) {
    // Set up
    bstring val1 = bfromcstr("value1");
    bstring non_existing = bfromcstr("value2");
    ArrayList_add(static_list, val1);

    // Act
    const int index_found = ArrayList_index_of(static_list, val1);
    const int index_not_found = ArrayList_index_of(static_list, non_existing);

    // Verify
    TEST_ASSERT_EQUAL_INT(0, index_found);
    TEST_ASSERT_EQUAL_INT(-1, index_not_found);

    bdestroy(non_existing);
}

void test_set_get(void) {
    // Set up
    bstring initial_val = bfromcstr("value1");
    ArrayList_add(static_list, initial_val);
    TEST_ASSERT_EQUAL_PTR(initial_val, ArrayList_get(static_list, 0));

    // Try when index out of bounds
    TEST_ASSERT_NULL(ArrayList_get(static_list, 1));
    TEST_ASSERT_NULL(ArrayList_set(static_list, 1, initial_val));
    TEST_ASSERT_EQUAL_INT(1, ArrayList_size(static_list));

    // Test updating an existing
    bstring new_val = bfromcstr("value2");
    bstring old_val = ArrayList_set(static_list, 0, new_val);

    // Old value should be val1
    TEST_ASSERT_NOT_NULL(old_val);
    TEST_ASSERT_EQUAL_PTR(initial_val, old_val);
    TEST_ASSERT_EQUAL_PTR(new_val, ArrayList_get(static_list, 0));
    TEST_ASSERT_EQUAL_INT(1, ArrayList_size(static_list));

    bdestroy(old_val);
}

void test_remove(void) {
    // Set up
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        ArrayList_add(static_list, bformat("value %d", i));
    }

    // Act
    bstring removed = ArrayList_remove(static_list, 5);
    TEST_ASSERT_EQUAL_STRING("value 5", removed->data);
    TEST_ASSERT_EQUAL_INT_MESSAGE(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_capacity(static_list),
                                  "Capacity changed should not be the case");
    TEST_ASSERT_EQUAL_INT_MESSAGE(ARRAYLIST_DEFAULT_CAPACITY - 1, ArrayList_size(static_list), "Size should be reduced by 1");
    TEST_ASSERT_FALSE(ArrayList_contains(static_list, removed));

    TEST_ASSERT_EQUAL_STRING("value 0", ((bstring)ArrayList_get(static_list, 0))->data);
    TEST_ASSERT_EQUAL_STRING("value 1", ((bstring)ArrayList_get(static_list, 1))->data);
    TEST_ASSERT_EQUAL_STRING("value 2", ((bstring)ArrayList_get(static_list, 2))->data);
    TEST_ASSERT_EQUAL_STRING("value 3", ((bstring)ArrayList_get(static_list, 3))->data);
    TEST_ASSERT_EQUAL_STRING("value 4", ((bstring)ArrayList_get(static_list, 4))->data);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("value 6", ((bstring)ArrayList_get(static_list, 5))->data,
                                     "This should now be value 6 since shift");
    TEST_ASSERT_EQUAL_STRING("value 7", ((bstring)ArrayList_get(static_list, 6))->data);
    TEST_ASSERT_EQUAL_STRING("value 8", ((bstring)ArrayList_get(static_list, 7))->data);
    TEST_ASSERT_EQUAL_STRING("value 9", ((bstring)ArrayList_get(static_list, 8))->data);

    bdestroy(removed);
}

void test_size(void) {
    bstring val2 = bfromcstr("value2");
    TEST_ASSERT_EQUAL_INT(0, ArrayList_size(static_list));
    ArrayList_add(static_list, bfromcstr("value1"));
    ArrayList_add(static_list, val2);
    TEST_ASSERT_EQUAL_INT(2, ArrayList_size(static_list));

    ArrayList_remove(static_list, ArrayList_index_of(static_list, val2));
    TEST_ASSERT_EQUAL_INT(1, ArrayList_size(static_list));
    bdestroy(val2);
}

void test_is_empty(void) {
    TEST_ASSERT_TRUE(ArrayList_is_empty(static_list));
    ArrayList_add(static_list, bfromcstr("value1"));
    TEST_ASSERT_FALSE(ArrayList_is_empty(static_list));
}

void test_fist_last(void) {
    TEST_ASSERT_NULL(ArrayList_last(static_list));
    bstring s1 = bfromcstr("value1");
    bstring s2 = bfromcstr("value2");
    bstring s3 = bfromcstr("value3");
    ArrayList_add(static_list, s1);
    ArrayList_add(static_list, s2);
    ArrayList_add(static_list, s3);
    TEST_ASSERT_EQUAL_PTR(s3, ArrayList_last(static_list));
    TEST_ASSERT_EQUAL_PTR(s1, ArrayList_first(static_list));
}

void test_last_index(void) {
    bstring val2 = bfromcstr("value2");
    TEST_ASSERT_EQUAL_INT(-1, ArrayList_last_index(static_list));
    ArrayList_add(static_list, bfromcstr("value1"));
    ArrayList_add(static_list, val2);
    TEST_ASSERT_EQUAL_INT(1, ArrayList_last_index(static_list));
    ArrayList_remove(static_list, ArrayList_index_of(static_list, val2));
    TEST_ASSERT_EQUAL_INT(0, ArrayList_last_index(static_list));
    bdestroy(val2);
}

void test_clear_no_destroy_fn(void) {
    // Set up
    ArrayList *new_list = ArrayList_new();
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        ArrayList_add(new_list, bformat("value %d", i));
    }
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_size(new_list));

    // Act
    void **cleared = ArrayList_clear(new_list, NOOP_DESTRUCTOR);

    // Verify
    TEST_ASSERT_EQUAL_INT(0, ArrayList_size(new_list));
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_capacity(static_list));
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        TEST_ASSERT_NOT_NULL(cleared[i]); // Should not have been freed
        TEST_ASSERT_NULL(ArrayList_get(new_list, i)); // Contents in array should be empty

        // We have to manually destroy this data here
        bdestroy(cleared[i]);
    }
    free(cleared);
    ArrayList_destroy(new_list);
}

void test_clear_with_destroy_fn(void) {
    // Set up
    ArrayList *new_list = ArrayList_new();
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        ArrayList_add(new_list, TestUtil_allocate_int(i));
    }
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_size(new_list));

    // Act
    ArrayList_clear(new_list, free);
    TEST_ASSERT_EQUAL_INT(ARRAYLIST_DEFAULT_CAPACITY, ArrayList_capacity(new_list));

    // Verify
    TEST_ASSERT_EQUAL_INT(0, ArrayList_size(new_list));
    for (int i = 0; i < ARRAYLIST_DEFAULT_CAPACITY; i++) {
        TEST_ASSERT_NULL(ArrayList_get(new_list, i)); // Contents in array should be empty
    }
    ArrayList_destroy(new_list);
}

void test_sort_strings(void) {
    void *values[] = {
        bfromcstr("Johan"),
        bfromcstr("Julia"),
        bfromcstr("Evelyn")
    };
    ArrayList_add_all(static_list, values, 3);

    // Verify not sorted
    TEST_ASSERT_EQUAL_STRING("Johan", ((bstring)(ArrayList_get(static_list, 0)))->data);
    TEST_ASSERT_EQUAL_STRING("Julia", ((bstring)(ArrayList_get(static_list, 1)))->data);
    TEST_ASSERT_EQUAL_STRING("Evelyn", ((bstring)(ArrayList_get(static_list, 2)))->data);

    // Act
    const int sort = ArrayList_sort(static_list, (ArrayList_compare_fun)testutil_sort_bstring);

    // Verify
    TEST_ASSERT_EQUAL_INT(0, sort);
    TEST_ASSERT_EQUAL_STRING("Evelyn", ((bstring)(ArrayList_get(static_list, 0)))->data);
    TEST_ASSERT_EQUAL_STRING("Johan", ((bstring)(ArrayList_get(static_list, 1)))->data);
    TEST_ASSERT_EQUAL_STRING("Julia", ((bstring)(ArrayList_get(static_list, 2)))->data);
}

void test_sort_ints(void) {
    void *values[] = {
        TestUtil_allocate_int(30),
        TestUtil_allocate_int(90),
        TestUtil_allocate_int(60)
    };
    ArrayList *new_list = ArrayList_new();
    ArrayList_add_all(new_list, values, 3);

    // Verify not sorted
    TEST_ASSERT_EQUAL_INT(30, deref_int(ArrayList_get(new_list, 0)));
    TEST_ASSERT_EQUAL_INT(90, deref_int(ArrayList_get(new_list, 1)));
    TEST_ASSERT_EQUAL_INT(60, deref_int(ArrayList_get(new_list, 2)));

    // Act
    const int sort = ArrayList_sort(new_list, (ArrayList_compare_fun)testutil_sort_int);

    // Verify
    TEST_ASSERT_EQUAL_INT(0, sort);
    TEST_ASSERT_EQUAL_INT(30, deref_int(ArrayList_get(new_list, 0)));
    TEST_ASSERT_EQUAL_INT(60, deref_int(ArrayList_get(new_list, 1)));
    TEST_ASSERT_EQUAL_INT(90, deref_int(ArrayList_get(new_list, 2)));

    ArrayList_clear_destroy(new_list, free);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_new);
    RUN_TEST(test_create);
    RUN_TEST(test_add_no_resize_needed);
    RUN_TEST(test_add_resize_needed);
    RUN_TEST(test_add_all);
    RUN_TEST(test_contains);
    RUN_TEST(test_contains_all_returns_true);
    RUN_TEST(test_contains_all_returns_false);
    RUN_TEST(test_index_of);
    RUN_TEST(test_set_get);
    RUN_TEST(test_remove);
    RUN_TEST(test_size);
    RUN_TEST(test_is_empty);
    RUN_TEST(test_fist_last);
    RUN_TEST(test_last_index);
    RUN_TEST(test_clear_no_destroy_fn);
    RUN_TEST(test_clear_with_destroy_fn);
    RUN_TEST(test_sort_strings);
    RUN_TEST(test_sort_ints);
    return UNITY_END();
}
