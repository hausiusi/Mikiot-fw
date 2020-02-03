/*
 * bpg_actions.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"
#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"

blob_fp_t* bp_actions;

int _delay(blob_t* blob) {
	debug_p("BLOB-FUNC: actions._delay\n");
	if (blob->data.args_length != sizeof(uint32_t)) {
		//TODO: handle error
		return -1;
	}
	uint32_t time_ms = *((uint32_t*) blob->data.args);
	debug_p("_delay timeout: %lums\n", time_ms);
	//TODO: vTaskDelay is temporary imprecise solution, later time should be calculated using RTC
	vTaskDelay(time_ms);
	/* return block */
	return 0;
}

/* @formatter:off */
static const blob_fp_t _actions[] = {
	_delay,
};
/* @formatter:on */

void bpt_actions_init() {
	bp_actions = (blob_fp_t*) _actions;
}
