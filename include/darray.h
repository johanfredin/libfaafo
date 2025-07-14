/**
 * @file darray.h
 * @brief Dynamic Array (DArray) - A resizable array implementation for void pointers
 *
 * DArray is a container for void pointers that automatically resizes as elements
 * are added. By default, DArray does not take ownership of the memory pointed to by
 * stored pointers – memory management of pointed-to objects is the caller's responsibility.
 * However, if you use DArray_clear or DArray_clear_destroy, these will call free() on
 * each non-NULL contained pointer. Only use these functions if all stored elements were
 * dynamically allocated.
 *
 * Removal or clearing does not shift or compact elements; removed entries are set to NULL,
 * and clearing frees all elements and sets size to zero.
 *
 * @note All stored elements are void* pointers. The element_size field is
 *       informational only and not used for memory operations.
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

/** @brief Default minimum capacity for new DArrays */
#define DARRAY_MINIMUM_CAPACITY 10

/**
 * @brief Check if the array is empty
 * @param A Pointer to DArray
 * @return Non-zero if empty, 0 if not empty
 */
#define DArray_is_empty(A) ((A)->size == 0)

/**
 * @brief Get the first element in the array
 * @param A Pointer to DArray  
 * @return void* to first element, or NULL if array is empty/on error
 */
#define DArray_first(A) DArray_get((A), 0)

/**
 * @brief Get the index of the last element
 * @param A Pointer to DArray
 * @return Index of last element, or undefined if array is empty
 */
#define DArray_last(A) ((A)->size > 0 ? DArray_get((A), (A)->size - 1) : NULL)


/**
 * @brief Get the number of elements in the array
 * @param A Pointer to DArray
 * @return Number of elements currently stored
 */
#define DArray_size(A) ((A)->size)

/**
 * @brief Get the current capacity of the array
 * @param A Pointer to DArray
 * @return Maximum number of elements that can be stored without reallocation
 */
#define DArray_cap(A) ((A)->capacity)

/**
 * @brief Create a new DArray with minimum capacity
 * @return Pointer to new DArray with DARRAY_MINIMUM_CAPACITY
 */
#define DArray_new() (DArray_create(DARRAY_MINIMUM_CAPACITY))

/**
 * @brief Dynamic Array structure
 * 
 * Stores an array of void* pointers that can grow as needed.
 * The actual objects pointed to are managed by the caller.
 */
typedef struct DArray {
    unsigned int size; /**< Current number of elements */
    unsigned int capacity; /**< Maximum elements before reallocation needed */
    void **contents; /**< Array of void* pointers to the actual data */
} DArray;

typedef int (*DArray_compare_func)(const void *a, const void *b);

int DArray_sort(DArray *array, DArray_compare_func compare_func) __nonnull((1));

/**
 * @brief Create a new dynamic array
 * @param initial_capacity Initial capacity (must be > 0)
 * @return Pointer to new DArray, or NULL on failure
 * @note The element_size is stored but not used for memory operations
 */
DArray *DArray_create(unsigned int initial_capacity);

/**
 * @brief Add an element to the end of the array
 * @param array Pointer to DArray
 * @param value Pointer to add (can be NULL)
 * @return 0 on success, -1 on failure (e.g., memory allocation failure)
 * @note Array will automatically expand if needed
 */
int DArray_add(DArray *array, void *value) __nonnull((1));

/**
 * @brief Set element at specific index, returning the old value
 * @param array Pointer to DArray
 * @param index Index to set (must be < size)
 * @param value New pointer value to store
 * @return Previous pointer value at that index, or NULL on error
 * @note Caller is responsible for freeing the returned old value if needed
 * @note Bounds checking is performed - invalid indices are handled gracefully
 * @note Setting an index does not alter the array size except if (index > size) (should not happen in normal use)
 */
void *DArray_set(DArray *array, unsigned int index, void *value)  __nonnull((1, 3));

/**
 * @brief Get element at specific index
 * @param array Pointer to DArray (const)
 * @param index Index to retrieve (must be < size)
 * @return Pointer at the specified index, or NULL on error
 * @note Bounds checking is performed - invalid indices return NULL
 */
void *DArray_get(const DArray *array, unsigned int index) __nonnull((1));

/**
 * @brief Remove and return element at specific index
 * @param array Pointer to DArray
 * @param index Index to remove (must be < size)
 * @return Pointer that was removed, or NULL on error
 * @note The element at the given index is set to NULL; elements after the removed index
 *       are NOT shifted down. Array size is NOT updated by this operation.
 * @note Caller is responsible for freeing the returned pointer if needed.
 * @note Bounds checking is performed - invalid indices are handled gracefully.
 */
void *DArray_remove(const DArray *array, unsigned int index) __nonnull((1));

/**
 * @brief Shrink array capacity to match current size
 * @param array Pointer to DArray
 * @return 0 on success, -1 on failure
 * @note Reduces memory usage but may cause reallocation on next add
 */
int DArray_trim_to_size(DArray *array) __nonnull((1));

/**
 * @brief Destroy the array structure only
 * @param array Pointer to DArray to destroy
 * @note Does NOT free the objects pointed to by the array elements
 * @note After calling this, the array pointer is invalid
 * @warning Memory leak if you don't free the contained objects first
 */
void DArray_destroy(DArray *array);

/**
 * @brief Clear all elements from array (set size to 0)
 * @param array Pointer to DArray
 * @note Frees the objects pointed to by the array elements (calls free() on each non-NULL element)
 * @note Capacity remains unchanged
 * @warning Only use if ALL elements were dynamically allocated with malloc/calloc.
 *          Do NOT use if the elements are stack variables, literals, or shared pointers.
 */
void DArray_clear(DArray *array);

/**
 * @brief Clear array and free all contained objects, then destroy array
 * @param array Pointer to DArray
 * @note Calls free() on each non-NULL element, then destroys the array
 * @note Use this when all elements were allocated with malloc/calloc
 * @warning Only use if ALL elements were dynamically allocated.
 */
void DArray_clear_destroy(DArray *array);

#endif //ARRAY_H
