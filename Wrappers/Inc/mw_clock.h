/*
 * mw_clock.h
 *
 *  Created on: Jan 18, 2020
 *      Author: Zviad
 */

#ifndef INC_MW_CLOCK_H_
#define INC_MW_CLOCK_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"

void mw_internal_clock_init();

void mw_external_clock_init(uint32_t clock_mhz);

#endif /* INC_MW_CLOCK_H_ */
