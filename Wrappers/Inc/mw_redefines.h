/*
 * w_redefines.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#ifndef MW_REDEFINES_H_
#define MW_REDEFINES_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"

#define mw_init()        HAL_Init()

typedef RTC_HandleTypeDef rtc_handle_t;
typedef RTC_TimeTypeDef rtc_time_t;
typedef RTC_DateTypeDef rtc_date_t;

#endif /* MW_REDEFINES_H_ */
