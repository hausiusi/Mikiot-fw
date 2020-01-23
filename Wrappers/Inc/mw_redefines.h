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

#define mw_hal_init()			HAL_Init()

#define rtc_handle_t			RTC_HandleTypeDef
#define rtc_time_t				RTC_TimeTypeDef
#define rtc_date_t				RTC_DateTypeDef

#endif /* MW_REDEFINES_H_ */
