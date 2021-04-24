/*
 * cli.h
 *
 *  Created on: Jan 21, 2020
 *      Author: Zviad
 */

#ifndef UI_INC_CLI_H_
#define UI_INC_CLI_H_

#include <inttypes.h>

typedef void (*cmd_func_t)(void*);

typedef struct _cmdstruct {
    char* cmd;
    cmd_func_t fp;
    char* help;
} cmd_struct_t;

void cli_cmd_process(void* input);

#endif /* UI_INC_CLI_H_ */
