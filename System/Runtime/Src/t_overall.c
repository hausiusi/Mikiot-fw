/*
 * t_monitor.c
 *
 *  Created on: May 31, 2020
 *      Author: Zviad
 */

#include <inttypes.h>
#include "overall_state.h"

#include "mgr_rtc.h"
#include "mgr_tasks.h"
#include "mw_timebase.h"

#include "FreeRTOS.h"
#include "task.h"

static overall_t overall_state;

extern void thread_overal_state_monitor(void* args) {
    rtc_time_t rtc_time;
    rtc_date_t rtc_date;
    uint32_t seconds_tmp = 0, subseconds_tmp = 0;
    overall_state.base_ticks = mw_timebase_ticks_get();
    for (;;) {
        // base_time is ok if base_ticks are changing (increasing)
        overall_state.base_time_ok = overall_state.base_ticks
                != mw_timebase_ticks_get();
        overall_state.base_ticks = mw_timebase_ticks_get();
        overall_state.os_ticks = xTaskGetTickCount();
        mgr_rtc_get_time(&rtc_time);

        // TODO: Requires fix
        // workaround for RTC functions as otherwise seconds don't
        // increase if get_date, get_time are not both called
        mgr_rtc_get_date(&rtc_date);

        /* RTC is ok if subseconds and seconds are changing (increasing) */
        overall_state.rtc_ok = !((rtc_time.SubSeconds == subseconds_tmp)
                && (rtc_time.Seconds == seconds_tmp));
        subseconds_tmp = rtc_time.SubSeconds;
        seconds_tmp = rtc_time.Seconds;
        overall_state.task_statuses = mgr_tasks_get_last_measured();
        if (overall_state.task_statuses->is_available) {
            overal_state_analyze(&overall_state);
        }
        vTaskDelay(1000);
    }
}
