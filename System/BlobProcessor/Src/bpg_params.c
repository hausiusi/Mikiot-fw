/*
 * bpg_params.c
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#include "bp_helper.h"
#include "macrodefs.h"

bfp_array_t bp_params;

static const blob_fp_t _params[] = {

};

void bpt_params_init() {
    bp_params.functions = (blob_fp_t*) _params;
    bp_params.len = array_count(_params);
}
