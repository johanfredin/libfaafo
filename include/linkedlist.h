#ifndef libfaafo_LIST_H
#define libfaafo_LIST_H

#define List_size(L) ((L)->size)
#define List_is_empty(L) (List_size(L) == 0)
#define List_first(L) ((L)->first != NULL ? (L)->first->value : NULL)
#define List_last(L) ((L)->last != NULL ? (L)->last->value : NULL)

#include <commons.h>
#include <stdbool.h>

typedef struct ListNode {
    struct ListNode *next;
    void *value;
} Node;

typedef struct LinkedList {
    int size;
    Node *first;
    Node *last;
    destructor_fn node_value_df;
} LinkedList;

/**
 * Allocate a new list
 * @param node_value_df The destructor function to use for each ListNode->value. If NULL, then noop will be used
 * @return a dynamically allocated List
 */
LinkedList *LinkedList_create(destructor_fn node_value_df);

void LinkedList_destroy(LinkedList *list);
void LinkedList_clear(LinkedList *list);
bool LinkedList_push_all(LinkedList *list, void **data, int data_count);
bool LinkedList_push(LinkedList *list, void *value);
bool LinkedList_contains(LinkedList *list, void *value);
void *LinkedList_pop(LinkedList *list);
void *LinkedList_remove(LinkedList *list, Node *node_to_remove);



#endif //libfaafo_LIST_H
