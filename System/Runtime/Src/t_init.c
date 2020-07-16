/*
 * t_init.c
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#include "mw_redefines.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mikiot_config.h"
#include "mw_clock.h"
#include "mw_timebase.h"
#include "mw_uart1.h"
#include "debug.h"
#include "mgr_rtc.h"
#include "error.h"

static void _rtc_init() {
	mw_timebase_init(1000);
	mgr_rtc_init(RTC_HOURFORMAT_24);
	/* Time */
	mgr_rtc_time.Hours = 2;
	mgr_rtc_time.Minutes = 10;
	mgr_rtc_time.Seconds = 40;
	mgr_rtc_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	mgr_rtc_time.StoreOperation = RTC_STOREOPERATION_RESET;

	/* Set date and time */
	mgr_rtc_date.WeekDay = RTC_WEEKDAY_WEDNESDAY;
	mgr_rtc_date.Month = RTC_MONTH_JANUARY;
	mgr_rtc_date.Date = 22;
	mgr_rtc_date.Year = 20;

	mgr_rtc_set(&mgr_rtc_time, &mgr_rtc_date);
}

extern void thread_init() {
	_rtc_init();
	for (;;) {
		vTaskDelay(10);
	}
}

#if (COMPILE_WITH_UNIT_TESTS == 1)
#include "test_framework.h"
void test_incorrect_blob();
#endif

/* Thread is created to test new features */
extern void thread_test() {
	for (int a = 0; a < 1000000; a++) {
		vTaskDelay(0);
	}
#if (COMPILE_WITH_UNIT_TESTS == 1)
	tf_run(test_incorrect_blob());
#endif
	for (;;) {
		vTaskDelay(1000);
	}
}
