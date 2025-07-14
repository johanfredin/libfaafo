//
// Created by johan on 2025-07-14.
//
#include "radixmap.h"
#include "dbg.h"
#include <stdlib.h>

#define ByteOf(x, y) (((uint8_t *)x)[y])

static void sort(short offset, uint64_t max, uint64_t *source, uint64_t *dest);

RadixMap *RadixMap_create(const size_t max) {
    check_return(max > 0, "Max must be > 0", NULL);
    RadixMap *map = calloc(1, sizeof(RadixMap));
    check_mem_return(map, NULL);

    map->contents = calloc(max, sizeof(RMElement));
    check_mem(map->contents, goto catch);

    map->temp = calloc(max, sizeof(RMElement));
    check_mem(map->contents, goto catch);

    map->max = max;
    map->end = 0;
    return map;
catch:
    log_err("Could not create a Map");
    RadixMap_destroy(map);
    return NULL;
}

void RadixMap_destroy(RadixMap *map) {
    check(map, "Map is null", return);
    free(map->contents);
    free(map->temp);
    free(map);
}

int RadixMap_sort(const RadixMap *map) {
    check_return(map, "Map is null", -1);

    uint64_t *source = &map->contents[0].raw;
    uint64_t *temp = &map->temp[0].raw;

    sort(0, map->end, source, temp);
    sort(1, map->end, temp, source);
    sort(2, map->end, source, temp);
    sort(3, map->end, temp, source);
    return 0;
}

RMElement *RadixMap_find(const RadixMap *map, const uint32_t key_to_find) {
    check_return(map, "Map is null", NULL);

    int low = 0;
    int high = (int) map->end - 1;
    RMElement *data = map->contents;

    while (low <= high) {
        const int middle = low + (high - low) / 2;
        const uint32_t key = data[middle].data.key;
        if (key_to_find < key) {
            high = middle - 1;
        } else if (key_to_find > key) {
            low = middle + 1;
        } else {
            return &data[middle];
        }
    }
    return NULL;
}

int RadixMap_add(RadixMap *map, const uint32_t key, const uint32_t value) {
    check_return(map, "Map is null", -1);
    check_return((map->end + 1) < map->max, "RadixMap is full", -1);

    const RMElement element = {
        .data = {
            .key = key,
            .value = value
        }
    };

    map->contents[map->end++] = element;
    return RadixMap_sort(map);
}

int RadixMap_delete(RadixMap *map, RMElement *el) {
    check_return(map, "Map is null", -1);
    check_return(el, "Element to delete is null", -1);

    el->data.key = UINT32_MAX;
    if (map->end > 1) {
        // Don't bother sorting if only 1 element in the map
        RadixMap_sort(map);
    }

    map->end--;
    return 0;
}

static void sort(const short offset, const uint64_t max, uint64_t *source, uint64_t *dest) {
    uint64_t count[256] = {0};
    uint64_t *cp = NULL;
    uint64_t *sp = NULL;
    const uint64_t *end = NULL;
    uint64_t s = 0;
    uint64_t c = 0;

    // count occurrences of every byte value
    for (sp = source, end = source + max; sp < end; sp++) {
        count[ByteOf(sp, offset)]++;
    }

    // transform count into index by summing elements and storing into the same array
    for (s = 0, cp = count, end = count + 256; cp < end; cp++) {
        c = *cp;
        *cp = s;
        s += c;
    }

    // fill dest with the right values in the right place
    for (sp = source, end = source + max; sp < end; sp++) {
        cp = count + ByteOf(sp, offset);
        dest[*cp] = *sp;
        ++(*cp);
    }
}
