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
#include "FreeRTOSConfig.h"
#include "mw_rtc.h"
#include "mgr_adc.h"
#include "mgr_gprs.h"
#include "mgr_rtc.h"
#include "mgr_tasks.h"
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
static void _gprs(void* args);
static void _kill(void* args);
static void _create(void* args);
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
	{ "gprs", _gprs, "Controls GPRS modem" },
	{ "kill", _kill, "Kills the provided task by its name" },
	{ "create", _create, "Creates the task by provided name and parameters" },
	{ "perfinfo", _perfinfo, "Gets the information about current performance" },
	{ "debuglevel", _dbglevel, "Gets or sets debug level" },
	{ "version", _version, "Prints current version" },
};/* @formatter:on */

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
        debug_p("The datetime command requires arguments\n");
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
    uint32_t time_us = prf_func_exec_time_get(cmd_process(args));
    debug_p("PERF: Execution took %lu us\n", time_us);
}

static char* _move_to_next_word(char* str, char* out_word, int max_len) {
    max_len -= 1;
    if (*str == '\0') {
        return NULL;
    }
    int len = 0;
    do {
        if (*str == ' ') {
            str++;
            break;
        }
        if (len++ > max_len) {
            debug_error("Argument must be shorter than %i characters\n",
                    max_len + 1);
            return NULL;
        }
        *out_word = *str;
        out_word++;

    } while (*str++);
    *out_word = '\0';
    return str;
}

static bool_t _extract_number_from_next_word(char** str, uint32_t* number) {
    if (!number) {
        return false;
    }
    char out_word[10];
    *str = _move_to_next_word(*str, out_word, sizeof(out_word));
    if (*str == NULL || !is_integer(out_word)) {
        return false;
    }
    *number = (unsigned) atoi(out_word);
    return true;
}

