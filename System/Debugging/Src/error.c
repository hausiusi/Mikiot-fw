/*
 * error.c
 *
 *  Created on: Feb 16, 2019
 *      Author: Zviad
 */

#include "stdint.h"
#include "debug.h"
#include "error.h"
#include "FreeRTOS.h"
#include "task.h"

static char error_buff[150];

void error_report_(const char* file_name, size_t line, const char* func_name,
		uint32_t id, uint32_t type) {
	char const* msg = "\nERROR_%u\nType: %u\nFile: %s\nLine: %i\nFunction: %s\n";
	sprintf(error_buff, msg, id, type, file_name, line, func_name);
	debug_p(error_buff);
}

void throw_error() {

}

