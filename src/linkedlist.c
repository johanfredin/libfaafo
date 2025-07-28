#include "linkedlist.h"

#include <stdlib.h>
#include <string.h>

#include "dbg.h"

LinkedList *LinkedList_create(const destructor_fn node_value_df) {
    LinkedList *list = calloc(1, sizeof(LinkedList));
    check_return(list != NULL, "Failed to allocate memory for List", NULL);
    list->node_value_df = node_value_df ? node_value_df : NOOP_DESTRUCTOR;
    return list;
}

void LinkedList_destroy(LinkedList *const list) {
    check(list, "List is null", return);
    LinkedList_clear(list);
    free(list);
}

void LinkedList_clear(LinkedList *const list) {
    check(list, "List is null", return);
    Node *cur = list->first;
    while (cur != NULL) {
        Node *next = cur->next;
        list->node_value_df(cur->value);
        free(cur);
        cur = next;
    }
    list->size = 0;
}


bool LinkedList_push(LinkedList *const list, void *value) {
    Node *node = calloc(1, sizeof(Node));
    check_return(node != NULL, "Failed to allocate memory for new node", false);

    node->value = value;

    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
    list->size++;
    return true;
}

void *LinkedList_pop(LinkedList *const list) {
    check_return(list, "List is null", NULL);
    return LinkedList_remove(list, list->last);
}

void *LinkedList_remove(LinkedList *const list, Node *const node_to_remove) {
    check_return(list, "List is null", NULL);
    check_return(node_to_remove, "node can't be NULL", NULL);

    if (List_size(list) == 1) {
        // Only one node
        list->first = NULL;
        list->last = NULL;
    } else if (node_to_remove == list->first) {
        // Node is the first but there are more entries
        list->first = node_to_remove->next;
        check_return(list->first != NULL, "Invalid list, somehow got a first that is NULL", NULL);
    } else if (node_to_remove == list->last) {
        // Node is the last but there are more entries
        Node *prev = list->first;
        while (prev->next != node_to_remove) {
            prev = prev->next;
        }
        prev->next = NULL;
        list->last = prev;
    } else {
        // Node is in the middle
        Node *prev = list->first;
        while (prev->next != node_to_remove) {
            prev = prev->next;
        }
        prev->next = node_to_remove->next;
    }

    list->size--;
    void *removed_value = node_to_remove->value;
    list->node_value_df(removed_value);
    free(node_to_remove);
    return removed_value;
}