static void _adc(void* args) {
    adc_uiconf_t* adc_uiconf = mgr_adc_get_uiconf();
    adc_uiconf->ref_voltage = CMD_ADC_REF_VOLTAGE;
    adc_uiconf->permanent = true;
    char out_word[10] = { 0 };
    char* wordptr = args;
    int arg_counter = 0;
    while (wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word)), wordptr
            != NULL) {
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
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            bool_t is_ok = wordptr != NULL && is_integer(out_word);
            if (!is_ok) {
                debug_error("Argument `delay` must be followed by number\n");
                return;
            }
            adc_uiconf->delay = atoi(out_word);
        } else if (!strncmp(out_word, "amount", 6)) {
            arg_counter++;
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
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

static void _gprs(void* args) {
    char out_word[21] = { 0 };
    char* wordptr = args;
    int arg_counter = 0;
    uint32_t timeout = 100;
    char expecting[sizeof(out_word)] = { 0 };
    while (wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word)), wordptr
            != NULL) {
        if (!strncmp(out_word, "call", 4)) {
            arg_counter++;
            uint32_t country_code = 0, number = 0;
            bool_t is_ok = _extract_number_from_next_word(&wordptr,
                    &country_code);
            is_ok &= _extract_number_from_next_word(&wordptr, &number);
            if (!is_ok) {
                debug_error(
                        "Argument `call` must be followed by country code (without `+`) and number\n");
                return;
            }
            gprs_call(country_code, number);
            return;
        } else if (!strncmp(out_word, "sms", 3)) {
            arg_counter++;
            uint32_t country_code = 0, number = 0;
            bool_t is_ok = _extract_number_from_next_word(&wordptr,
                    &country_code);
            is_ok &= _extract_number_from_next_word(&wordptr, &number);
            if (!is_ok) {
                debug_error(
                        "Argument `sms` must be followed by country code (without `+`) and number\n");
                return;
            }
            gprs_sms_send(country_code, number, wordptr);
            return;
        } else if (!strncmp(out_word, "rssi", 4)) {
            arg_counter++;
            gprs_rssi_percentage(mgr_gprs_process_output);
        } else if (!strncmp(out_word, "date", 4)) {
            arg_counter++;
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            if (!strncmp(out_word, "get", 3)) {
                gprs_time_get(mgr_gprs_process_output);
            } else if (!strncmp(out_word, "set", 3)) {
                wordptr = _move_to_next_word(wordptr, out_word,
                        sizeof(out_word));
                if (!strlen(out_word)) {
                    debug_error(
                            "Argument set must be followed by date in following format: YY/MM/DD,HH:mm:ss+ZZ\n");
                    return;
                }
                gprs_time_set(out_word);
            } else {
                debug_error(
                        "Argument `date` must be followed by `get` or `set`\n");
                return;
            }
            return;
        } else if (!strncmp(out_word, "join", 4)) {
            char* apn = wordptr;
            char* user = _move_to_next_word(apn, out_word, sizeof(out_word));

            char* password = _move_to_next_word(user, out_word,
                    sizeof(out_word));
            /* If user and password are null pointers initialize them */
            if (!user) {
                user = apn + strlen(apn);
            }
            if (!password) {
                password = user + strlen(user);
            }
            /* Hack! split string into shorter strings to save memory
             * as we don't have to declare additional arrays to keep apn user and password */
            *(user - 1) = '\0';
            *(password - 1) = '\0';

            gprs_join(apn, user, password);
            return;
        } else if (!strncmp(out_word, "restart", 7)) {
            arg_counter++;
            gprs_restart();
        } else if (!strncmp(out_word, "tcp", 3)) {
            char ip[16] = { 0 };
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            bool_t is_ok = wordptr != NULL;
            if (!is_ok || strlen(out_word) > sizeof(ip)) {
                goto invalid_args;
            }
            memcpy(ip, out_word, strlen(out_word));
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            is_ok = wordptr != NULL && is_integer(out_word);
            if (!is_ok) {
                goto invalid_args;
            }
            uint16_t port = atoi(out_word);
            gprs_send_tcp_data(ip, port, (uint8_t* ) wordptr, strlen(wordptr));
            gprs_connection_close(0);
            goto normal_return;
            invalid_args:
            debug_error(
                    "Argument `tcp` must be followed by ip, port and data\n");
            normal_return: return;
        } else if (!strncmp(out_word, "server", 6)) {
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            bool_t is_ok = wordptr != NULL && is_integer(out_word);
            if (!is_ok) {
                debug_error("Argument `server` must be followed by port\n");
                return;
            }
            uint16_t port = atoi(out_word);
            gprs_start_server(port);
            return;
        } else if (!strncmp(out_word, "timeout", 7)) {
            arg_counter++;
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            bool_t is_ok = wordptr != NULL && is_integer(out_word);
            if (!is_ok) {
                debug_error("Argument `timeout` must be followed by number\n");
                return;
            }
            timeout = atoi(out_word);
        } else if (!strncmp(out_word, "expect", 6)) {
            arg_counter++;
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            if (wordptr == NULL) {
                debug_error(
                        "Argument `expect` must be followed by the expected string\n");
                return;
            }
            memcpy(expecting, out_word, sizeof(expecting));
        } else if (!strncmp(out_word, "cmd", 3)) {
            arg_counter++;
            uint32_t len = strlen(wordptr);
            wordptr[len] = '\r';
            wordptr[len + 1] = '\n';
            sim800_cmd_send(wordptr, expecting, timeout, 0);
            /* cmd must be the last command. Returning */
            return;
        } else if (!strncmp(out_word, "debug", 3)) {
            arg_counter++;
            wordptr = _move_to_next_word(wordptr, out_word, sizeof(out_word));
            if (!strncmp(out_word, "on", 2)) {
                gprs_debug_set(true);
            } else if (!strncmp(out_word, "off", 3)) {
                gprs_debug_set(false);
            } else {
                debug_error(
                        "Argument `debug` must be followed by `on` or `off`\n");
                return;
            }
        }
    }
    if (!arg_counter) {
        /* @formatter:off */
        debug_p("The gprs command requires arguments. They can be:\n");
        debug_p("    call    - calls to the provided number\n");
        debug_p("    sms     - sends provided message to the provided number e.g. sms number <<message text>>\n");
        debug_p("    rssi    - gets radio signal strength value in percents\n");
        debug_p("    date    - gets or sets date and time on GPRS module\n");
        debug_p("    join    - join GPRS context\n");
        debug_p("    tcp     - send TCP data\n");
        debug_p("    server  - starts TCP server\n");
        debug_p("    timeout - used for cmd argument when sending a raw command how much to wait the response\n");
        debug_p("    expect  - used for cmd argument when sending a raw command what answer do we expect\n");
        debug_p("    cmd     - raw command. Must be the last argument\n");
        debug_p("    debug   - debugs gprs driver. Can be on or off\n");
        debug_p("    restart - restarts gprs module using the reset pin\n");/* @formatter:on */
    }
    return;
}

static void _kill(void* args) {
    if (mgr_tasks_kill_by_name(args)) {
        debug_p("Task '%s' successfully terminated\n", args);
    } else {
        debug_error("Task '%s' couldn't be terminated\n", args);
    }
}

static void _create(void* args) {
    char* token = NULL;
    char* rest = args;
    int arg_counter = 0;
    char* task_name = NULL;
    char* reg_name = NULL;
    uint16_t stack_depth = 128;
    uint32_t priority = 1;
    while (token = strtok_r(rest, " ", &rest), token != NULL) {
        if (!strncmp(token, "task", 4)) {
            token = strtok_r(rest, " ", &rest);
            task_name = token;
            arg_counter++;
        } else if (!strncmp(token, "name", 4)) {
            token = strtok_r(rest, " ", &rest);
            reg_name = token;
            arg_counter++;
        } else if (!strncmp(token, "stack", 5)) {
            token = strtok_r(rest, " ", &rest);
            stack_depth = atoi(token);
            arg_counter++;
        } else if (!strncmp(token, "priority", 8)) {
            token = strtok_r(rest, " ", &rest);
            priority = atol(token);
            arg_counter++;
        }
    }
    if (!arg_counter) {
        debug_p("The create command requires arguments. They can be:\n");
        debug_p("    task     - name of the task from tasks list\n");
        debug_p("    name     - name of the task after creation\n");
        debug_p("    stack    - depth of the task's stack in bytes\n");
        debug_p("    priority - task priority from 0 to %i\n",
                configMAX_PRIORITIES);
        mgr_tasks_list_available_runnables();
        return;
    }
    mgr_tasks_create_by_name(task_name, reg_name, stack_depth, priority, NULL);
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
    debug_p("Unknown command '%s'. Type help\n", input);
}
