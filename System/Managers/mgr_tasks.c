/*
 * mgr_tasks.c
 *
 *  Created on: Apr 25, 2020
 *      Author: Zviad
 */

#include <string.h>
#include "mgr_tasks.h"

static tasks_statuses_t system_tasks_state;
static TaskStatus_t* tasks_statuses_current = NULL, * tasks_statuses_prev =
NULL;

tasks_statuses_t* mgr_tasks_get_system_state() {
	uint32_t total_runtime;
	static uint32_t alloc_len_buffer = 0;
	static uint32_t alloc_len_last = 0;
	// First, make a copy of tasks statuses if it already exists, if not - skip.
	// Allocate memory only if it is needed
	if (alloc_len_last) {
		if (alloc_len_buffer != alloc_len_last || tasks_statuses_prev == NULL) {
			if (tasks_statuses_prev) {
				vPortFree(tasks_statuses_prev);
			}
			tasks_statuses_prev = pvPortMalloc(alloc_len_last);
			if (!tasks_statuses_prev) {
				debug_p(
						"Unable to allocate %lu bytes of memory to copy last task statuses \n",
						alloc_len_last);
				system_tasks_state.is_available = false;
				return &system_tasks_state;
			}
		}
		// If above operations are successful save this previously measured tasks status state
		// in the buffer to have a reference for next check
		memcpy(tasks_statuses_prev, tasks_statuses_current, alloc_len_last);
	}
	// Update last task allocation length which after this line is current tasks allocation length
	alloc_len_buffer = alloc_len_last;
	system_tasks_state.current_count = uxTaskGetNumberOfTasks();
	alloc_len_last = system_tasks_state.current_count * sizeof(TaskStatus_t);
	// Allocate memory only if it is needed
	// Buffer allocation length is the same as for tasks during previous check
	if (alloc_len_buffer < alloc_len_last || tasks_statuses_current == NULL) {
		if (tasks_statuses_current) {
			vPortFree(tasks_statuses_current);
		}
		tasks_statuses_current = pvPortMalloc(alloc_len_last);
		if (!tasks_statuses_current) {
			debug_p(
					"Unable to allocate %lu bytes of memory for task statuses\n",
					alloc_len_last);
			system_tasks_state.is_available = false;
			return &system_tasks_state;
		}

	}
	system_tasks_state.current_count = uxTaskGetSystemState(
			tasks_statuses_current, system_tasks_state.current_count,
			&total_runtime);
	system_tasks_state.previous = tasks_statuses_prev;
	system_tasks_state.current = tasks_statuses_current;
	system_tasks_state.is_available = tasks_statuses_prev
			&& tasks_statuses_current;
	return &system_tasks_state;
}
