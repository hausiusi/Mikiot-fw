/*
 * utils.c
 *
 *  Created on: Apr 19, 2020
 *      Author: Zviad
 */
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
