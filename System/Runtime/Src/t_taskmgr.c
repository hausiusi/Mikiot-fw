/*
 * t_taskmgr.c
 *
 *  Created on: Apr 19, 2020
 *      Author: Zviad
 */

#include "string.h"
#include "debug.h"
#include "utils.h"
#include "stddef.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mw_redefines.h"
#include "mgr_tasks.h"

void _system_load_print(TaskStatus_t* const tasks_statuses_current,
        TaskStatus_t* tasks_statuses_prev, uint32_t tasks_count);

static bool_t taskmgr_printing;

extern void thread_taskmgr() {
    tasks_statuses_t* tasks_statuses;
    for (;;) {
        tasks_statuses = mgr_tasks_get_system_state();
        if (tasks_statuses->is_available) {
            if (taskmgr_printing) {
                _system_load_print(tasks_statuses->current,
                        tasks_statuses->previous,
                        tasks_statuses->current_count);
            }
        }
        vTaskDelay(1000);
    }
}

extern void toggle_taskmgr_printing() {
    taskmgr_printing = !taskmgr_printing;
}

void _system_load_print(TaskStatus_t* const tasks_statuses_current,
        TaskStatus_t* tasks_statuses_prev, uint32_t tasks_count) {
    // Check how much CPU time used each task after the last check
    // For this subtract previous counter value from the current runtime counter of the task
    for (int i = 0; i < tasks_count; i++) {
        for (int j = 0; j < tasks_count; j++) {
            if (tasks_statuses_prev[i].xTaskNumber
                    == tasks_statuses_current[j].xTaskNumber) {
                tasks_statuses_prev[i].ulRunTimeCounter =
                        tasks_statuses_current[j].ulRunTimeCounter
                                - tasks_statuses_prev[i].ulRunTimeCounter;
            }
        }
    }
    float task_load = 0;
    static char spaces1[15];
    static char spaces2[4];
    float total_runtime = 0;
    // Calculate the total amount of time passed after last check for all tasks together
    for (int i = 0; i < tasks_count; i++) {
        total_runtime += tasks_statuses_prev[i].ulRunTimeCounter;
    }
    total_runtime /= 100;
    if (total_runtime == 0) {
        return;
    }
    debug_p("Name             Load       WM\n");
    debug_p("------------------------------\n");
    for (int i = 0; i < tasks_count; i++) {

        task_load = (tasks_statuses_prev[i].ulRunTimeCounter / total_runtime);
        memset(spaces1, 0, sizeof(spaces1));
        memset(spaces1, 32,
                sizeof(spaces1) - strlen(tasks_statuses_prev[i].pcTaskName));
        memset(spaces2, 0, sizeof(spaces2));
        memset(spaces2, 32, sizeof(spaces2) - unumlen(task_load));
        debug_p("%s %s %f %s %u\n", tasks_statuses_prev[i].pcTaskName, spaces1,
                task_load, spaces2,
                tasks_statuses_prev[i].usStackHighWaterMark); //TODO: This line will delay watermark (shows current in next). Use find by task id and print from current statuses
    }
    debug_p("\n");
}
