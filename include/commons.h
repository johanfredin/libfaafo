//
// Created by johan on 2025-07-21.
//
#ifndef COMMONS_H
#define COMMONS_H

#include <stdbool.h>

#define NOOP Commons_noop


#if defined(__GNUC__) || defined(__clang__)
// GCC or Clang specific code
#define Commons_will_overflow(operand, multiplier, result) \
__builtin_mul_overflow((operand), multiplier, (result))
#else
// Generic implementation
static inline bool Commons_will_overflow(size_t a, size_t b, size_t *result) {
	if (a == 0 || b == 0) {
		*result = 0;
		return false;
	}

	size_t product = a * b;
	if (product / a != b) {
		return true; // Overflow occurred
	}

	*result = product;
	return false;
}

#endif


typedef void (*destructor_fn)(void *);
typedef bool (*equals_fn)(const void *a, const void *b);

void Commons_bstring_destroy(void *b_string);

void Commons_noop(void *ptr);

/**
 * Vanilla destroy function for a MapEntry. Assumes regular key/value types
 * and will just call regular free on both.
 * @param map_entry entry to free
 */
void Commons_map_entry_destroy(void *map_entry);

#endif //COMMONS_H
