//
// Created by johan on 2025-06-05.
//
#include <unity.h>
#include <linkedlist.h>
#include <ptr_deref.h>
#include <stdlib.h>

#include "testutil.h"

static void from_arraylist(bool is_destroy_src);

static LinkedList *list;

void setUp(void) {
     list = NULL;
}

void tearDown() {
    LinkedList_destroy(list);
}

void test_create(void) {
    LinkedList *noop_df_list = LinkedList_create(NULL);
    LinkedList *new = LinkedList_new();
    LinkedList *defined = LinkedList_create(Commons_bstring_destroy);

    TEST_ASSERT_TRUE(noop_df_list->first == NULL && new->first == NULL && defined->first == NULL);
    TEST_ASSERT_TRUE(noop_df_list->size = new->size = defined->size == 0);

    TEST_ASSERT_EQUAL_PTR(NOOP, noop_df_list->node_value_df);
    TEST_ASSERT_EQUAL_PTR(free, new->node_value_df);
    TEST_ASSERT_EQUAL_PTR(Commons_bstring_destroy, defined->node_value_df);

    LinkedList_destroy(noop_df_list);
    LinkedList_destroy(new);
    LinkedList_destroy(defined);
}

void test_from_ArrayList_no_df(void) {
    from_arraylist(false);
}

void test_from_array_list_destroy_src(void) {
    from_arraylist(true);
}

static void from_arraylist(const bool is_destroy_src) {
    // Set up
    ArrayList *arr = ArrayList_new();
    void *data[] = {
        TestUtil_allocate_int(10),
        TestUtil_allocate_int(20),
        TestUtil_allocate_int(30)
    };
    ArrayList_add_all(arr, data, 3);

    // Act
    list = LinkedList_from_ArrayList(arr, is_destroy_src);
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, list->size, "Wrong amount of elements in list");
    TEST_ASSERT_EQUAL_INT_MESSAGE(10, deref_int(LinkedList_first(list)), "Wrong first element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(20, deref_int(list->first->next->value), "Wrong middle element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(30, deref_int(LinkedList_find_last(list)), "Wrong last element");

    if (!is_destroy_src) {
        // We need to handle these manually in the test if we told function to not remove them
        ArrayList_destroy(arr);
    }
}

void test_from_array_stack_allocated(void) {
    // Set up
    int data[] = {10, 20, 30};

    // Act
    list = LinkedList_from_array(data, 3, sizeof(int), free);
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, list->size, "Wrong amount of elements in list");
    TEST_ASSERT_EQUAL_INT_MESSAGE(10, deref_int(LinkedList_first(list)), "Wrong first element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(20, deref_int(list->first->next->value), "Wrong middle element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(30, deref_int(LinkedList_find_last(list)), "Wrong last element");
}

void test_from_array_heap_allocated(void) {
    // Set up
    int *data = calloc(3, sizeof(int));
    data[0] = 10;
    data[1] = 20;
    data[2] = 30;

    // Act
    list = LinkedList_from_array(data, 3, sizeof(int), free);
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, list->size, "Wrong amount of elements in list");
    TEST_ASSERT_EQUAL_INT_MESSAGE(10, deref_int(LinkedList_first(list)), "Wrong first element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(20, deref_int(list->first->next->value), "Wrong middle element");
    TEST_ASSERT_EQUAL_INT_MESSAGE(30, deref_int(LinkedList_find_last(list)), "Wrong last element");
    free(data);
}

void test_push_all_and_find_node(void) {
    // Set up
    list = LinkedList_create(Commons_bstring_destroy);
    bstring test1 = bfromcstr("test1 data");
    bstring test2 = bfromcstr("test2 data");
    bstring test3 = bfromcstr("test3 data");
    bstring test4 = bfromcstr("test4 data");

    // Test error
    TEST_ASSERT_FALSE(LinkedList_push_all(list, (void*[]){NULL}, 0));

    // Act
    const bool pushed = LinkedList_push_all(list, (void*[]){test1, test2, test3, test4}, 4);
    TEST_ASSERT_TRUE(pushed);
    TEST_ASSERT_EQUAL_INT_MESSAGE(4, list->size, "Wrong amount of elements in list");
    const Node *res1 = LinkedList_find_node(list, test1);
    const Node *res2 = LinkedList_find_node(list, test2);
    const Node *res3 = LinkedList_find_node(list, test3);
    const Node *res4 = LinkedList_find_node(list, test4);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(test1, res1->value, "Wrong first element");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(test2, res2->value, "Wrong second element");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(test3, res3->value, "Wrong third element");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(test4, res4->value, "Wrong last element");
}

void test_push_pop(void) {
    // Set up
    list = LinkedList_create(Commons_noop);

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

void test_contains(void) {
    // Set up
    list = LinkedList_new();

    float *no_match = TestUtil_allocate_float(10.0f);
    float *match1 = TestUtil_allocate_float(20.0f);
    float *match2 = TestUtil_allocate_float(30.0f);
    TEST_ASSERT_FALSE(LinkedList_contains(list, no_match));

    LinkedList_push_all(list, (void*[]){match1, match2}, 2);
    TEST_ASSERT_TRUE(LinkedList_contains(list, match1));
    TEST_ASSERT_TRUE(LinkedList_contains(list, match2));

    free(no_match);
}

void test_remove(void) {
    // Set up
    list = LinkedList_create(Commons_noop);
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

void test_clear(void) {
    // Set up
    list = LinkedList_from_array((int[]){1, 2, 3}, 3, sizeof(int), free);
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, list->size, "Wrong amount of elements in list");

    // Act
    LinkedList_clear(list);

    // Verify
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, list->size, "List is not empty");
    TEST_ASSERT_NULL_MESSAGE(list->first, "First element is null");

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_push_pop);
    RUN_TEST(test_remove);
    RUN_TEST(test_from_ArrayList_no_df);
    RUN_TEST(test_from_array_list_destroy_src);
    RUN_TEST(test_from_array_stack_allocated);
    RUN_TEST(test_from_array_heap_allocated);
    RUN_TEST(test_push_all_and_find_node);
    RUN_TEST(test_contains);
    RUN_TEST(test_clear);
    return UNITY_END();
}


