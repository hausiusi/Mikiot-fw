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
#include "mw_rtc.h"

uint32_t tmp, sysclk;
rtc_handle_t hrtc;
rtc_time_t sTime = { 0 };
rtc_date_t sDate = { 0 };

void Error_Handler(void) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}

extern void thread_init() {
	for (int a = 0; a < 1000000; a++) {
		asm("NOP");
	}
	for (;;) {
		vTaskDelay(10);
	}
}

static void rtc_init(rtc_handle_t* handl) {
	mw_rtc_init(handl, RTC_HOURFORMAT_24, Error_Handler);
}

extern void thread_test() {
	for (int a = 0; a < 1000000; a++) {
		asm("NOP");
	}
	//int mhz = 30;
	rtc_init(&hrtc);
	/*  Time */
	sTime.Hours = 2;
	sTime.Minutes = 10;
	sTime.Seconds = 40;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	/* Set date and time */
	sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
	sDate.Month = RTC_MONTH_JANUARY;
	sDate.Date = 22;
	sDate.Year = 20;
	mw_rtc_set(&hrtc, &sTime, &sDate, Error_Handler);
	/**/

	for (;;) {
		//		tmp = HAL_RCC_GetPCLK2Freq();
		//		sysclk = HAL_RCC_GetSysClockFreq();
		//		mw_internal_clock_init();
		//		mw_external_clock_init(mhz--);
		//		mw_uart1_init(9600, uart_idle_detected);
		//		debug_p("123456789\n");

		mw_rtc_get_time(&hrtc, &sTime);
		mw_rtc_get_date(&hrtc, &sDate);
		/* USER CODE BEGIN 3 */
		vTaskDelay(1000);
	}
}
