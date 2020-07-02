/*
 * bp_groups.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */
#include <stddef.h>
#include "bp_helper.h"
#include "defines.h"
#include "error.h"

blob_fp_t* bp_actions;
blob_fp_t* bp_operators;
blob_fp_t* bp_inits;
blob_fp_t* bp_inouts;
blob_fp_t* bp_params;

static bool_t groups_initialized = false;

blob_fp_t* _groups[BpgCount];

blob_fp_t bp_groups_get_actual_function(bp_groups_enum_t group_id,
		uint16_t member_id) {
	if (!groups_initialized) {
		bpt_actions_init();
		bpt_conditions_init();
		bpt_inits_init();
		bpt_inouts_init();
		bpt_params_init();
		_groups[BpgActions] = bp_actions;
		_groups[BpgOperators] = bp_operators;
		_groups[BpgInits] = bp_inits;
		_groups[BpgInouts] = bp_inouts;
		_groups[BpgParams] = bp_params;
		/* Set the conditional initialization variable to true */
		groups_initialized = true;
	}
	if (!(group_id < COUNT(_groups))) {
		error_report(8, BlobProcessorError);
		return NULL;
	}
	if (!(member_id < COUNT(_groups[group_id]))) {
		error_report(9, BlobProcessorError);
		return NULL;
	}
	return _groups[group_id][member_id];
}
