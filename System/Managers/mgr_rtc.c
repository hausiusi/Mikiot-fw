/*
 * mgr_rtc.c
 *
 *  Created on: Apr 9, 2020
 *      Author: Zviad
 */

#include "mgr_rtc.h"
#include "mw_rtc.h"
#include "macrodefs.h"
#include "math.h"

rtc_handle_t mgr_rtc_handle;

static void _set_week_day(rtc_date_t* date);

void mgr_rtc_init(uint32_t hour_format) {
    mw_rtc_init(&mgr_rtc_handle, hour_format);
}

void mgr_rtc_set(rtc_time_t* time, rtc_date_t* date) {
    _set_week_day(date);
    mw_rtc_set(&mgr_rtc_handle, time, date);
}

void mgr_rtc_get_time(rtc_time_t* rtc_time) {
    mw_rtc_get_time(&mgr_rtc_handle, rtc_time);
}

void mgr_rtc_get_date(rtc_date_t* rtc_date) {
    mw_rtc_get_date(&mgr_rtc_handle, rtc_date);
}

uint8_t mgr_rtc_get_max_days_in_month(uint8_t month, uint8_t year) {
    static const uint8_t days_in_a_month[] = {
            31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    month -= 1;
    return (month == 1) && (year % 4 != 0) ?
            days_in_a_month[month] - 1 : days_in_a_month[month];
}

void mgr_rtc_set_strf(const char* dt) {
    int day, month, year, hour, minute, second;
    rtc_date_t rtc_date = { 0 };
    rtc_time_t rtc_time = { 0 };
    sscanf(dt, "%d-%d-%d %d:%d:%d", &day, &month, &year, &hour, &minute,
            &second);
    ASSERT_DATE_ELEMENT(month, 1, 12);
    ASSERT_DATE_ELEMENT(year, 0, 99);
    ASSERT_DATE_ELEMENT(day, 1, mgr_rtc_get_max_days_in_month(month, year));
    ASSERT_DATE_ELEMENT(hour, 0, 23);
    ASSERT_DATE_ELEMENT(minute, 0, 59);
    ASSERT_DATE_ELEMENT(second, 0, 59);

    rtc_date.Year = year;
    rtc_date.Month = month;
    rtc_date.Date = day;

    rtc_time.Hours = hour;
    rtc_time.Minutes = minute;
    rtc_time.Seconds = second;
    mgr_rtc_set(&rtc_time, &rtc_date);
}

void mgr_rtc_print_date() {
    rtc_date_t rtc_date = { 0 };
    rtc_time_t rtc_time = { 0 };
    mw_rtc_get_time(&mgr_rtc_handle, &rtc_time);
    mw_rtc_get_date(&mgr_rtc_handle, &rtc_date);
    static const char* weekday_map[] = {
            "N/A", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
            "Saturday", "Sunday" };
    debug_p("Date: %i-%i-%i - Time: %i:%i:%i Weekday: %s\n", rtc_date.Date,
            rtc_date.Month, rtc_date.Year, rtc_time.Hours, rtc_time.Minutes,
            rtc_time.Seconds, weekday_map[rtc_date.WeekDay]);
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
