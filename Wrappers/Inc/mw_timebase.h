/*
 * mw_timebase.h
 *
 *  Created on: Apr 24, 2020
 *      Author: Zviad
 */

#ifndef INC_MW_TIMEBASE_H_
#define INC_MW_TIMEBASE_H_

#include <inttypes.h>

void mw_timebase_init(int frequency_hz);

uint32_t mw_timebase_ticks_get();

uint64_t mw_timebase_ticks_get_us();

#endif /* INC_MW_TIMEBASE_H_ */
