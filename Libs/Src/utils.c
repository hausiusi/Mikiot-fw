/*
 * utils.c
 *
 *  Created on: Apr 19, 2020
 *      Author: Zviad
 */
#include <ctype.h>
#include <inttypes.h>
#include <string.h>
#include "utils.h"

int unumlen(uint32_t x) {
    if (x >= 100000) {
        if (x >= 10000000) {
            if (x >= 1000000000) {
                return 10;
            }
            if (x >= 100000000) {
                return 9;
            }
            return 8;
        }
        if (x >= 1000000) {
            return 7;
        }
        return 6;
    } else {
        if (x >= 1000) {
            if (x >= 10000) {
                return 5;
            }
            return 4;
        } else {
            if (x >= 100) {
                return 3;
            }
            if (x >= 10) {
                return 2;
            }
            return 1;
        }
    }
}

void* find(void* collection, void* item, uint32_t item_size, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        if (equals(collection, item, item_size)) {
            return collection;
        }
        collection += item_size;
    }
    return 0;
}

bool_t equals(void* item1, void* item2, uint32_t item_size) {
    for (uint32_t i = 0; i < item_size; i++) {
        if (*((uint8_t*) (item1 + i)) != *((uint8_t*) (item2 + i))) {
            return false;
        }
    }
    return true;
}

bool_t is_integer(char* str) {
    if (*str == '-') {
        str++;
    }
    while (*str && *str != ' ') {
        if (!isdigit(0xFF & *str++)) {
            return false;
        }
    }
    return true;
}

void strtrim(const char* restrict src, char* dest, uint32_t dest_max) {
    // Use one character less than buffer length to have place for the '\0' at the end
    dest_max -= 1;
    memset(dest, 0, dest_max);
    // Trim start
    char* start = (char*) src;
    char* end = (char*) src + (strlen(src));
    while (isspace(0xFF & *start)) {
        start++;
    }
    // Trim end
    while (!*end || isspace(0xFF & *end)) {
        end--;
    }
    for (uint32_t i = 0; (i < dest_max) && (start <= end); i++) {
        *dest++ = *start++;
    }
    if (start < end) {
        // End with three dots if text was longer than dest buffer
        *(dest - 3) = '.';
        *(dest - 2) = '.';
        *(dest - 1) = '.';
    }
}

bool_t strnullempty(const char* restrict str) {
    return (str == NULL) || (str[0] == 0);
}

int32_t map_int(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
        int32_t out_max) {
    if (x < in_min) {
        return out_min;
    }

    if (x > in_max) {
        return out_max;
    }

    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
