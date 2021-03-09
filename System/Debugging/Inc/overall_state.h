/*
 * monitor.h
 *
 *  Created on: May 31, 2020
 *      Author: Zviad
 */

#ifndef DEBUGGING_INC_OVERALL_STATE_H_
#define DEBUGGING_INC_OVERALL_STATE_H_

#include <inttypes.h>

#include "mgr_tasks.h"

#include "FreeRTOS.h"
#include "task.h"

typedef struct _overall {
    uint32_t rtc_ok :1;
    uint32_t base_time_ok :1;
    uint32_t base_ticks;
    uint32_t os_ticks;
    tasks_statuses_t* task_statuses;
} overall_t;

void overal_state_analyze(overall_t* overall_state);

#endif /* DEBUGGING_INC_OVERALL_STATE_H_ */
