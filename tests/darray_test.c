//
// Created by johan on 2025-06-15.
//

#include <darray.h>
#include <unity.h>

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

void setUp(void) {
    val1 = malloc(sizeof(int));
    val2 = malloc(sizeof(int));
    *val1 = 1;
    *val2 = 2;
}

void tearDown(void) {
    free(val1);
    free(val2);
}

void test_create(void) {
    array = DArray_create(sizeof(int), 100);
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
    // Ensure no crash
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_destroy);
    return UNITY_END();
}