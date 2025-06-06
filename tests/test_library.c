#include "library.h"
#include "unity.h"

void setUp(void) {
    // This is run before each test
    puts("setup");
}

void tearDown(void) {
    // This is run after each test
    puts("teardown");
}

void test_hello_function_runs(void) {
    TEST_ASSERT_NOT_NULL(hello);
    hello();
    TEST_PASS();
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hello_function_runs);
    return UNITY_END();
}