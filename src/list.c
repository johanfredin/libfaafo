#include "list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

List *List_create() {
    return calloc(1, sizeof(List));
}

void List_destroy(List *list) {
    check(list, "List is null", return);
    ListNode *cur = list->first;
    while (cur != NULL) {
        ListNode *next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}

void List_clear(const List *list) {
    check(list, "List is null", return);
    const ListNode *cur = list->first;
    while (cur != NULL) {
        const ListNode *next = cur->next;
        free(cur->value);
        cur = next;
    }
}

void List_clear_destroy(List *list) {
    check(list, "List is null", return);
    List_clear(list);
    List_destroy(list);
}

int List_push(List *list, void *value) {
    ListNode *node = calloc(1, sizeof(ListNode));
    check_return(node != NULL, "Failed to allocate memory for new node", 0);

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
}

void *List_pop(List *list) {
    check_return(list, "List is null", NULL);
    ListNode *node = list->last;
    if (node != NULL) {
        return List_remove(list, node);
    }
    return NULL;
}

void *List_remove(List *list, ListNode *node_to_remove) {
    check_return(list != NULL, "List is null", NULL);
    check_return(list->first && list->last, "List is empty", NULL);
    check_return(node_to_remove, "node can't be NULL", NULL);

    void *result = node_to_remove->value;

    if (node_to_remove == list->first && node_to_remove == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node_to_remove == list->first) {
        list->first = node_to_remove->next;
        check_return(list->first != NULL, "Invalid list, somehow got a first that is NULL", NULL);
    } else {
        ListNode *prev = list->first;
        while (prev->next != node_to_remove) {
            prev = prev->next;
            if (prev == NULL) {
                log_err("Invalid list, somehow got a node that is not in the list");
                return NULL;
            }
        }

        prev->next = node_to_remove->next;
        if (node_to_remove == list->last) {
            list->last = prev;
        }
    }

    list->count--;
    free(node_to_remove);
    return result;
}
