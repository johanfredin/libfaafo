#include "arraylist.h"

#include <dbg.h>
#include <stdlib.h>

#define reset_size_and_capacity(list)               \
    (list)->size = 0;                               \
    (list)->capacity=ARRAYLIST_DEFAULT_CAPACITY;

static bool expand(ArrayList *list);

static bool resize(ArrayList *list, size_t new_capacity);


struct ArrayList {
    void **data;
    unsigned int size;
    unsigned int capacity;
};

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

bool ArrayList_add_all(ArrayList *const list, void **data, const unsigned int data_count) {
    check_return(list != NULL, "list is null", false);
    check_return(data != NULL, "data is null", false);
    check_return(data_count > 0, "data_count must be > 0", false);
    for (int i = 0; i < data_count; i++) {
        ArrayList_add(list, data[i]);
    }
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
    check_return(value != NULL, "value is null", NULL);
    check_return(list->data != NULL, "list->data is NULL", NULL);

    void *old_value = list->data[index];
    list->data[index] = value;
    return old_value;
}

void *ArrayList_get(const ArrayList *const list, const unsigned int index) {
    check_return(list != NULL, "list is null", NULL);
    check_return(index < list->size, "index %u out of bounds, current size=%u", NULL, index, list->size);
    return list->data[index];
}

void *ArrayList_remove(ArrayList *const list, const unsigned int index) {
    check_return(list != NULL, "list is null", NULL);
    check_return(index < list->size, "index %u out of bounds, current size=%u", NULL, index, list->size);
    check_return(list->data != NULL, "list->data is NULL", NULL);

    void *removed = list->data[index];

    /*
     * Shift all elements after removed index one position left.
     */
    const unsigned int last_index = list->size - 1;
    if (index < last_index) {
        memmove(
            &list->data[index],
            &list->data[index + 1],
            (list->size - index - 1) * sizeof(void *)
        );
    }

    list->size--;
    list->data[list->size] = NULL;

    return removed;
}

int ArrayList_sort(const ArrayList *list, const ArrayList_compare_fun compare_func) {
    check_return(list != NULL, "list is null", -1);
    check_return(compare_func != NULL, "compare_func is null", -1);
    qsort(list->data, list->size, sizeof(void *), compare_func);
    return 0;
}

bool ArrayList_destroy(ArrayList *const list) {
    check_return(list != NULL, "list is null", false);
    free(list->data);
    free(list);
    return true;
}

void **ArrayList_clear(ArrayList *const list, const destructor_fn df) {
    check_return(list != NULL, "list is null", NULL);
    if (df) {
        for (int i = 0; i < list->size; i++) {
            df(list->data[i]);
        }
        memset(list->data, 0, list->size * sizeof(void *));
        reset_size_and_capacity(list);
        return list->data;
    }

    /*
     * If we are not going to free the data, then give the caller original data
     * and allocate a new empty array for the list
     */
    void **old_data = list->data;
    list->data = calloc(list->capacity, sizeof(void *));
    check_return(list->data != NULL, "failed to allocate memory for values", NULL);
    reset_size_and_capacity(list);
    return old_data;
}

void **ArrayList_clear_destroy(ArrayList *const list, const destructor_fn df) {
    check_return(list != NULL, "list is null", NULL);
    void **result = ArrayList_clear(list, df);
    ArrayList_destroy(list);
    return result;
}

unsigned int ArrayList_size(const ArrayList *const list) {
    check_return(list != NULL, "list is null", 0);
    return list->size;
}

unsigned int ArrayList_capacity(const ArrayList *const list) {
    check_return(list != NULL, "list is null", 0);
    return list->capacity;
}

bool ArrayList_is_empty(const ArrayList *const list) {
    check_return(list != NULL, "list is null", false);
    return list->size == 0;
}

void *ArrayList_last(const ArrayList *const list) {
    check_return(list != NULL, "list is null", NULL);
    check_return(list->size > 0, "list is empty", NULL);
    return list->data[list->size - 1];
}

void *ArrayList_first(const ArrayList *const list) {
    check_return(list != NULL, "list is null", NULL);
    check_return(list->size > 0, "list is empty", NULL);
    return list->data[0];
}

int ArrayList_last_index(const ArrayList *const list) {
    check_return(list != NULL, "list is null", -1);
    check_return(list->size > 0, "list is empty", -1);
    return (int) list->size - 1;
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
