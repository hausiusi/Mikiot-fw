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

void _system_load_print(TaskStatus_t* const tasks_statuses,
		uint32_t tasks_count);

static TaskStatus_t* tasks_statuses_now = NULL, * tasks_statuses_prev = NULL;

extern void thread_taskmgr() {
	uint32_t total_runtime;
	uint32_t alloc_len_prev = 0;
	for (;;) {
		uint32_t tasks_count = uxTaskGetNumberOfTasks();
		uint32_t alloc_len_now = tasks_count * sizeof(TaskStatus_t);
		// Allocate memory only if it is needed
		if (alloc_len_prev < alloc_len_now || tasks_statuses_now == NULL) {
			if (tasks_statuses_now) {
				vPortFree(tasks_statuses_now);
			}
			tasks_statuses_now = pvPortMalloc(alloc_len_now);
			if (!tasks_statuses_now) {
				debug_p(
						"Unable to allocate %lu bytes of memory for task statuses\n",
						alloc_len_now);
				continue;
			}
			alloc_len_prev = alloc_len_now;
		}
		tasks_count = uxTaskGetSystemState(tasks_statuses_now, tasks_count,
				&total_runtime);
		if (tasks_statuses_prev) {
			_system_load_print(tasks_statuses_now, tasks_count);
		}
		// Allocate memory only if it is needed
		if (alloc_len_prev < alloc_len_now || tasks_statuses_prev == NULL) {
			if (tasks_statuses_prev) {
				vPortFree(tasks_statuses_prev);
			}
			tasks_statuses_prev = pvPortMalloc(alloc_len_now);
			if (!tasks_statuses_prev) {
				debug_p(
						"Unable to allocate %lu bytes of memory for previous copy of task statuses \n",
						alloc_len_now);
				continue;
			}
		}
		// After all (process, print etc.) save this task status state in the buffer
		// to have a reference for next check
		memcpy(tasks_statuses_prev, tasks_statuses_now, alloc_len_now);
		vTaskDelay(1000);
	}
}

void _system_load_print(TaskStatus_t* const tasks_statuses,
		uint32_t tasks_count) {
	// Check how much CPU time used each task after the last check
	// For this subtract previous counter value from the current runtime counter of the task
	for (int i = 0; i < tasks_count; i++) {
		for (int j = 0; j < tasks_count; j++) {
			if (tasks_statuses_prev[i].xTaskNumber
					== tasks_statuses[j].xTaskNumber) {
				tasks_statuses_prev[i].ulRunTimeCounter =
						tasks_statuses[j].ulRunTimeCounter
								- tasks_statuses_prev[i].ulRunTimeCounter;
			}
		}
	}
	int task_load = 0;
	static char spaces1[15];
	static char spaces2[4];
	uint32_t total_runtime = 0;
	// Calculate the total amount of time passed after last check for all tasks together
	for (int i = 0; i < tasks_count; i++) {
		total_runtime += tasks_statuses_prev[i].ulRunTimeCounter;
	}
	total_runtime /= 100;
	if (total_runtime == 0) {
		return;
	}
	debug_p("Name             Load  WM\n");
	debug_p("--------------------------\n");
	for (int i = 0; i < tasks_count; i++) {

		task_load = (tasks_statuses_prev[i].ulRunTimeCounter / total_runtime);
		memset(spaces1, 0, sizeof(spaces1));
		memset(spaces1, 32,
				sizeof(spaces1) - strlen(tasks_statuses_prev[i].pcTaskName));
		memset(spaces2, 0, sizeof(spaces2));
		memset(spaces2, 32, sizeof(spaces2) - unumlen(task_load));
		debug_p("%s %s %i %s %u\n", tasks_statuses_prev[i].pcTaskName, spaces1,
				task_load, spaces2,
				tasks_statuses_prev[i].usStackHighWaterMark); //TODO: This line will delay watermark (shows current in next). Use find by task id and print from current statuses
	}
	debug_p("\n");
}
