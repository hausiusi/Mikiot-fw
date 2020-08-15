/*
 * cmdline.c
 *
 *  Created on: Jan 21, 2020
 *      Author: Zviad
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "cmdline.h"
#include "macrodefs.h"
#include "debug.h"
#include "bp_player.h"
#include "error.h"
#include "mw_rtc.h"
#include "mgr_adc.h"
#include "mgr_rtc.h"
#include "os_loader.h"
#include "performance.h"
#include "utils.h"
#include "version.h"

void toggle_taskmgr_printing();

static void _echo(void* args);
static void _help(void* args);
static void _play_blob(void* args);
static void _datetime(void* args);
static void _taskmgr(void* args);
static void _time(void* args);
static void _adc(void* args);
static void _perfinfo(void* args);
static void _dbglevel(void* args);
static void _version(void* args);

#define CMD_BLOB_MAX_SIZE 		128 /* Maximum accepted command-line length for blob */
static uint8_t blob_bytes[CMD_BLOB_MAX_SIZE];
#define CMD_ADC_REF_VOLTAGE 3.0f

/* @formatter:off */
static cmd_struct_t commands[] = {
	{ "echo", _echo, "Echoes the input" },
	{ "help", _help, "Prints this help" },
	{ "blob", _play_blob, "Converts input string to blob data and plays"},
	{ "datetime", _datetime, "Sets or gets datetime"},
	{ "taskmgr", _taskmgr, "Starts or stops task manager"},
	{ "time", _time, "Measures command execution time" },
	{ "adc", _adc, "Measures voltage on PB0" },
	{ "perfinfo", _perfinfo, "Gets the information about current performance" },
	{ "debuglevel", _dbglevel, "Gets or sets debug level" },
	{ "version", _version, "Prints current version" },
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

static void _version(void* args) {
	debug_p("Version: %i.%i.%i\nCode name: %s\nRelease date: %s\n",
			VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_CODE_NAME,
			VERSION_RELEASE_DATE);
}

static void _play_blob(void* args) {
	debug_p("Playing the blob\n");
	/* Divide by 3 because 2 hex digits represent 1 byte and there is one space.
	 * And add one at the end because the last one doesn't have a space
	 * TODO: Until tests will be done example blob command is bellow:
	 * blob 15 00 00 00 00 00 04 00 AA 10 00 00 00 00 04 00 AA 02 00 00 11 */
	uint32_t len = (strlen(args) / 3) + 1;
	if (len > CMD_BLOB_MAX_SIZE) {
		error_report(5, CmdlineError);
		return;
	}

	for (uint32_t i = 0; i < len; i++) {
		for (int j = 0; j < 2; j++) {
			char c = *((char*) args++);
			if (!isxdigit(c)) {
				error_report(6, CmdlineError);
				return;
			}
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

static void _time(void* args) {
	debug_p("PERF: Measuring execution time for: '%s'\n", (char* )args);
	uint32_t time_us = prf_func_exect_time_get(cmd_process(args));
	debug_p("PERF: Execution took %lu us\n", time_us);
}

static char* _move_to_next_word(char* str, char* out_word, int max_len) {
	if (*str == '\0') {
		return NULL;
	}
	int len = 0;
	do {
		if (*str == ' ') {
			str++;
			break;
		}
		*out_word = *str;
		out_word++;
		if (++len > max_len) {
			debug_error("Argument must be shorter than %i characters\n",
					max_len);
			return NULL;
		}
	} while (*str++);
	*out_word = '\0';
	return str;
}

static void _adc(void* args) {
	adc_uiconf_t* adc_uiconf = mgr_adc_get_uiconf();
	adc_uiconf->ref_voltage = CMD_ADC_REF_VOLTAGE;
	adc_uiconf->permanent = true;
	char out_word[10] = { 0 };
	char* wordptr = args;
	int arg_counter = 0;
	while (wordptr = _move_to_next_word(wordptr, out_word, 9), wordptr != NULL) {
		if (!strncmp(out_word, "start", 5)) {
			arg_counter++;
			mgr_adc_init();
			vTaskResume(os_get_task_handler(AdcHandler));
		} else if (!strncmp(out_word, "stop", 4)) {
			mgr_adc_deinit();
			arg_counter++;
		} else if (!strncmp(out_word, "value", 5)) {
			arg_counter++;
			adc_uiconf->print_value = true;
		} else if (!strncmp(out_word, "voltage", 7)) {
			arg_counter++;
			adc_uiconf->print_voltage = true;
		} else if (!strncmp(out_word, "delay", 5)) {
			arg_counter++;
			wordptr = _move_to_next_word(wordptr, out_word, 9);
			bool_t is_ok = wordptr != NULL && is_integer(out_word);
			if (!is_ok) {
				debug_error("Argument `delay` must be followed by number\n");
				return;
			}
			adc_uiconf->delay = atoi(out_word);
		} else if (!strncmp(out_word, "amount", 6)) {
			arg_counter++;
			wordptr = _move_to_next_word(wordptr, out_word, 9);
			bool_t is_ok = wordptr != NULL && is_integer(out_word);
			if (!is_ok) {
				debug_error("Argument `amount` must be followed by number\n");
				return;
			}
			adc_uiconf->amount = atoi(out_word);
			adc_uiconf->permanent = false;
		}
	}
	if (!arg_counter) {
		debug_p("The adc command requires arguments:\n");
		debug_p("    start   - starts the ADC conversions if it is stopped\n");
		debug_p("    stop    - stops the ADC conversions if it is started\n");
		debug_p("    value   - shows the converted value\n");
		debug_p("    voltage - converts ADC value into voltage\n");
		debug_p(
				"    delay   - delays between prints by the number followed after\n");
		debug_p(
				"    amount  - number of processed before the automatic deinitialization\n");
	}
}

static void _dbglevel(void* args) {
	bool_t success = true;
	if (!strncmp(args, "set", 3)) {
		args += 4; // It is supposed to be one space after set argument
		char symbol = *(char*) args;
		int level = 0;
		switch (symbol) {
		case 'E':
			level = DBG_ERROR;
			break;
		case 'W':
			level = DBG_WARNING;
			break;
		case 'N':
			level = DBG_NOTE;
			break;
		case 'I':
			level = DBG_INFO;
			break;
		default:
			success = false;
		}
		if (success) {
			debug_set_level(level);
			debug_p("Debug level set successfully\n");
		}
	} else if (!strncmp(args, "get", 3)) {
		int current_level = debug_get_level();
		debug_p("Current debug level is ");
		switch (current_level) {
		case DBG_ERROR:
			debug_p("DBG_ERROR");
			break;
		case DBG_WARNING:
			debug_p("DBG_WARNING");
			break;
		case DBG_NOTE:
			debug_p("DBG_NOTE");
			break;
		case DBG_INFO:
			debug_p("DBG_INFO");
			break;
		default:
			debug_p("UNKNOWN");
			break;
		}
		debug_p("\n");
	} else {
		success = false;
	}
	if (!success) {
		debug_p(
				"To set debug level type 'debuglevel set' and one of following symbols:\n");
		debug_p("    E - nothing will printed except errors\n");
		debug_p("    W - errors and warnings will be printed\n");
		debug_p("    N - errors, warnings and notifications will be printed\n");
		debug_p("    I - every possible printable outputs will be printed\n");
	}
}

static void _perfinfo(void* args) {
	perfinfo_t perfinfo;
	prf_get_info(&perfinfo);
	debug_p("Core clock       : %uHz\n", perfinfo.core_clock);
	debug_p("AHB clock        : %uHz\n", perfinfo.ahb_clock);
	debug_p("APB1 clock       : %uHz\n", perfinfo.apb1_clock);
	debug_p("APB2 clock       : %uHz\n", perfinfo.apb2_clock);
	debug_p("Tasks count      : %u\n", perfinfo.tasks_count);
	debug_p("Average CPU load : %u%\n", perfinfo.cpu_utilization);
	debug_p("Available memory : %uB\n", perfinfo.available_heap);
	debug_p("Uptime           : %ums\n", perfinfo.uptime);
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
