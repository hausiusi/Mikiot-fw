/*
 * bpg_inouts.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"
#include "macrodefs.h"

bfp_array_t bp_inouts;

static const blob_fp_t _inouts[] = {

};

void bpt_inouts_init() {
	bp_inouts.functions = (blob_fp_t*) _inouts;
	bp_inouts.len = array_count(_inouts);
}
