#ifndef libfaafo_RADIXMAP_H
#define libfaafo_RADIXMAP_H

#include <stddef.h>
#include <stdint.h>

typedef union RMElement {
    uint64_t raw;

    struct {
        uint32_t key;
        uint32_t value;
    } data;
} RMElement;

typedef struct RadixMap {
    size_t max;
    size_t end;
    uint32_t counter;
    RMElement *contents;
    RMElement *temp;
} RadixMap;

RadixMap *RadixMap_create(size_t max);

void RadixMap_destroy(RadixMap *map) __nonnull((1));

int RadixMap_sort(const RadixMap *map) __nonnull((1));

RMElement *RadixMap_find(const RadixMap *map, uint32_t key_to_find) __nonnull((1));

int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value) __nonnull((1));

int RadixMap_delete(RadixMap *map, RMElement *el) __nonnull((1, 2));

#endif //libfaafo_RADIXMAP_H
