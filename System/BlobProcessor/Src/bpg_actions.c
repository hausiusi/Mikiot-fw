/*
 * bpg_actions.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"
#include "debug.h"
#include "error.h"
#include "FreeRTOS.h"
#include "macrodefs.h"
#include "mw_timebase.h"
#include "task.h"

bfp_array_t bp_actions;

static int _delay(blob_t* blob) {
	debug_info("BLOB-FUNC: actions._delay\n");
	if (blob->data.args_length != sizeof(uint32_t)) {
		error_report(7, BlobProcessorError);
		return -1;
	}
	blob->counter += blob->data.args_length;
	uint32_t delay_amount_ms = *((uint32_t*) blob->data.args);
	debug_info("_delay timeout: %lums\n", delay_amount_ms);
	uint32_t timeout_end = mw_timebase_ticks_get() + delay_amount_ms;
	while (mw_timebase_ticks_get() < timeout_end) {
		vTaskDelay(0);
	}

	/* return block */
	return 0;
}

static int _jump(blob_t* blob) {
	debug_info("BLOB-FUNC: actions._jump\n");
	if (blob->data.args_length != sizeof(uint16_t)) {
		error_report(11, BlobProcessorError);
		return -1;
	}
	blob->counter += blob->data.args_length;
	uint16_t jump_to = *((uint16_t*) blob->data.args);
	debug_info("Jump to: %i\n", jump_to);
	blob->counter = jump_to;
	return 0;
}

static int _print(blob_t* blob) {
	debug_info("BLOB-FUNC: actions._print\n");
	if (blob->data.args[blob->data.args_length - 1] != 0) {
		error_report(12, BlobProcessorError);
		return -1;
	}
	blob->counter += blob->data.args_length;
	debug_p("%s\n", (char* )blob->data.args);
	return 0;
}

/* @formatter:off */
static const blob_fp_t _actions[] = {
	_delay,
	_jump,
	_print
};
/* @formatter:on */

void bpt_actions_init() {
	bp_actions.functions = (blob_fp_t*) _actions;
	bp_actions.len = array_count(_actions);
}
