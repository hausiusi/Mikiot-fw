/*
 * cmdline.c
 *
 *  Created on: Jan 21, 2020
 *      Author: Zviad
 */

#include <string.h>
#include "cmdline.h"
#include "macrodefs.h"
#include "debug.h"

static void _test(void* args);
static void _help(void* args);

/* @formatter:off */
static cmd_struct_t commands[] = {
	{ "echo", _test, "Echoes the input" },
	{ "help", _help, "Prints this help" }
};
/* @formatter:on */

static void _test(void* args) {
	debug_p("%s\n", (char* )args);
}

static void _help(void* args) {
	debug_p("\nMikiot command-line help\n");
	for (int i = 0; i < array_count(commands); i++) {
		debug_p("%s - %s\n", commands[i].cmd, commands[i].help);
	}
	debug_p("--------------------------\n");
}

void cmd_process(void* input) {
	for (int i = 0; i < array_count(commands); i++) {
		if (!strncmp(commands[i].cmd, (char*) input,
				strlen((char*) commands[i].cmd))) {
			commands[i].fp(input + strlen(commands[i].cmd) + 1);
			return;
		}
	}
	debug_p("Unknown command. Type help\n");
}
