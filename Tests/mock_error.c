/*
 * mock_error.c
 *
 *  Created on: Jul 16, 2020
 *      Author: Zviad
 */
#include "mock_error.h"
#include "test_framework.h"

void mock_error_limits_check() {
	if (mock_ereport_index > MOCK_EREPORTS_COUNT) {
		tf_write(
				"mock error report limit reached. Please, reset or increase. Limit is: %i\n",
				MOCK_EREPORTS_COUNT);
		while (1) {
		};
	}
}
