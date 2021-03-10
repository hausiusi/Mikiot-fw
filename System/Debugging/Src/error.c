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

void error_report_(const char* file_name, size_t line, const char* func_name,
        uint32_t id, const char* type) {
    debug_error("%u\nType: %s\nFile: %s\nLine: %i\nFunction: %s\n", id, type,
            file_name, line, func_name);
}

void throw_error() {

}

