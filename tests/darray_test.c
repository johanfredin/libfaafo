#include <darray.h>
#include <ptr_deref.h>
#include <string.h>
#include <unity.h>

static DArray *array;
static int *val1;
static int *val2;

static int *dummy_ival(const int val) {
    int *iptr = malloc(sizeof(int));
    *iptr = val;
    return iptr;
}

void setUp(void) {
    val1 = NULL;
    val2 = NULL;
    array = DArray_new();
}

void tearDown(void) {
    DArray_clear_destroy(array);
}

void test_create(void) {
    TEST_ASSERT_TRUE_MESSAGE(array != NULL, "DArray create failed");
    TEST_ASSERT_EQUAL_INT(array->capacity, DARRAY_MINIMUM_CAPACITY);
    TEST_ASSERT_EQUAL_INT(array->size, 0);
}

void test_destroy(void) {
    DArray *localArray = DArray_new();
    DArray_add(localArray, val1);
    DArray_add(localArray, val2);
    DArray_destroy(localArray);
    TEST_ASSERT_TRUE_MESSAGE(1, "If we end up here means array was freed without error");
}

void test_set_get_remove(void) {
    // Set up
    val1 = malloc(sizeof(int));
    val2 = malloc(sizeof(int));
    DArray_add(array, val1);
    DArray_add(array, val2);

    // Verify inserted
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), val1, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), val2, "DArray_get failed");

    // Make valgrind happy
    free(val1);
    free(val2);

    int *new_val1 = dummy_ival(666);
    int *new_val2 = dummy_ival(999);
    DArray_set(array, 0, new_val1);
    DArray_set(array, 1, new_val2);

    // Verify new content
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), new_val1, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), new_val2, "DArray_get failed");

    const int *first_removed = DArray_remove(array, 0);
    const int *second_removed = DArray_remove(array, 1);
    // Verify removed correct pointers
    TEST_ASSERT_EQUAL_MESSAGE(*first_removed, *new_val1, "DArray_remove failed");
    TEST_ASSERT_TRUE_MESSAGE(first_removed != NULL, "First removed should not be null");
    TEST_ASSERT_EQUAL_MESSAGE(*second_removed, *new_val2, "DArray_remove failed");
    TEST_ASSERT_TRUE_MESSAGE(second_removed != NULL, "Second removed should not be null");

    // Verify no longer in the array
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), NULL, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), NULL, "DArray_get failed");

    free(new_val1);
    free(new_val2);

}

void test_push(void) {
    // Set up
    val1 = dummy_ival(1);
    val2 = dummy_ival(2);

    // Act
    DArray_add(array, val1);
    DArray_add(array, val2);

    // Verify
    TEST_ASSERT_EQUAL_INT_MESSAGE(array->size, 2, "DArray_push failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 0), val1, "DArray_get failed");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(DArray_get(array, 1), val2, "DArray_get failed");
}

void test_expand_contract(void) {
    const unsigned int old_capacity = array->capacity;
    
    // Fill array to trigger expansion
    for (int i = 0; i < old_capacity; i++) {
        DArray_add(array, dummy_ival(i));
    }
    
    // Push one more to trigger expansion
    DArray_add(array, dummy_ival(999));
    
    // Should have expanded by 50%
    unsigned int expected_capacity = old_capacity + (old_capacity >> 1);
    TEST_ASSERT_EQUAL_INT_MESSAGE(array->capacity, expected_capacity, 
                                  "DArray_expand not the expected size");
    
}

static int string_compare(const void **a, const void**b) {
    return strcmp(*a, *b);
}

static int int_compare(const void **a, const void**b) {
    return (int*) *a < (int*) *b;
}

void test_sort(void) {
    // Set up
    char *val1 = strdup("Bert");
    char *val2 = strdup("Alfred");

    DArray_add(array, val1);
    DArray_add(array, val2);

    TEST_ASSERT_EQUAL_STRING(deref_string(DArray_get(array, 0)), "Bert");
    TEST_ASSERT_EQUAL_STRING(deref_string(DArray_get(array, 1)), "Alfred");


    // Act
    DArray_sort(array, (DArray_compare_func) string_compare);

    // Verify
    TEST_ASSERT_EQUAL_STRING(deref_string(DArray_get(array, 0)), "Alfred");
    TEST_ASSERT_EQUAL_STRING(deref_string(DArray_get(array, 1)), "Bert");
}

void test_sort_int(void) {
    // Set up
    int *val1 = dummy_ival(2);
    int *val2 = dummy_ival(1);

    DArray_add(array, val1);
    DArray_add(array, val2);

    TEST_ASSERT_EQUAL_INT(deref_int(DArray_get(array, 0)), 2);
    TEST_ASSERT_EQUAL_INT(deref_int(DArray_get(array, 1)), 1);


    // Act
    DArray_sort(array, (DArray_compare_func) int_compare);

    // Verify
    TEST_ASSERT_EQUAL_INT(deref_int(DArray_get(array, 0)), 1);
    TEST_ASSERT_EQUAL_INT(deref_int(DArray_get(array, 1)), 2);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_set_get_remove);
    RUN_TEST(test_create);
    RUN_TEST(test_destroy);
    RUN_TEST(test_expand_contract);
    RUN_TEST(test_push);
    RUN_TEST(test_sort);
    RUN_TEST(test_sort_int);
    return UNITY_END();
}