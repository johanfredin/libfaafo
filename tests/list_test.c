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

void test_copy() {
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;
    List_push(list, &v1);
    List_push(list, &v2);
    List_push(list, &v3);

    const List *copy = List_copy(list, sizeof(int));

    TEST_ASSERT_EQUAL_INT_MESSAGE(List_count(copy), 3, "Wrong amount in copy");

    const int *cpv1 = copy->first->value;
    const int *cpv2 = copy->first->next->value;
    const int *cpv3 = copy->last->value;

    // Verify values correct
    TEST_ASSERT_EQUAL_INT_MESSAGE(*cpv1, v1, "Different first values");
    TEST_ASSERT_EQUAL_INT_MESSAGE(*cpv2, v2, "Different second values");
    TEST_ASSERT_EQUAL_INT_MESSAGE(*cpv3, v3, "Different third values");

    // Verify pointer addresses different, if not; then it's not a true copy
    TEST_ASSERT_FALSE_MESSAGE(cpv1 == &v1, "Same address for v1");
    TEST_ASSERT_FALSE_MESSAGE(cpv2 == &v2, "Same address for v2");
    TEST_ASSERT_FALSE_MESSAGE(cpv3 == &v3, "Same address for v3");

}

void test_join() {
    // Set up
    int v1 = 1;
    int v2 = 2;
    int v3 = 3;
    List_push(list, &v1);
    List_push(list, &v2);
    List_push(list, &v3);

    List *newList = List_create();
    int v4 = 4;
    int v5 = 5;
    int v6 = 6;
    List_push(newList, &v4);
    List_push(newList, &v5);
    List_push(newList, &v6);

    // Act
    List_join(list, newList);

    // Verify
    TEST_ASSERT_EQUAL_INT_MESSAGE(List_count(list), 6, "Wrong amount");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(List_first(list), &v1, "Wrong first value");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(List_last(list), &v6, "Wrong last value");

    TEST_ASSERT_EQUAL_INT_MESSAGE(List_count(newList), 3, "Count altered for newList");

    int val = 1;
    for (const ListNode *cur = list->first; cur != NULL; cur = cur->next) {
        const int *curVal = cur->value;
        const int expected = val++;
        TEST_ASSERT_EQUAL_INT_MESSAGE(*curVal, expected, "Wrong value");
    }

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_push_pop);
    RUN_TEST(test_remove);
    RUN_TEST(test_copy);
    RUN_TEST(test_join);
    return UNITY_END();
}

