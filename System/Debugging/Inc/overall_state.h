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
#include "defines.h"

#include "FreeRTOS.h"
#include "task.h"

#define WATERMARK_NORMAL_MIN        32
#define WATERMARK_LOW_MIN           16
#define WATERMARK_EXTREME_LOW_MIN   1
#define WATERMARK_OVERFLOW          0

typedef enum {
    TaskMemStateNormal = 1,
    TaskMemStateLow = 2,
    TaskMemStateExtremeLow = 3,
    TaskMemStateOverflow = 4,
} task_memory_state_t;

typedef struct {
    uint32_t task_id;
    task_memory_state_t memory_state;
    task_memory_state_t memory_state_reported;
} task_memory_info_t;

typedef struct _overall {
    uint32_t rtc_ok :1;
    uint32_t base_time_ok :1;
    uint32_t base_ticks;
    uint32_t os_ticks;
    tasks_statuses_t* task_statuses;
} overall_t;

void overal_state_init();
void overal_state_analyze(overall_t* overall_state);

#endif /* DEBUGGING_INC_OVERALL_STATE_H_ */
