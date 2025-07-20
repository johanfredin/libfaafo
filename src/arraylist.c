#include "arraylist.h"

#include <dbg.h>
#include <stdlib.h>

static bool expand(ArrayList *list);
static bool resize(ArrayList *list, size_t new_capacity);

ArrayList *ArrayList_create(const unsigned int capacity) {
    check_return(capacity > 0, "initial capacity must be > 0", NULL);
    ArrayList *list = calloc(1, sizeof(ArrayList));
    check_return(list != NULL, "failed to allocate memory for list", NULL);

    // Reserve space for the "capacity" number of void pointers
    void **data = calloc(capacity, sizeof(void *));
    check(data != NULL, "failed to allocate memory for values", goto catch);
    list->data = data;
    list->capacity = capacity;
    list->size = 0;
    return list;
catch:
    ArrayList_destroy(list);
    return NULL;
}

bool ArrayList_add(ArrayList *const list, void *const value) {
    check_return(list != NULL, "list is null", 0);
    check_return(value != NULL, "value is null", 0);
    if (list->size >= list->capacity) {
        const bool is_expanded = expand(list);
        check_return(is_expanded, "failed to expand list", false);
    }
    list->data[list->size++] = value;
    return true;
}

void *ArrayList_get(const ArrayList *const list, const unsigned int index) {
    check_return(list != NULL, "list is null", NULL);
    check_return(index < list->size, "index %u out of bounds, current size=%u", NULL, index, list->size);
    return list->data[index];
}

bool ArrayList_destroy(ArrayList *const list) {
    check_return(list != NULL, "list is null", false);
    free(list->data);
    free(list);
    return true;
}

bool ArrayList_contains(const ArrayList *const list, const void *const value) {
    return ArrayList_index_of(list, value) >= 0;
}

bool ArrayList_contains_all(const ArrayList *const list, void **data, const unsigned int data_count) {
    check_return(list != NULL, "list is null", false);
    for (int i = 0; i < data_count; i++) {
        if (!ArrayList_contains(list, data[i])) {
            return false;
        }
    }
    return true;
}

int ArrayList_index_of(const ArrayList *const list, const void *const value) {
    check_return(list != NULL, "list is null", -1);
    check_return(value != NULL, "value is null", -1);

    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            return i;
        }
    }
    return -1;
}

void *ArrayList_set(const ArrayList *const list, const unsigned int index, void *value) {
    check_return(list != NULL, "list is null", NULL);
    check_return(index < list->size, "index %u out of bounds, current size=%u", NULL, index, list->size);
    void *old_value = list->data[index];
    list->data[index] = value;
    return old_value;
}

static bool expand(ArrayList *const list) {
    check_return(list != NULL, "list is null", false);
    const unsigned int curr_cap = list->capacity;
    unsigned int new_cap = curr_cap + (curr_cap >> 1); // multiply by 1.5
    if (new_cap <= curr_cap) {
        new_cap = curr_cap + 1; // if overflow, just add 1
    }
    const bool resized = resize(list, new_cap);
    check_return(resized, "failed to resize list", false);

    memset(
        list->data + curr_cap,
        0,
        (new_cap - curr_cap) * sizeof(void *)
    );

    return true;
}

static bool resize(ArrayList *const list, const size_t new_capacity) {
    check_return(list, "Can not resize a NULL list", false);
    check_return(new_capacity > 0, "new_size must be > 0", false);

    void *data = realloc(list->data, new_capacity * sizeof(void *));
    check_mem_return(data, -1);
    list->data = data;
    list->capacity = new_capacity;
    return true;
}
