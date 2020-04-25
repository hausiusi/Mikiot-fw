/*
 * mgr_rtc.c
 *
 *  Created on: Apr 9, 2020
 *      Author: Zviad
 */

#include "debug.h"
#include "mgr_rtc.h"
#include "mw_rtc.h"
#include "macrodefs.h"
#include "math.h"

static const uint8_t days_in_a_month[] = {
		31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define ASSERT_DATE_ELEMENT(DATE_ELEMENT, from, to) { if (!is_range(DATE_ELEMENT, from, to)) { \
													debug_p("Argument '"#DATE_ELEMENT "' should be an integer from %i to %i\n", from, to); \
													return; }\
													}

static void _set_week_day(rtc_date_t* date);

void mgr_rtc_init(uint32_t hour_format) {
	mw_rtc_init(&mgr_rtc_handle, hour_format);
}

void mgr_rtc_set(rtc_time_t* time, rtc_date_t* date) {
	mw_rtc_set(&mgr_rtc_handle, time, date);
}

void mgr_rtc_set_strf(const char* dt) {
	int day, month, year, hour, minute, second;
	sscanf(dt, "%d-%d-%d %d:%d:%d", &day, &month, &year, &hour, &minute,
			&second);
	ASSERT_DATE_ELEMENT(month, 1, 12);
	ASSERT_DATE_ELEMENT(day, 1, days_in_a_month[month - 1]);
	ASSERT_DATE_ELEMENT(year, 0, 99);
	ASSERT_DATE_ELEMENT(hour, 0, 23);
	ASSERT_DATE_ELEMENT(minute, 0, 59);
	ASSERT_DATE_ELEMENT(second, 0, 59);

	mgr_rtc_date.Year = year;
	mgr_rtc_date.Month = month;
	mgr_rtc_date.Date = day;

	_set_week_day(&mgr_rtc_date);

	mgr_rtc_time.Hours = hour;
	mgr_rtc_time.Minutes = minute;
	mgr_rtc_time.Seconds = second;
	mgr_rtc_set(&mgr_rtc_time, &mgr_rtc_date);
}

void mgr_rtc_print_date() {
	mw_rtc_get_time(&mgr_rtc_handle, &mgr_rtc_time);
	mw_rtc_get_date(&mgr_rtc_handle, &mgr_rtc_date);
	debug_p("Date: %i-%i-%i - Time: %i:%i:%i\n", mgr_rtc_date.Date,
			mgr_rtc_date.Month, mgr_rtc_date.Year, mgr_rtc_time.Hours,
			mgr_rtc_time.Minutes, mgr_rtc_time.Seconds);
}

/* Weekday calculation with Zeller's Rule and set */
static void _set_week_day(rtc_date_t* date) {
	int m;
	int year = date->Year;
	if (date->Month == 1 || date->Month == 2) {
		year -= 1;
	}
	m = date->Month < 3 ? date->Month + 10 : date->Month - 2;
	int week_day = date->Date + (13 * m - 1) / 5 + year + year / 4 - 35;
	week_day %= 7;

	/* When the modulo is negative, we add 7 to get the correct weekday */
	if (week_day < 0) {
		week_day += 7;
	}
	date->WeekDay = week_day;
}
