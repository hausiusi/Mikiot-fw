/*
 * mw_rtc.c
 *
 *  Created on: Jan 19, 2020
 *      Author: saturn
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "mw_redefines.h"
#include "inttypes.h"
#include "error.h"

void mw_rtc_init(rtc_handle_t* hrtc, const uint32_t hour_format) {

    /* Initialize RTC Only */
    hrtc->Instance = RTC;
    hrtc->Init.HourFormat = hour_format;
    hrtc->Init.AsynchPrediv = 127;
    hrtc->Init.SynchPrediv = 255;
    hrtc->Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc->Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc->Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    __HAL_RCC_RTC_ENABLE();
    /* While loop until RTC interrupt */
    if (HAL_RTC_Init(hrtc) != HAL_OK) {
        error_report(0, InitError);
    }

}

rtc_date_t* mw_rtc_get_date(rtc_handle_t* hrtc, rtc_date_t* date) {
    /* Get the RTC current Date */
    HAL_RTC_GetDate(hrtc, date, FORMAT_BIN);
    return date;
}

rtc_time_t* mw_rtc_get_time(rtc_handle_t* hrtc, rtc_time_t* time) {
    /* Get the RTC current Time */
    HAL_RTC_GetTime(hrtc, time, FORMAT_BIN);
    return time;
}

void mw_rtc_set(rtc_handle_t* hrtc, rtc_time_t* time, rtc_date_t* date) {
    /* Initialize RTC and set the Time and Date */
    if (HAL_RTC_SetTime(hrtc, time, RTC_FORMAT_BIN) != HAL_OK) {
        error_report(1, SetupError);
    }
    if (HAL_RTC_SetDate(hrtc, date, RTC_FORMAT_BIN) != HAL_OK) {
        error_report(2, SetupError);
    }
}
