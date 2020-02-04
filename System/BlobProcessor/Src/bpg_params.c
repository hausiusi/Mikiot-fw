/*
 * bpg_params.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"

blob_fp_t* bp_params;

static const blob_fp_t _params[] = {

};

void bpt_params_init() {
	bp_params = (blob_fp_t*) _params;
}
