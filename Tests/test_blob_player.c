/*
 * test_blob_player.c
 *
 *  Created on: Feb 3, 2020
 *      Author: Zviad
 */
#include "mikiot_config.h"

#if (COMPILE_WITH_UNIT_TESTS == 1)

#include <inttypes.h>
#include "bp_player.h"
#include "test_framework.h"
#include "mock_error.h"

/*
 * =======CAUTION==CAUTION==CAUTION=======
 * =    TESTS SHOULD BE RUN FROM TASK    =
 * =======CAUTION==CAUTION==CAUTION=======
 */

#define INCORRECT_BLOB_MAXLEN 25

/* @formatter:off */
const uint8_t incorrect_blobs[][INCORRECT_BLOB_MAXLEN] = {
		/* 0. delay has incorrect length of arguments (3 instead of 4) */
		{0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0A, 0x00, 0x00, 0x07, 0x00, /* this blob ends here, filling up with extra bytes */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		/* 1. jump has incorrect length of arguments */
		{0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xF4, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x04, 0x00, 0x00, 0x07, 0x00, /* this blob ends here, filling up with extra bytes */ 0, 0, 0, 0},
		/* 2. print has incorrect length of arguments */
		{0x10, 0x00, 0x00, 0x00, 0x00, 0x02, 0x05, 0x00, 0x70, 0x72, 0x69, 0x6E, 0x74, 0x00, 0x07, 0x00,  /* this blob ends here, filling up with extra bytes */ 0, 0, 0, 0, 0, 0, 0, 0, 0},
		/* 3. snilfe has incorrect length of arguments */
		{0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x64, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x04, 0x00, 0x07, 0x00},
		/* 4. incorrect member id */
		{0x0C, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x02, 0x00, 0x04, 0x00, 0x07, 0x00, /* this blob ends here, filling up with extra bytes */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		/* 5. incorrect group id */
		{0x0C, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x02, 0x00, 0x04, 0x00, 0x07, 0x00, /* this blob ends here, filling up with extra bytes */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		/* */
		};
	/* @formatter:on */

void _load_and_check_incorrect_blob(int blob_index, int error_index,
		uint32_t expected_error_id, error_enum_t expected_error_type) {
	uint8_t blob[INCORRECT_BLOB_MAXLEN];
	memcpy(blob, incorrect_blobs[blob_index], sizeof(blob));
	mock_error_report_reset();
	bp_player_play(blob);
	tf_equals(mock_ereports[error_index][0], expected_error_id);
	tf_equals(mock_ereports[error_index][1], expected_error_type);
}

extern void test_incorrect_blob() {
	_load_and_check_incorrect_blob(0, 0, 7, BlobProcessorError);
	_load_and_check_incorrect_blob(1, 0, 11, BlobProcessorError);
	_load_and_check_incorrect_blob(2, 0, 12, BlobProcessorError);
	_load_and_check_incorrect_blob(3, 0, 13, BlobProcessorError);
	_load_and_check_incorrect_blob(4, 0, 9, BlobProcessorError);
	_load_and_check_incorrect_blob(5, 0, 8, BlobProcessorError);
}

#endif
