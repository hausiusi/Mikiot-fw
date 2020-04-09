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
#include "mw_uart1.h"
#include "mw_clock.h"
#include "debug.h"
#include "mgr_rtc.h"
#include "error.h"

static void _rtc_init() {
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

/* Thread is created to test new features */
extern void thread_test() {
	for (int a = 0; a < 1000000; a++) {
		asm("NOP");
	}
	//int mhz = 30;

	/**/

	for (;;) {
		//		tmp = HAL_RCC_GetPCLK2Freq();
		//		sysclk = HAL_RCC_GetSysClockFreq();
		//		mw_internal_clock_init();
		//		mw_external_clock_init(mhz--);
		//		mw_uart1_init(9600, uart_idle_detected);

		vTaskDelay(1000);
	}
}
