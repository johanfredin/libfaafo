#ifndef libfaafo_LINKEDLIST_H
#define libfaafo_LINKEDLIST_H

/** 
 * @file linkedlist.h
 * @brief Generic singly linked list implementation
 * 
 * A generic singly linked list that only tracks the first node.
 * Supports custom destructor functions for memory management.
 */

#include <arraylist.h>
#include <commons.h>
#include <stdbool.h>

/** Get the size of a LinkedList, returns 0 if list is NULL */
#define LinkedList_size(L) ((L) ? (L)->size: 0)

/** Check if a LinkedList is empty (NULL or size 0) */
#define LinkedList_is_empty(L) (LinkedList_size(L) == 0)

/** Get the value of the first node, returns NULL if list or first node is NULL */
#define LinkedList_first(L) ((L)->first ? (L)->first->value : NULL)

/** Create a new LinkedList with free() as the default destructor */
#define LinkedList_new() (LinkedList_create(free))

/** 
 * Iterate through all nodes in a LinkedList
 * @param list The LinkedList to iterate
 * @param node Variable name for the current Node* in each iteration
 */
#define LINKEDLIST_FOREACH(list, node) \
for (Node *node = (list)->first; node != NULL; node = node->next)

/**
 * @brief A single node in the linked list
 */
typedef struct ListNode {
    struct ListNode *next;  /**< Pointer to the next node */
    void *value;           /**< Pointer to the node's data */
} Node;

/**
 * @brief A singly linked list structure
 */
typedef struct LinkedList {
    size_t size;                /**< Number of nodes in the list */
    Node *first;               /**< Pointer to the first node */
    Node *last;                /**< Pointer to the last node */
    destructor_fn node_value_df;  /**< Function to destroy node values */
} LinkedList;

/**
 * @brief Allocate a new LinkedList
 * @param node_value_df The destructor function for each node's value. 
 *                      If NULL, noop will be used.
 * @return A dynamically allocated LinkedList, or NULL on memory allocation failure
 * @note The caller is responsible for calling LinkedList_destroy()
 */
LinkedList *LinkedList_create(destructor_fn node_value_df);

/**
 * @brief Construct a LinkedList from an ArrayList
 * 
 * All values in the ArrayList are copied (deep copy). This function
 * assumes all elements in the ArrayList are the same type.
 * 
 * @param array_list The ArrayList to copy from. Must not be NULL.
 * @param element_size Size of each element (e.g., sizeof(int))
 * @param destroy_src If true, the source ArrayList is destroyed
 * @return A new LinkedList with copied data, or NULL on failure
 * @warning Will not work correctly if ArrayList contains different types
 */
LinkedList *LinkedList_from_ArrayList(ArrayList *array_list, size_t element_size, bool destroy_src) __nonnull((1));

/**
 * @brief Construct a LinkedList from a C array
 * @param array The source array. Must not be NULL.
 * @param n_elements Number of elements in the array
 * @param element_size Size of each element in bytes
 * @param node_value_df Destructor function for the copied elements
 * @return A new LinkedList with copied array data, or NULL on failure
 */
LinkedList *LinkedList_from_array(void *array, size_t n_elements, size_t element_size, destructor_fn node_value_df)
__nonnull((1));

/**
 * @brief Convert LinkedList to a C array
 * @param list The LinkedList to convert. Must not be NULL.
 * @param element_size Size of each element in bytes
 * @param destroy_src If true, the source LinkedList is destroyed
 * @return A newly allocated array containing copied list data, or NULL on failure
 * @note The caller is responsible for freeing the returned array
 */
void *LinkedList_to_array(LinkedList *list, size_t element_size, bool destroy_src) __nonnull((1));

/**
 * @brief Add an element to the end of the list
 * @param list The LinkedList to add to. Must not be NULL.
 * @param value The value to add. Must not be NULL.
 * @return true on success, false on failure
 */
bool LinkedList_push(LinkedList *list, void *value) __nonnull((1, 2));

/**
 * @brief Add multiple elements to the end of the list
 * @param list The LinkedList to add to. Must not be NULL.
 * @param data Array of pointers to add. Must not be NULL.
 * @param n_elements Number of elements to add
 * @return true on success, false on failure
 */
bool LinkedList_push_all(LinkedList *list, void **data, size_t n_elements) __nonnull((1, 2));

/**
 * @brief Find a node containing a specific value (by pointer comparison)
 * @param list The LinkedList to search. Must not be NULL.
 * @param value The value to search for. Must not be NULL.
 * @return Pointer to the Node containing the value, or NULL if not found
 * @warning Uses pointer comparison, not value comparison
 */
Node *LinkedList_find_node(const LinkedList *list, const void *value) __nonnull((1, 2));

/**
 * @brief Get the value of the last node
 * @param list The LinkedList to search. Must not be NULL.
 * @return Pointer to the last value, or NULL if list is empty
 * @note O(n) operation - traverses the entire list
 */
void *LinkedList_find_last(const LinkedList *list) __nonnull((1));

/**
 * @brief Check if the list contains a value (by pointer comparison)
 * @param list The LinkedList to search. Must not be NULL.
 * @param value The value to search for. Must not be NULL.
 * @return true if found, false otherwise
 * @warning Uses pointer comparison, not value comparison
 */
bool LinkedList_contains(const LinkedList *list, const void *value) __nonnull((1, 2));

/**
 * @brief Remove and return the last element
 * @param list The LinkedList to pop from. Must not be NULL.
 * @return true on success, false if list is empty or NULL
 * @note O(n) operation - traverses the entire list
 * @note The popped value is freed using the list's destructor function
 */
bool LinkedList_pop(LinkedList *list) __nonnull((1));

/**
 * @brief Remove a specific node from the list
 * @param list The LinkedList to remove from. Must not be NULL.
 * @param node_to_remove The node to remove. Must not be NULL.
 * @return true on success, false on failure
 * @note The removed value is freed using the list's destructor function
 * @note The node itself is also freed
 */
bool LinkedList_remove(LinkedList *list, Node *node_to_remove) __nonnull((1, 2));

/**
 * @brief Destroy the entire LinkedList
 * @param list The LinkedList to destroy. Must not be NULL.
 * @param is_destroy_node_values If true, call destructor on all values
 * @return true on success, false on failure
 */
bool LinkedList_destroy(LinkedList *list, bool is_destroy_node_values) __nonnull((1));

/**
 * @brief Clear all nodes from the list but keep the list structure
 * @param list The LinkedList to clear. Must not be NULL.
 * @param is_destroy_node_values If true, call destructor on all values
 * @return true on success, false on failure
 */
bool LinkedList_clear(LinkedList *list, bool is_destroy_node_values) __nonnull((1));

#endif //libfaafo_LINKEDLIST_H