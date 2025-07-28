//
// Created by johan on 2025-07-21.
//

#ifndef COMMONS_H
#define COMMONS_H

#include <bstrlib.h>

#define NOOP_DESTRUCTOR noop_destroy

typedef void (*destructor_fn)(void *);

void bstring_destroy(void *b_string);

static inline void noop_destroy(void *ptr) {
    // Does nothing
    (void)ptr;
}

/**
 * Vanilla destroy function for a MapEntry. Assumes regular key/value types
 * and will just call regular free on both.
 * @param map_entry entry to free
 */
void map_entry_destroy(void *map_entry);

#endif //COMMONS_H
