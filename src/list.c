#include "list.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"

List *List_create() { return calloc(1, sizeof(List)); }

void List_destroy(List *list) {
    ListNode *cur = list->first;
    while (cur != NULL) {
        ListNode *next = cur->next;
        free(cur);
        cur = next;
    }
    // free(list->last);
    free(list);
}

void List_clear(const List *list) {
    LIST_FOREACH(list, first, next, cur) {
        free(cur->value);
    }
}

void List_clear_destroy(List *list) {
    List_clear(list);
    List_destroy(list);
}

void List_push(List *list, void *value) {
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        list->last = node;
    }

    list->count++;

error:
    // Fallthrough
}

void *List_pop(List *list) {
    ListNode *node = list->last;
    if (node != NULL) {
        return List_remove(list, node);
    }
    return NULL;
}

void *List_remove(List *list, ListNode *node_to_remove) {
    check(list != NULL, "List is null");
    void *result = NULL;

    check(list->first && list->last, "List is empty");
    check(node_to_remove, "node can't be NULL");

    if (node_to_remove == list->first && node_to_remove == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if (node_to_remove == list->first) {
        list->first = node_to_remove->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL");
    } else {
        // Handle removal of both last and middle nodes
        ListNode *prev = list->first;
        while(prev->next != node_to_remove) {
            prev = prev->next;
        }
        check(prev != NULL, "Invalid list, somehow got a node that is not in the list");

        prev->next = node_to_remove->next;  // will be NULL for last node case
        if (node_to_remove == list->last) {
            list->last = prev;
        }

    }

    list->count--;
    result = node_to_remove->value;
    free(node_to_remove);

error:
    return result;
}

List *List_copy(const List *list_to_copy, const size_t s) {
    List *copy = List_create();
    copy->count = 0;
    LIST_FOREACH(list_to_copy, first, next, cur) {
        void *copy_value = malloc(s);
        check_mem(copy_value);
        memcpy(copy_value, cur->value, s);
        List_push(copy, copy_value);
    }

    return copy;

error:
    log_err("Could not copy list");
    return NULL;
}

void List_join(List *base, List *additional) {
    check(base, "Base list is null");
    check(additional, "Additonal is null");

    base->last->next = additional->first;
    base->last = additional->last;
    base->count += additional->count;


error:
    // Fallthrough;

}
