#ifndef libfaafo_LIST_H
#define libfaafo_LIST_H

#include <arraylist.h>
#include <commons.h>
#include <stdbool.h>


#define LinkedList_size(L) ((L) ? (L)->size: 0)
#define LinkedList_is_empty(L) (LinkedList_size(L) == 0)
#define LinkedList_first(L) ((L)->first ? (L)->first->value : NULL)

#define LINKEDLIST_FOREACH(list, node) \
for (Node *node = (list)->first; node != NULL; node = node->next)

typedef struct ListNode {
    struct ListNode *next;
    void *value;
} Node;

typedef struct LinkedList {
    size_t size;
    Node *first;
    destructor_fn node_value_df;
} LinkedList;

/**
 * Allocate a new list
 * @param node_value_df The destructor function to use for each ListNode->value. If NULL, then noop will be used
 * @return a dynamically allocated List
 */
LinkedList *LinkedList_create(destructor_fn node_value_df);
LinkedList *LinkedList_from_ArrayList(ArrayList *array_list);
LinkedList *LinkedList_from_array(void *array[], unsigned int array_size, destructor_fn node_value_df);

bool LinkedList_push_all(LinkedList *list, void **data, unsigned int data_count);

bool LinkedList_push(LinkedList *list, void *value);

Node *LinkedList_find_node(const LinkedList *list, const void *value);

void *LinkedList_find_last(const LinkedList *list);

bool LinkedList_contains(const LinkedList *list, const void *value);

void *LinkedList_pop(LinkedList *list);

void *LinkedList_remove(LinkedList *list, Node *node_to_remove);

void LinkedList_destroy(LinkedList *list);

void LinkedList_clear(LinkedList *list);

#endif //libfaafo_LIST_H
