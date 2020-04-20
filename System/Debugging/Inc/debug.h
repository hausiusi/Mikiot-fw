/*
 * debug.h
 *
 *  Created on: Feb 17, 2019
 *      Author: Zviad
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <mw_uart1.h>
#include "mw_redefines.h"

#define DEBUG_ON 1
#define DEBUG_BUFFER_SIZE 128

char trace_dbg_buf[DEBUG_BUFFER_SIZE];

void put_trace_text(char* ch);

void debugf(const char* buffer, ...);

#define debug_trace(str_format, vargs...) do { sprintf(trace_dbg_buf, str_format, ##vargs); \
											 	 put_trace_text(trace_dbg_buf); } while(0)

#define debug_p(str_format, vargs...) do { debugf(str_format, ##vargs); } while(0)

#ifdef __cplusplus
 }
#endif

#endif /* DEBUG_H_ */
