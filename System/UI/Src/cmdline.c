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
#include "bp_player.h"
#include "mw_rtc.h"
#include "mgr_rtc.h"

void toggle_taskmgr_printing();

static void _echo(void* args);
static void _help(void* args);
static void _play_blob(void* args);
static void _datetime(void* args);
static void _taskmgr(void* args);

#define CMD_BLOB_MAX_SIZE 		64 /* Maximum accepted command-line length for blob */
static uint8_t blob_bytes[CMD_BLOB_MAX_SIZE];

/* @formatter:off */
static cmd_struct_t commands[] = {
	{ "echo", _echo, "Echoes the input" },
	{ "help", _help, "Prints this help" },
	{ "blob", _play_blob, "Converts input string to blob data and plays"},
	{ "datetime", _datetime, "Sets or gets datetime"},
	{ "taskmgr", _taskmgr, "Starts or stops task manager"},
};
/* @formatter:on */

static void _echo(void* args) {
	debug_p("%s\n", (char* )args);
}

static void _help(void* args) {
	debug_p("\nMikiot command-line help\n");
	for (int i = 0; i < array_count(commands); i++) {
		debug_p("%s - %s\n", commands[i].cmd, commands[i].help);
	}
	debug_p("--------------------------\n");
}

static void _play_blob(void* args) {
	debug_p("Playing the blob\n");
	/* Divide by 3 because 2 hex digits represent 1 byte and there is one space.
	 * And add one at the end because the last one doesn't have a space
	 * TODO: Until tests will be done example blob command is bellow:
	 * blob 15 00 00 00 00 00 04 00 AA 10 00 00 00 00 04 00 AA 02 00 00 11 */
	uint32_t len = (strlen(args) / 3) + 1;
	if (len > CMD_BLOB_MAX_SIZE) {
		//TODO: Handle CMD blob size error here
		return;
	}

	for (uint32_t i = 0; i < len; i++) {
		for (int j = 0; j < 2; j++) {
			char c = *((char*) args++);
			/* https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int */
			char val = ((c & 0xF) + (c >> 6)) | ((c >> 3) & 0x8);
			blob_bytes[i] = (blob_bytes[i] << 4) | (uint8_t) val;
		}
		args++;
	}
	bp_player_play(blob_bytes);
}

static void _datetime(void* args) {
	if (!strncmp(args, "set", 3)) {
		args += 4; // It is supposed to be one space after set argument
		mgr_rtc_set_strf((char*) args);

	} else if (!strncmp(args, "get", 3)) {
		mgr_rtc_print_date();
	} else {
		debug_p("datetime command requires passed arguments\n");
		debug_p("    get - prints system date and time in the terminal\n");
		debug_p(
				"    set - sets passed date and time as a system clock. format: dd-MM-yy HH:mm:ss\n");
	}
}

static void _taskmgr(void* args) {
	toggle_taskmgr_printing();
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
