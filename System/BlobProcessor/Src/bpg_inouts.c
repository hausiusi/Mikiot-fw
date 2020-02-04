/*
 * bpg_inouts.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"

blob_fp_t* bp_inouts;

static const blob_fp_t _inouts[] = {

};

void bpt_inouts_init() {
	bp_inouts = (blob_fp_t*) _inouts;
}
