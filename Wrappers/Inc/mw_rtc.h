/*
 * mw_rtc.h
 *
 *  Created on: Jan 19, 2020
 *      Author: saturn
 */

#ifndef INC_MW_RTC_H_
#define INC_MW_RTC_H_

#include "mw_redefines.h"
#include "inttypes.h"

void mw_rtc_init(rtc_handle_t* hrtc, const uint32_t* hour_format,
		void (*handler)());
rtc_date_t* mw_rtc_get_date(rtc_handle_t* hrtc, rtc_date_t* date);
rtc_time_t* mw_rtc_get_time(rtc_handle_t* hrtc, rtc_time_t* time);
void mw_rtc_set(rtc_handle_t* hrtc, rtc_time_t* time, rtc_date_t* date,
		void (*handler)());

#endif /* INC_MW_RTC_H_ */
