/*
 * mgr_rtc.c
 *
 *  Created on: Apr 9, 2020
 *      Author: Zviad
 */

#include "debug.h"
#include "mgr_rtc.h"
#include "mw_rtc.h"

void mgr_rtc_init(uint32_t hour_format) {
	mw_rtc_init(&mgr_rtc_handle, hour_format);
}

void mgr_rtc_set(rtc_time_t* time, rtc_date_t* date) {
	mw_rtc_set(&mgr_rtc_handle, time, date);
}

void mgr_rtc_print_date() {
	mw_rtc_get_time(&mgr_rtc_handle, &mgr_rtc_time);
	mw_rtc_get_date(&mgr_rtc_handle, &mgr_rtc_date);
	debug_p("Date: %i-%i-%i - Time: %i:%i:%i\n", mgr_rtc_date.Year,
			mgr_rtc_date.Month, mgr_rtc_date.Date, mgr_rtc_time.Hours,
			mgr_rtc_time.Minutes, mgr_rtc_time.Seconds);
}
