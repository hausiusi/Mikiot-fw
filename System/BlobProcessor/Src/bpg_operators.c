/*
 * bpg_operators.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"

blob_fp_t* bp_operators;

int _if(blob_t* args) {
	// TODO: Should be able to run next blob operation and in case of success implement particular block, otherwise skip it. Jump. Increase the blob.counter
	return 0;
}

/* @formatter:off */
static const blob_fp_t _operators[] = {
	_if,
};
/* @formatter:on */

void bpt_conditions_init() {
	bp_operators = (blob_fp_t*) _operators;
}