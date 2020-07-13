/*
 * bpg_operators.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"
#include "bp_groups.h"
#include "debug.h"
#include "error.h"
#include "macrodefs.h"

bfp_array_t bp_operators;

int _if(blob_t* blob) {
	debug_info("BLOB-FUNC: operators._if\n");
	// Skip first 10 bytes
	blob->counter += blob->data.args_length;
	if (blob->data.args_length != 10) {
		error_report(14, BlobProcessorError);
		return -1;
	}
	// Group id and member id are first two bytes of args
	int32_t expect = *(int32_t*) (blob->data.args);
	uint16_t jump_to = *(uint16_t*) (blob->data.args + 4);
	blob_data_t* bdata = (blob_data_t*) (blob->data.args + 6);
	blob_fp_t bfp = bp_groups_get_actual_function(
			(bp_groups_enum_t) bdata->group_id, bdata->member_id);
	blob->data.args = bdata->args;
	if (bfp) {
		if (bfp(blob) != expect) {
			blob->counter = jump_to;
		}
	}
	return 0;
}

/*
 * Skip next if last function return value equals
 */
int _snilfe(blob_t* blob) {
	debug_info("BLOB-FUNC: operators._snilfe\n");
	blob->counter += blob->data.args_length;
	if (blob->data.args_length != sizeof(int32_t)) {
		error_report(13, BlobProcessorError);
		return -1;
	}
	int exit_code = *((int*) blob->data.args);
	if (exit_code == bpf_exit_code) {
		// Skip next function data part completely
		blob_data_t* next_data = (blob_data_t*) ((uint8_t*) blob->data.args
				+ blob->data.args_length);
		blob->counter += offsetof(blob_data_t, args) + next_data->args_length;
	}
	return 0;
}

/* @formatter:off */
static const blob_fp_t _operators[] = {
	_if,
	_snilfe
};
/* @formatter:on */

void bpt_conditions_init() {
	bp_operators.functions = (blob_fp_t*) _operators;
	bp_operators.len = array_count(_operators);
}
