/*
 * utils.h
 *
 *  Created on: Apr 19, 2020
 *      Author: Zviad
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <inttypes.h>
#include "defines.h"

/*
 * Gets length of unsigned number up to 10 digits
 * @params x: unsigned number
 *
 * @returns: length
 *  */
int unumlen(uint32_t x);

void* find(void* collection, void* item, uint32_t item_size, uint32_t count);

bool_t equals(void* item1, void* item2, uint32_t item_size);

bool_t is_integer(char* str);

void strtrim(const char* src, char* dest, uint32_t dest_max);

bool_t strnullempty(const char* restrict str);

inline uint8_t singlebitpos(uint16_t n) {
    uint8_t ch = n & 0xFF;
    uint8_t shift = 0;
    if (!ch) {
        ch = (uint8_t) ((n & 0xFF00) >> 8);
        shift = 8;
    }

    uint8_t position = (0xFAC688 / ch / ch / ch) & 0x7;
    return position + shift;
}

int32_t map_int(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
        int32_t out_max);

#endif /* INC_UTILS_H_ */
