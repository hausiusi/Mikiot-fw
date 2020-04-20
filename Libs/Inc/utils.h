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

#endif /* INC_UTILS_H_ */
