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
#include "mikiot_config.h"

#define DBG_ANY     1 // Should not be set. Is used for comparison to print anything when any debug level is enabled.
#define DBG_ERROR   1
#define DBG_WARNING 2
#define DBG_NOTE    3
#define DBG_INFO    4

#define DEBUG_ON                    1
#define DEBUG_BUFFER_SIZE           MCONF_DEBUG_BUFFER_SIZE

int debug_level;

// TODO: implement this idea, keep traces in ringbuffer and print them
void put_trace_text(char* ch);

void debugf(const char* buffer, ...);

void debug_set_level(int level);

int debug_get_level();

#if (DEBUG_ON == 1)

// TODO: implement this idea, keep traces in ringbuffer and print them
//#define debug_trace(str_format, vargs...) do { sprintf(trace_dbg_buf, str_format, ##vargs); put_trace_text(trace_dbg_buf); } while(0)

#define debug_info(str_format, vargs...) do { if (debug_level >= DBG_INFO) debugf("INFO: "str_format, ##vargs); } while(0)
#define debug_note(str_format, vargs...) do { if (debug_level >= DBG_NOTE) debugf("NOTE: "str_format, ##vargs); } while(0)
#define debug_warn(str_format, vargs...) do { if (debug_level >= DBG_WARNING) debugf("WARN: "str_format, ##vargs); } while(0)
#define debug_error(str_format, vargs...) do { if (debug_level >= DBG_ERROR) debugf("ERROR: "str_format, ##vargs); } while(0)
#define debug_p(str_format, vargs...) do { if (debug_level >= DBG_ANY) debugf(str_format, ##vargs); } while(0)

#else
//#define debug_trace(str_format, vargs...)
#define debug_p(str_format, vargs...)
#define debug_note(str_format, vargs...)
#define debug_warn(str_format, vargs...)
#define debug_error(str_format, vargs...)
#endif

#ifdef __cplusplus
 }
#endif

#endif /* DEBUG_H_ */
