/*
 * t_init.c
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#include <mw_uart.h>
#include "mw_redefines.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mikiot_config.h"
#include "mw_clock.h"
#include "mw_timebase.h"
#include "debug.h"
#include "mgr_rtc.h"
#include "error.h"

#if COMPILE_WITH_UNIT_TESTS == 1
#include "Tests/test_framework.h"
tf_result tf_current_results, tf_total_results;
#endif

static void _rtc_init() {
#if (MCONF_RTC_ON == 1)
    mgr_rtc_init(MCONF_RTC_TIME_FORMAT);
    /* Time */
    rtc_date_t rtc_date = { 0 };
    rtc_time_t rtc_time = { 0 };
    rtc_time.Hours = 2;
    rtc_time.Minutes = 10;
    rtc_time.Seconds = 40;
    rtc_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    rtc_time.StoreOperation = RTC_STOREOPERATION_RESET;

    /* Set date and time */
    rtc_date.Month = RTC_MONTH_JANUARY;
    rtc_date.Date = 22;
    rtc_date.Year = 20;
    mgr_rtc_set(&rtc_time, &rtc_date);
#endif
}

extern void thread_init(void* args) {
    mw_timebase_init(1000);
    _rtc_init();
    for (;;) {
        vTaskDelay(10);
    }
}

#if (COMPILE_WITH_UNIT_TESTS == 1)
#include "test_framework.h"
void test_incorrect_blob();
void test_ringbuffer();
#endif

/* Thread is created to test new features */
extern void thread_test() {
    for (int a = 0; a < 1000000; a++) {
        vTaskDelay(0);
    }
#if (COMPILE_WITH_UNIT_TESTS == 1)
    tf_run(test_ringbuffer());
    tf_run(test_incorrect_blob());
#endif
    for (;;) {
        vTaskDelay(1000);
    }
}
