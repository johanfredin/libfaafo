//
// Created by johan on 2025-06-05.
//
#include <unity.h>
#include <linkedlist.h>
#include <ptr_deref.h>
#include <stdlib.h>

#include "testutil.h"

static LinkedList *list;

void setUp(void) {
     list = NULL;
}

void tearDown() {
    LinkedList_destroy(list);
}

void test_push_pop(void) {
    // Set up
    list = LinkedList_create(noop_destroy);
    char *test1 = "test1 data";
    char *test2 = "test2 data";
    char *test3 = "test3 data";

    // Test push and count
    LinkedList_push(list, test1);
    LinkedList_push(list, test2);
    LinkedList_push(list, test3);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(LinkedList_first(list), test1, "Wrong first value");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(LinkedList_find_last(list), test3, "Wrong last value");
    TEST_ASSERT_EQUAL_INT_MESSAGE(LinkedList_size(list), 3, "Wrong amount of entries in list");

    // Test pop
    const char *pop1 = LinkedList_pop(list);
    const char *pop2 = LinkedList_pop(list);
    const char *pop3 = LinkedList_pop(list);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(pop1, test3, "Wrong value popped first");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(pop2, test2, "Wrong value popped second");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(pop3, test1, "Wrong value popped third");
    TEST_ASSERT_TRUE_MESSAGE(LinkedList_is_empty(list), "List is not empty");
}

void test_remove() {
    // Set up
    list = LinkedList_create(noop_destroy);
    static char *test1 = "test1 data";
    static char *test2 = "test2 data";
    static char *test3 = "test2 data";
    LinkedList_push(list, test1);
    LinkedList_push(list, test2);
    LinkedList_push(list, test3);

    // Act
    char *val = LinkedList_remove(list, list->first->next);

    // Verify
    TEST_ASSERT_EQUAL_PTR_MESSAGE(val, test2, "Wrong removed element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(LinkedList_size(list), 2, "Wrong amount");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(LinkedList_first(list), test1, "Wrong first element");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(LinkedList_find_last(list), test3, "Wrong last element");
}

void test_from_ArrayList(void) {
    // Set up
    ArrayList *arr = ArrayList_new();
    void *data[] = {
        TestUtil_allocate_int(10),
        TestUtil_allocate_int(20),
        TestUtil_allocate_int(30)
    };
    ArrayList_add_all(arr, data, 3);

    // Act
    list = LinkedList_from_ArrayList(arr);
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, list->size, "Wrong amount of elements in list");
    TEST_ASSERT_EQUAL_INT_MESSAGE(10, deref_int(LinkedList_first(list)), "Wrong first element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(20, deref_int(list->first->next->value), "Wrong middle element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(30, deref_int(LinkedList_find_last(list)), "Wrong last element");
    ArrayList_clear_destroy(arr, free);
}

void test_from_array(void) {
    // Set up
    int data[] = {10, 20, 30};

    // Act
    list = LinkedList_from_array(&data, 3, noop_destroy);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_push_pop);
    RUN_TEST(test_remove);
    RUN_TEST(test_from_ArrayList);
    RUN_TEST(test_from_array);
    return UNITY_END();
}
