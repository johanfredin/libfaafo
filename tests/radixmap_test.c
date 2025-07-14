#include <radixmap.h>
#include <ptr_deref.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

#define MAX 100

static RadixMap *map;

// static int alloc_fail = 0;
// #define malloc(size) mock_malloc(size)
// #define calloc(n, size) mock_calloc(n, size)
// void *mock_malloc(const size_t size) {
//     if (alloc_fail) {
//         return NULL;
//     }
//     return malloc(size);
// }
//
// void *mock_calloc(const int n, const size_t size) {
//     if (alloc_fail) {
//         return NULL;
//     }
//     return calloc(n, size);
// }

static int make_random(RadixMap *map) {
    size_t i = 0;
    for (i = 0; i < map->max - 1; i++) {
        const uint32_t key = (uint32_t) (rand() | (rand() << 16));
        const int result = RadixMap_add(map, key, i);
        TEST_ASSERT_EQUAL_INT(result, 0);
    }
    return (int) i;
}

static int check_order(const RadixMap *map) {
    for (unsigned int i = 0; i < map->end - 1; i++) {
        const RMElement *el1 = &map->contents[i];
        const RMElement *el2 = &map->contents[i + 1];
        TEST_ASSERT_TRUE_MESSAGE(el1->data.key <= el2->data.key, "First element is not smaller than or equal to the second");
    }
    return 1;
}

void setUp(void) {
    map = RadixMap_create(MAX);
}

void tearDown(void) {
    RadixMap_destroy(map);
}

void test_create(void) {
    TEST_ASSERT_NOT_EQUAL(map, NULL);
    TEST_ASSERT_EQUAL_INT(map->max, MAX);
    TEST_ASSERT_EQUAL_INT(map->end, 0);
}

void test_sort(void) {
    // Set up
    make_random(map);

    // Act
    const int res = RadixMap_sort(map);

    // Verify
    check_order(map);
    TEST_ASSERT_EQUAL_INT(res, 0);
}

void test_search(void) {
    // Set up
    RadixMap_add(map, 1, 100);
    RadixMap_add(map, 2, 200);
    RadixMap_add(map, 3, 300);
    RadixMap_add(map, 4, 400);
    RadixMap_sort(map);

    // Act
    const RMElement *el1 = RadixMap_find(map, 1);
    const RMElement *el2 = RadixMap_find(map, 2);
    const RMElement *el3 = RadixMap_find(map, 3);
    const RMElement *el4 = RadixMap_find(map, 4);

    // Verify
    TEST_ASSERT_EQUAL_INT(el1->data.value, 100);
    TEST_ASSERT_EQUAL_INT(el2->data.value, 200);
    TEST_ASSERT_EQUAL_INT(el3->data.value, 300);
    TEST_ASSERT_EQUAL_INT(el4->data.value, 400);
    TEST_ASSERT_EQUAL_PTR(NULL, RadixMap_find(map, 10));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_sort);
    RUN_TEST(test_search);
    return UNITY_END();
}