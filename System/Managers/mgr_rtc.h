/*
 * mgr_rtc.h
 *
 *  Created on: Apr 9, 2020
 *      Author: Zviad
 */

#ifndef MANAGERS_MGR_RTC_H_
#define MANAGERS_MGR_RTC_H_

#include <inttypes.h>
#include "mw_redefines.h"

rtc_handle_t mgr_rtc_handle;
rtc_time_t mgr_rtc_time;
rtc_date_t mgr_rtc_date;

void mgr_rtc_init(uint32_t hour_format);

void mgr_rtc_set(rtc_time_t* time, rtc_date_t* date);

rtc_time_t* mgr_rtc_get_time();

rtc_date_t* mgr_rtc_get_date();

void mgr_rtc_set_strf(const char* dt);

void mgr_rtc_print_date();

#endif /* MANAGERS_MGR_RTC_H_ */
