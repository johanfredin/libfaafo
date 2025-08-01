//
// Created by johan on 2025-07-21.
//
#include "commons.h"

#include <assert.h>
#include <hashmap.h>
#include <stdlib.h>
#include <bstrlib.h>


inline void Commons_noop(void *ptr) {
    // Does nothing
    (void)ptr;
}

void Commons_bstring_destroy(void *b_string) {
    assert(bdestroy(b_string) == BSTR_OK);
}

void Commons_map_entry_destroy(void *map_entry) {
    MapEntry *entry = map_entry;
    free(entry->key);
    free(entry->value);
    free(entry);
}
