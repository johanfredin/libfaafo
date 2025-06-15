//
// Created by johan on 2025-06-05.
//
#include <unity.h>
#include <list.h>
#include <dbg.h>

static List *list;

static char *test1 = "test1 data";
static char *test2 = "test2 data";
static char *test3 = "test2 data";

void setUp(void) {
    list = List_create();
}

void tearDown() {
    List_destroy(list);
}

void test_push_pop(void) {
    // Test push and count
    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(List_first(list), test1, "Wrong first value");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(List_last(list), test3, "Wrong last value");
    TEST_ASSERT_EQUAL_INT_MESSAGE(List_count(list), 3, "Wrong amount of entries in list");

    // Test pop
    const char *pop1 = List_pop(list);
    const char *pop2 = List_pop(list);
    const char *pop3 = List_pop(list);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(pop1, test3, "Wrong value popped first");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(pop2, test2, "Wrong value popped second");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(pop3, test1, "Wrong value popped third");
    TEST_ASSERT_TRUE_MESSAGE(List_is_empty(list), "List is not empty");
}

void test_remove() {
    List_push(list, test1);
    List_push(list, test2);
    List_push(list, test3);

    char *val = List_remove(list, list->first->next);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(val, test2, "Wrong removed element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(List_count(list), 2, "Wrong amount");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(List_first(list), test1, "Wrong first element");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(List_last(list), test3, "Wrong last element");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_push_pop);
    RUN_TEST(test_remove);
    return UNITY_END();
}
