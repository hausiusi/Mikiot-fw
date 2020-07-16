/*
 * error.h
 *
 *  Created on: Mar 6, 2019
 *      Author: Zviad
 */

#ifndef ERROR_H_
#define ERROR_H_

#include "debug.h"
#include "mikiot_config.h"

/* Get file path, line and function name where error happened, using
 * __FILE__, __LINE__ and __FUNCTION__ macro */
#define error_report(id, type) (mikiot_error_report(id, type))
// @formatter:off
typedef enum {
	InitError = 0,
	SetupError = 1,
	WriteError = 2,
	ReadError = 3,
	VerificationError = 4,
	CmdlineError = 5,
	BlobProcessorError = 6,
} error_enum_t;
// @formatter:on
typedef struct {
	uint32_t id;
	error_enum_t error_type;
	char* error_buffer_p;
} error_t;

void error_report_(const char* file_name, size_t line, const char* func_name,
		uint32_t id, const char* type);

#endif /* ERROR_H_ */
