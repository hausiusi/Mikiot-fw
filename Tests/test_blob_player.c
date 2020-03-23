/*
 * test_blob_player.c
 *
 *  Created on: Feb 3, 2020
 *      Author: Zviad
 */

#include <inttypes.h>

/*
 * =======CAUTION==CAUTION==CAUTION=======
 * =====TESTS SHOULD BE RUN FROM TASK=====
 * =======CAUTION==CAUTION==CAUTION=======
 */

extern void test_blob_player() {
	/* Simple, 16-bytes long blob, delays thread during 170ms
	 * Can be run from terminal by typing: blob 10 00 00 00 00 00 04 00 AA 00 00 00 00 00 00 11
	 * Second part of the blob is empty. First time delay should work, second should fail
	 * TODO: CRC is not calculated and later should be replaced by correct one according to ISSUE_#43.
	 */
	uint8_t blob1[] = {
			0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0xAA, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x11 };
	(void) blob1;
}
