/*
 * bpg_inits.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"
#include "macrodefs.h"

bfp_array_t bp_inits;

static const blob_fp_t _inits[] = {

};

extern void bpt_inits_init() {
	bp_inits.functions = (blob_fp_t*) _inits;
	bp_inits.len = array_count(_inits);
}
