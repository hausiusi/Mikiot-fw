/*
 * mikiot_config.h
 *
 *  Created on: Jul 16, 2020
 *      Author: Zviad
 */

#ifndef INC_MIKIOT_CONFIG_H_
#define INC_MIKIOT_CONFIG_H_

#include "mock_error.h"
#include "error.h"

#define COMPILE_WITH_UNIT_TESTS           0
#define MCONF_DEBUG_BUFFER_SIZE           128

char tf_debug_array[MCONF_DEBUG_BUFFER_SIZE];

#if (COMPILE_WITH_UNIT_TESTS == 1)
#define mikiot_error_report(id, type)     (mock_error_report(id, type))
#define debug_put_log(dbg_array)          (memcpy(tf_debug_array, dbg_array, MCONF_DEBUG_BUFFER_SIZE))
#else
#define mikiot_error_report(id, type)     error_report_(__FILE__, __LINE__, __FUNCTION__, id, #type)
#define debug_put_log(dbg_array)          (mw_uart1_dma_string_transmit(dbg_array))
#endif

#endif /* INC_MIKIOT_CONFIG_H_ */
