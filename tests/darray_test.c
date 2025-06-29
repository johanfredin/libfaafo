//
// Created by johan on 2025-06-15.
//

#include <darray.h>
#include <unity.h>

static DArray *array;
static int *val1;
static int *val2;

void setUp(void) {
    val1 = NULL;
    val2 = NULL;
    array = DArray_create(sizeof(int), 100);
}

void tearDown(void) {
    free(val1);
    free(val2);
    free(array);
}

void test_create(void) {
    TEST_ASSERT_TRUE_MESSAGE(array != NULL, "DArray create failed");
    TEST_ASSERT_EQUAL_INT(array->max, 100);
    TEST_ASSERT_EQUAL_INT(array->element_size, sizeof(int));
    TEST_ASSERT_EQUAL_INT(array->end, 0);
}

void test_destroy(void) {
    DArray *localArray = DArray_create(sizeof(int), 100);
    DArray_push(localArray, val1);
    DArray_push(localArray, val2);
    DArray_destroy(localArray);
    TEST_ASSERT_TRUE_MESSAGE(1, "If we end up here means array was freed without error");
}

void test_new(void) {
    val1 = DArray_new(array);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(val1, NULL, "val1 was not initialized");
    val2 = DArray_new(array);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(val2, NULL, "val1 was not initialized");
}

void test_set_get_remove(void) {
    // Set up
    val1 = malloc(sizeof(int));
    val2 = malloc(sizeof(int));
    *val1 = 1;
    *val2 = 2;
    DArray_set(array, 0, val1);
    DArray_set(array, 1, val2);

    // Verify inserted
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), val1, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), val2, "DArray_get failed");

    const int *first_removed = DArray_remove(array, 0);
    const int *second_removed = DArray_remove(array, 1);
    // Verify removed correct pointers
    TEST_ASSERT_EQUAL_MESSAGE(*first_removed, *val1, "DArray_remove failed");
    TEST_ASSERT_TRUE_MESSAGE(first_removed != NULL, "First removed should not be null");
    TEST_ASSERT_EQUAL_MESSAGE(*second_removed, *val2, "DArray_remove failed");
    TEST_ASSERT_TRUE_MESSAGE(second_removed != NULL, "Second removed should not be null");

    // Verify no longer in the array
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), NULL, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), NULL, "DArray_get failed");
}

void test_push(void) {
    // Set up
    val1 = malloc(sizeof(int));
    val2 = malloc(sizeof(int));
    *val1 = 1;
    *val2 = 2;

    // Act
    DArray_push(array, val1);
    DArray_push(array, val2);

    // Verify
    TEST_ASSERT_EQUAL_INT_MESSAGE(array->end, 2, "DArray_push failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), val1, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), val2, "DArray_get failed");
}

void test_pop(void) {
    // Set up
    val1 = malloc(sizeof(int));
    val2 = malloc(sizeof(int));
    DArray_push(array, val1);
    DArray_push(array, val2);

    // Act
    const int *popped = DArray_pop(array);
    const int *popped2 = DArray_pop(array);

    // Verify
    TEST_ASSERT_EQUAL_PTR_MESSAGE(popped, val2, "DArray_pop failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(popped2, val1, "DArray_pop failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(array->end, 0, "DArray_pop failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), NULL, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), NULL, "DArray_get failed");
}

void test_expand_contract(void) {
    const unsigned int old_max = array->max;
    DArray_push(array, val1);
    DArray_push(array, val2);
    DArray_expand(array);
    TEST_ASSERT_EQUAL_INT_MESSAGE(array->max, old_max + array->expand_rate, "DArray_expand not the expected size");
    DArray_contract(array);
    TEST_ASSERT_EQUAL_INT_MESSAGE(array->max, array->expand_rate + 1, "DArray_contract failed");
    DArray_contract(array);
    TEST_ASSERT_EQUAL_INT_MESSAGE(array->max, array->expand_rate + 1, "Should stay at the expand rate");
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_destroy);
    RUN_TEST(test_new);
    RUN_TEST(test_set_get_remove);
    RUN_TEST(test_expand_contract);
    RUN_TEST(test_push);
    RUN_TEST(test_pop);
    return UNITY_END();
}
