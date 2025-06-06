#include "list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

List *List_create() {
    List *list = calloc(1, sizeof(List));
    check_mem_return_null(list);
    return list;
}

void List_destroy(List *list) {
    check_return(list, "List is null");
    ListNode *cur = list->first;
    while (cur != NULL) {
        ListNode *next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}

void List_clear(const List *list) {
    check_return(list, "List is null");
    const ListNode *cur = list->first;
    while (cur != NULL) {
        const ListNode *next = cur->next;
        free(cur->value);
        cur = next;
    }
}

void List_clear_destroy(List *list) {
    check_return(list, "List is null");
    List_clear(list);
    List_destroy(list);
}

int List_push(List *list, void *value) {
    ListNode *node = calloc(1, sizeof(ListNode));
    try(node != NULL, "Failed to allocate memory for new node");

    node->value = value;

    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }
    list->count++;
    return 1;
catch:
    return 0;
}

void *List_pop(List *list) {
    check_return_null(list, "List is null");
    ListNode *node = list->last;
    if (node != NULL) {
        return List_remove(list, node);
    }
    return NULL;
}

void *List_remove(List *list, ListNode *node_to_remove) {
    try(list != NULL, "List is null");
    void *result = NULL;

    try(list->first && list->last, "List is empty");
    try(node_to_remove, "node can't be NULL");

    if (node_to_remove == list->first && node_to_remove == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node_to_remove == list->first) {
        list->first = node_to_remove->next;
        try(list->first != NULL, "Invalid list, somehow got a first that is NULL");
    } else {
        // Handle removal of both last and middle nodes
        ListNode *prev = list->first;
        while (prev->next != node_to_remove) {
            prev = prev->next;
        }
        try(prev != NULL, "Invalid list, somehow got a node that is not in the list");

        prev->next = node_to_remove->next; // will be NULL for last node case
        if (node_to_remove == list->last) {
            list->last = prev;
        }
    }

    list->count--;
    result = node_to_remove->value;
    free(node_to_remove);
    return result;
catch:
    log_err("Errors occurred while removing node from list");
    return NULL;
}
