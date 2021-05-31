/*
 * cli.c
 *
 *  Created on: Jan 21, 2020
 *      Author: Zviad
 */

#include <cli.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
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
#include "mgr_mcron.h"
#include "mgr_version.h"
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
static void _mcron(void* args);
static void _repeat(void* args);
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
	{ "mcron", _mcron, "Creates the Mikiot cron task" },
	{ "repeat", _repeat, "Repeats the provided task by its name"},
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
    mikiot_versions_t* versions = mgr_versions_get();
    debug_p("Mikiot firmware\n");
    debug_p(" FW version      : %i.%i.%i\n", versions->fw_version.main,
            versions->fw_version.sub1, versions->fw_version.sub2);
    debug_p(" Code name       : %s\n", versions->fw_code_name);
    debug_p(" Release date    : %s\n", versions->fw_release_date);
    debug_p(" FreeRTOS vesion : %i.%i.%i\n", versions->rtos_version.main,
            versions->rtos_version.sub1, versions->rtos_version.sub2);
    debug_p(" HAL version     : %i.%i.%i.%i\n", versions->hal_version.main,
            versions->hal_version.sub1, versions->hal_version.sub2,
            versions->hal_version.sub3);
}

static void _play_blob(void* args) {
    debug_p("Playing the blob\n");
    /* Divide by 3 because 2 hex digits represent 1 byte and there is one space.
     * And add one at the end because the last one doesn't have a space
     * TODO: Until tests will be done example blob command is bellow:
     * blob 15 00 00 00 00 00 04 00 AA 10 00 00 00 00 04 00 AA 02 00 00 11 */
    static uint8_t blob_instance = 0;
    char name_to_assign[configMAX_TASK_NAME_LEN];
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
    sprintf(name_to_assign, "BLOB_PLAYER%i", ++blob_instance);
    mgr_tasks_create_by_name("blob", name_to_assign,
    configMINIMAL_STACK_SIZE + len, 1, blob_bytes);
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
    uint32_t time_us = prf_func_exec_time_get(cli_cmd_process(args));
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
    char* task_name = (char*) args;
    if (!strlen(task_name)) {
        debug_p(
                "The kill command requires argument task name. They can be found in taskmgr\n");
        return;
    }
    if (mgr_tasks_kill_by_name(task_name)) {
        debug_p("Task '%s' successfully terminated\n", task_name);
    } else {
        debug_error("Task '%s' couldn't be terminated\n", task_name);
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
    if (mgr_tasks_create_by_name(task_name, reg_name, stack_depth, priority,
    NULL)) {
        debug_p("The task `%s` (aka `%s`) created successfully\n", task_name,
                reg_name);
    } else {
        debug_p("Task creation failed\n");
    }
}

static void _mcron(void* args) {
    char* token = NULL;
    char* rest = args;
    mcron_item_t mcron = {
            .args = NULL, .fn = NULL, .free = NULL, .id = 0,
            .setup.ticks.value = 60000, .variant = 0 };
    /* The mask to control which arguments are passed. Some of them are more prioritized than others */
    uint8_t setup_mask = 0;
    while (token = strtok_r(rest, " ", &rest), token != NULL) {
        if (!strncmp(token, "add", 3)) {
            while (token = strtok_r(rest, " ", &rest), token != NULL) {
                if (!strncmp(token, "cmd", 2)) {
                    mcron.fn = cli_cmd_process;
                    uint32_t len = strlen(rest);
                    if (len == 0) {
                        debug_p(
                                "The argument `cmd` must be followed by the cli string\n");
                        return;
                    }
                    /* Copy also null terminator */
                    mgr_mcron_deep_copy_args(&mcron, rest, len + 1);
                    set_bit(setup_mask, 7);
                    /* Break loop here, as `cmd` must be the last argument. */
                    break;
                } else if (!strncmp(token, "variant", 7)) {
                    token = strtok_r(rest, " ", &rest);
                    uint32_t variant = mgr_mcron_description_to_variant(token);
                    if (strlen(token) == 0 || variant == 0) {
                        debug_p(
                                "The argument `variant` must be followed by one of these following:\n");
                        mgr_mcron_time_setup_variants_print();
                        return;
                    }
                    mcron.variant = variant;
                    set_bit(setup_mask, 6);
                } else if (!strncmp(token, "timebase", 8)) {
                    if (mcron.variant == 0) {
                        debug_p(
                                "Before `timebase` the `variant` argument must be provided\n");
                        return;
                    }
                    if (mcron.variant == McronTaskVariantPeriodic
                            || mcron.variant == McronTaskVariantDelayed) {
                        token = strtok_r(rest, " ", &rest);
                        if (!is_integer(token)) {
                            debug_p(
                                    "The `timebase` argument for periodic and delayed tasks must be followed by a number\n");
                            return;
                        }
                        mcron.setup.ticks.value = atoi(token);
                    }
                    if (mcron.variant == McronTaskVariantPeriodicLimited) {
                        token = strtok_r(rest, " ", &rest);
                        char* token2 = strtok_r(rest, " ", &rest);
                        if (!is_integer(token) || !is_integer(token2)) {
                            debug_p(
                                    "The `timebase` argument for `limited` variant must be followed by two numbers `delay` `count`\n");
                            return;
                        }
                        mcron.setup.ticks.value = atoi(token);
                        mcron.setup.ticks.exec_count =
                                mcron.target.ticks.exec_count = atoi(token2);
                    }
#if (MCONF_RTC_ON == 1)
                    rtc_date_t rtc_date = { 0 };
                    rtc_time_t rtc_time = { 0 };
                    mgr_rtc_get_time(&rtc_time);
                    mgr_rtc_get_date(&rtc_date);
                    if (mcron.variant == McronTaskVariantHourly) {
                        mcron.setup.exec_date.hour = rtc_time.Hours + 1;
                    } else if (mcron.variant == McronTaskVariantDaily) {
                        mcron.setup.exec_date.day = rtc_date.Date + 1;
                    } else if (mcron.variant == McronTaskVariantWeekly) {
                        token = strtok_r(rest, " ", &rest);
                        if (!is_integer(token)) {
                            debug_p(
                                    "The `timebase` argument for weekday tasks must be followed by a number between 0 and 6\n");
                            return;
                        }
                        mcron.setup.exec_date.weekday = atoi(token);
                        if (!is_range(mcron.setup.exec_date.weekday, 0, 6)) {
                            debug_p("Weekday must be between 0, 6\n");
                        }
                    } else if (mcron.variant == McronTaskVariantMonthly) {
                        mcron.setup.exec_date.month = rtc_date.Month + 1;
                    } else if (mcron.variant == McronTaskVariantYearly) {
                        mcron.setup.exec_date.year = rtc_date.Year + 1;
                    } else if (mcron.variant == McronTaskVariantOnceOnDate) {
                        int day, month, year, hour, minute, second;
                        sscanf(rest, "%d.%d.%d-%d:%d:%d", &day, &month, &year,
                                &hour, &minute, &second);
                        ASSERT_DATE_ELEMENT(month, 1, 12);
                        ASSERT_DATE_ELEMENT(year, 0, 99);
                        ASSERT_DATE_ELEMENT(day, 1,
                                mgr_rtc_get_max_days_in_month(month, year));
                        ASSERT_DATE_ELEMENT(hour, 0, 23);
                        ASSERT_DATE_ELEMENT(minute, 0, 59);
                        ASSERT_DATE_ELEMENT(second, 0, 59);
                        mcron.setup.exec_date.day = day;
                        mcron.setup.exec_date.month = month;
                        mcron.setup.exec_date.year = year;
                        mcron.setup.exec_date.hour = hour;
                        mcron.setup.exec_date.minute = minute;
                        mcron.setup.exec_date.second = second;
                    }
#endif
                    set_bit(setup_mask, 5);
                }
            }
            if (setup_mask >= 192) {
                mgr_mcron_add(&mcron);
                return;
            } else {
                debug_p(
                        "To set up mcron must be cmd and variant provided. Mask=%i\n",
                        setup_mask);
            }
        } else if (!strncmp(token, "remove", 6)) {
            token = strtok_r(rest, " ", &rest);
            if (!is_integer(token)) {
                debug_p(
                        "To remove the mcron task mcron id must be provided. Check mcron list command\n");
                return;
            }
            mcron.id = atoi(token);
            mgr_mcron_remove_by_item_id(&mcron);
            return;
        } else if (!strncmp(token, "list", 4)) {
            mgr_mcron_tasks_list_print();
            return;
        }
    }

    debug_p("The mcron command needs arguments. They can be:\n");
    debug_p(
            "    add    - adds the new mcron task, is followed by: `variant`, `timebase` and `cmd` arguments\n");
    debug_p("    remove - removes the existing mcron task\n");
    debug_p("    list   - lists all mcron task\n");
}

static void _repeat(void* args) {
    char* token = NULL;
    char* rest = args;
    uint32_t len = 0;
    char* cmd = NULL;
    int32_t amount = 0;
    int32_t delay = 1000;
    int arg_counter = 0;
    while (token = strtok_r(rest, " ", &rest), token != NULL) {
        if (!strncmp(token, "amount", 6)) {
            token = strtok_r(rest, " ", &rest);
            if (!is_integer(token)) {
                debug_error("argument amount must be followed by the number\n");
                return;
            }
            amount = atoi(token);
        } else if (!strncmp(token, "delay", 5)) {
            token = strtok_r(rest, " ", &rest);
            if (!is_integer(token)) {
                debug_error("argument delay must be followed by the number\n");
                return;
            }
            delay = atoi(token);
        } else if (!strncmp(token, "cmd", 3)) {
            len = strlen((char*) rest) + 1;
            cmd = mconf_malloc(len);
            if (cmd == NULL) {
                debug_error("Can not allocate %i bytes for repeat buffer\n",
                        len);
                return;
            }
            memset(cmd, 0, len);
            memcpy(cmd, rest, len - 1);
            debug_p("Repeating cmd  : '%s'\n", cmd);
            for (int i = 0; i < amount; i++) {
                cli_cmd_process(cmd);
                mconf_delay(delay);
            }
            arg_counter++;
        }
    }
    if (!arg_counter) {
        debug_p("The repeat command requires arguments:\n");
        debug_p("    amount   - amount of repeats\n");
        debug_p("    delay    - delay between repeats\n");
        debug_p("    cmd      - cmd to execute n times\n");
        debug_p("Usage: repeat amount 10 delay 1000 cmd echo test\n");
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

void cli_cmd_process(void* input) {
    for (int i = 0; i < array_count(commands); i++) {
        if (!strncmp(commands[i].cmd, (char*) input,
                strlen((char*) commands[i].cmd))) {
            commands[i].fp(input + strlen(commands[i].cmd) + 1);
            return;
        }
    }
    debug_p("Unknown command '%s'. Type help\n", input);
}
