//
// Created by johan on 2025-07-21.
//

#ifndef COMMONS_H
#define COMMONS_H

#define NOOP Commons_noop

typedef void (*destructor_fn)(void *);

void Commons_bstring_destroy(void *b_string);

void Commons_noop(void *ptr);

/**
 * Vanilla destroy function for a MapEntry. Assumes regular key/value types
 * and will just call regular free on both.
 * @param map_entry entry to free
 */
void Commons_map_entry_destroy(void *map_entry);

#endif //COMMONS_H
