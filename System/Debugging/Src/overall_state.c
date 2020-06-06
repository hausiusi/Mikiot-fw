/*
 * monitor.c
 *
 *  Created on: May 31, 2020
 *      Author: Zviad
 */

#include "debug.h"
#include "overall_state.h"

void overal_state_analyze(overall_t* overall_state) {
	/* TODO: This function must check overall state structure and
	 * display the errors via different interfaces
	 */
	if (!overall_state->base_time_ok) {
		debug_warn("Base time doesn't work\n");
	}
	if (!overall_state->rtc_ok) {
		debug_warn("RTC doesn't work\n");
	}

	// Check task statuses and warn if possible stack overflow is discovered
	tasks_statuses_t* task_statuses = overall_state->task_statuses;
	if (task_statuses->is_available) {
		for (uint8_t i = 0; i < task_statuses->current_count; i++) {
			if (task_statuses->current[i].usStackHighWaterMark == 0) {
				debug_warn("Task \"%s\" has possible stack overflow\n",
						task_statuses->current[i].pcTaskName);
			}
		}
	}
}
