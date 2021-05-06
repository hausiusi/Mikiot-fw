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
#include "debug.h"

#define ASSERT_DATE_ELEMENT(DATE_ELEMENT, from, to) { if (!is_range(DATE_ELEMENT, from, to)) { \
                                                    debug_p("Argument '"#DATE_ELEMENT "' should be an integer from %i to %i\n", from, to); \
                                                    return; }\
                                                    }

rtc_handle_t mgr_rtc_handle;

void mgr_rtc_init(uint32_t hour_format);

void mgr_rtc_set(rtc_time_t* time, rtc_date_t* date);

void mgr_rtc_get_time(rtc_time_t* rtc_time);

void mgr_rtc_get_date(rtc_date_t* rtc_date);

void mgr_rtc_set_strf(const char* dt);

uint8_t mgr_rtc_get_max_days_in_month(uint8_t month, uint8_t year);

void mgr_rtc_print_date();

#endif /* MANAGERS_MGR_RTC_H_ */
