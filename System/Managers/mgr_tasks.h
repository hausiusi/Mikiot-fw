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

typedef TaskHandle_t task_handle_t;
typedef TaskFunction_t task_function_t;

tasks_statuses_t* mgr_tasks_get_system_state();

tasks_statuses_t* mgr_tasks_get_last_measured();

bool_t mgr_tasks_kill_by_name(const char* task_name);

task_handle_t mgr_tasks_get_task_handle_from_name(const char* task_name);

bool_t mgr_tasks_create_by_name(char* task_name, char* name_to_assign,
        uint16_t stack_depth, uint32_t priority, void* const parameters);

task_function_t mgr_tasks_get_thread_by_name(char* name);

void mgr_tasks_list_available_runnables();

#endif /* MANAGERS_MGR_TASKS_H_ */
