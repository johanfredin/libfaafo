#include "linkedlist.h"

#include <stdlib.h>
#include <string.h>

#include "dbg.h"

LinkedList *LinkedList_create(const destructor_fn node_value_df) {
    LinkedList *list = calloc(1, sizeof(LinkedList));
    check_return(list != NULL, "Failed to allocate memory for List", NULL);
    list->node_value_df = node_value_df ? node_value_df : NOOP;
    return list;
}

LinkedList *LinkedList_from_ArrayList(ArrayList *const array_list, const size_t element_size, const bool destroy_src) {
    check_return(array_list, "ArrayList is null", NULL);
    LinkedList *list = LinkedList_create(ArrayList_get_df(array_list));
    check_return(list, "Failed to create LinkedList", NULL);
    for (size_t i = 0; i < ArrayList_size(array_list); i++) {

        /*
         * Always copy the content of each arraylist entry so we don't get shared pointers!
         */
        void *new_node = calloc(1, element_size);
        check_return(new_node, "Failed to allocate memory for new node", NULL);
        memcpy(new_node, ArrayList_get(array_list, i), element_size);
        LinkedList_push(list, new_node);
    }

    if (destroy_src) {
        ArrayList_destroy(array_list);
    }
    return list;
}

LinkedList *LinkedList_from_array(void *const array, const size_t n_elements, const size_t element_size,
                                  const destructor_fn node_value_df) {
    check_return(array, "Array is null", NULL);
    check_return(n_elements > 0, "Array size must be > 0", NULL);
    LinkedList *list = LinkedList_create(node_value_df);
    check(list, "Failed to create LinkedList", NULL);
    for (size_t i = 0, offset = (i * element_size);
         i < n_elements;
         i++, offset += element_size
    ) {
        void *new_node = calloc(1, element_size);
        check_mem(new_node, goto catch);

        const void *src = (char *) array + offset;
        memcpy(new_node, src, element_size);
        check(LinkedList_push(list, new_node), "Could not push array value to list", {
              node_value_df(new_node);
              free(new_node);
              goto catch;
              });
    }
    return list;
catch:
    LinkedList_destroy(list, true);
    return NULL;
}

void *LinkedList_to_array(LinkedList *const list, const size_t element_size, const bool destroy_src) {
    check_return(list, "List is null", NULL);
    check_return(element_size > 0, "Element size must be > 0", NULL);
    check_return(!LinkedList_is_empty(list), "List is empty", NULL);

    char *array = calloc(LinkedList_size(list), element_size);
    check_return(array, "Failed to allocate memory for array", NULL);
    size_t offset = 0;
    LINKEDLIST_FOREACH(list, node) {
        memcpy(array + offset, node->value, element_size);
        offset += element_size;
    }

    if (destroy_src) {
        LinkedList_destroy(list, true);
    }

    return array;
}

bool LinkedList_push(LinkedList *const list, void *value) {
    check_return(list, "List is null", false);
    check_return(value, "value is null", false);
    Node *new_node = calloc(1, sizeof(Node));
    check_mem_return(new_node, false);

    new_node->value = value;

    if (LinkedList_is_empty(list)) {
        list->first = new_node;
        list->last = new_node;
    } else {
        list->last->next = new_node;
        list->last = new_node;
    }

    list->size++;
    return true;
}

bool LinkedList_push_all(LinkedList *const list, void **data, const size_t n_elements) {
    check_return(list, "List is null", false);
    check_return(data != NULL, "values is null", false);
    check_return(n_elements > 0, "values_count must be > 0", false);
    for (size_t i = 0; i < n_elements; i++) {
        check(LinkedList_push(list, data[i]), "Failed to push value", false);
    }
    return true;
}

Node *LinkedList_find_node(const LinkedList *const list, const void *const value, const equals_fn ef) {
    check_return(list, "List is null", NULL);
    check_return(value != NULL, "value is null", NULL);
    LINKEDLIST_FOREACH(list, node) {
        if (ef(node->value, value)) {
            return node;
        }
    }
    log_warn("Node not found in list");
    return NULL;
}

bool LinkedList_pop(LinkedList *const list) {
    check_return(list, "List is null", false);
    return LinkedList_remove(list, list->last);
}

bool LinkedList_contains(const LinkedList *const list, const void *const value, const equals_fn ef) {
    return LinkedList_find_node(list, value, ef);
}

bool LinkedList_remove(LinkedList *list, Node *node_to_remove) {
    check_return(list, "List is null", false);
    check_return(node_to_remove, "Node can't be NULL", false);

    void *removed = node_to_remove->value;

    // Find the pointer that points to our node
    Node **current = &list->first;
    while (*current && *current != node_to_remove) {
        current = &(*current)->next;
    }

    // Remove the node
    *current = node_to_remove->next;

    // Handle update of list->last
    if (node_to_remove == list->last) {
        if (LinkedList_size(list) == 1) {
            // List becomes empty, first node already NULL from before
            list->last = NULL;
        } else {
           // Find new last node
            Node *new_last = list->first;
            while (new_last->next) {
                new_last = new_last->next;
            }
            list->last = new_last;
        }

    }

    list->size--;
    list->node_value_df(removed); // Free the value using provided destroy function
    free(node_to_remove); // Free the node itself
    return true;
}

bool LinkedList_destroy(LinkedList *const list, const bool is_destroy_node_values) {
    check_return(list, "List is null", false);
    LinkedList_clear(list, is_destroy_node_values);
    free(list);
    return true;
}

bool LinkedList_clear(LinkedList *const list, const bool is_destroy_node_values) {
    check_return(list, "List is null", false);
    Node *current = list->first;
    while (current) {
        Node *next = current->next;
        if (is_destroy_node_values) {
            list->node_value_df(current->value);
        }
        free(current);
        current = next;
    }
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return true;
}

