/*
 * bp_helper.h
 *
 *  Created on: Jan 30, 2020
 *      Author: Zviad
 */

#ifndef BLOBPROCESSOR_INC_BP_HELPER_H_
#define BLOBPROCESSOR_INC_BP_HELPER_H_

#include <inttypes.h>

typedef struct _blob_data {
    uint32_t group_id :8; /* Id of functions group. */
    uint32_t member_id :8; /* Id of a member from functions group. */
    uint32_t args_length :16; /* Length of arguments in bytes. */
    uint8_t* args; /* Arguments. */
} blob_data_t;

typedef struct _blob {
    uint32_t length :16; /* Whole blob length on the disk, including CRC. */
    uint32_t counter :16; /* Data pointer actual position from start. */
    blob_data_t data; /* Blob data. */
    uint16_t crc; /* Cyclic redundancy check in the end of blob, calculated from the rest of data. */
} blob_t;

typedef int (*blob_fp_t)(blob_t*);

typedef struct {
    blob_fp_t* functions; /* Pointer to blob functions array */
    uint32_t len; /* Length of parent array where this functions are placed */
} bfp_array_t; /* array of blob function pointers */

/* @formatter:off */
typedef enum {
	BpgActions = 0,
	BpgOperators = 1,
	BpgInits = 2,
	BpgInouts = 3,
	BpgParams = 4,
	BpgCount
}bp_groups_enum_t;
/* @formatter:on */

/* Pointers to functions of different groups */
bfp_array_t bp_actions;
bfp_array_t bp_operators;
bfp_array_t bp_inits;
bfp_array_t bp_inouts;
bfp_array_t bp_params;

/* The last blob processor function exit code */
int bpf_exit_code;

/* Group initializers */
extern void bpt_actions_init();
extern void bpt_conditions_init();
extern void bpt_inits_init();
extern void bpt_inouts_init();
extern void bpt_params_init();

#endif /* BLOBPROCESSOR_INC_BP_HELPER_H_ */
