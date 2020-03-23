/*
 * bpg_inits.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"

blob_fp_t* bp_inits;

static const blob_fp_t _inits[] = {

};

extern void bpt_inits_init() {
	bp_inits = (blob_fp_t*) _inits;
}
