/*
 * mock_error.h
 *
 *  Created on: Jul 16, 2020
 *      Author: Zviad
 */

#ifndef MOCK_ERROR_H_
#define MOCK_ERROR_H_

#include <inttypes.h>

#define MOCK_EREPORTS_COUNT 30

extern uint8_t mock_ereports[MOCK_EREPORTS_COUNT][2];
extern uint8_t mock_ereport_index;

void mock_error_limits_check();

#define mock_error_report(id, type)                                                                                      \
    {                                                                                                                    \
    	mock_error_limits_check();                                                                                       \
    	mock_ereports[mock_ereport_index][0] = id;                                                                       \
    	mock_ereports[mock_ereport_index][1] = type;                                                                     \
    	mock_ereport_index++;                                                                                            \
    }

#define mock_error_report_reset() do { mock_ereport_index = 0; memset(mock_ereports, 0, sizeof(mock_ereports)); } while(0)

#endif /* MOCK_ERROR_H_ */
