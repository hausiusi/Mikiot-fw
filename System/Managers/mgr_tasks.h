/*
 * mgr_tasks.h
 *
 *  Created on: Apr 25, 2020
 *      Author: Zviad
 */

#ifndef MANAGERS_MGR_TASKS_H_
#define MANAGERS_MGR_TASKS_H_

#include "defines.h"
#include "mw_redefines.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct _tasks_statuses {
    bool_t is_available;
    TaskStatus_t* current;
    TaskStatus_t* previous;
    uint32_t current_count;
} tasks_statuses_t;

tasks_statuses_t* mgr_tasks_get_system_state();

tasks_statuses_t* mgr_tasks_get_last_measured();

#endif /* MANAGERS_MGR_TASKS_H_ */
