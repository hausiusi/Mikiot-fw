/*
 * mgr_gprs.c
 *
 *  Created on: Sep 8, 2020
 *      Author: Zviad
 */

#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "macrodefs.h"
#include "mgr_gprs.h"

typedef void (*at_func_t)(void*);

typedef struct {
    char* at;
    at_func_t fp;
} at_parser_struct_t;

static void _rssi_percents(void* data);
static void _ring(void* data);
static void _parse_clock(void* data);

/* @formatter:off */
static at_parser_struct_t _parsers[] = {
		{"+CSQ:", _rssi_percents},
		{"RING", _ring},
		{"+CCLK:", _parse_clock},
};
/* @formatter:on */

bool_t mgr_gprs_init(gprs_state_t* state, bool_t enable_debug) {
    gprs_debug_set(enable_debug);
    return gprs_init(state);
}

void mgr_gprs_process_output(gprs_state_t* state) {
    char* rx_buffer = (char*) state->rx_buffer + 2;
    for (int i = 0; i < array_count(_parsers); i++) {
        if (!strncmp(_parsers[i].at, (char*) rx_buffer,
                strlen((char*) _parsers[i].at))) {
            _parsers[i].fp(rx_buffer + strlen(_parsers[i].at) + 1);
            return;
        }
    }
    debug_p("MGR GPRS: Can't parse \"%s\"\n", rx_buffer);
}

static void _rssi_percents(void* data) {
    mgr_gprs_status.rssi_percent = (atoi(data) * 827 + 127) >> 8;
    debug_p("RSSI: %i\n", mgr_gprs_status.rssi_percent);
}

static void _ring(void* data) {
    debug_p("RINGING from:\n");
}

static void _parse_clock(void* data) {
    char* p = data + 1;
    mgr_gprs_status.date.Year = (uint16_t) atoi(p);
    mgr_gprs_status.date.Month = (uint8_t) atoi(p + 3);
    mgr_gprs_status.date.Date = (uint8_t) atoi(p + 6);
    mgr_gprs_status.time.Hours = (uint8_t) atoi(p + 9);
    mgr_gprs_status.time.Seconds = (uint8_t) atoi(p + 12);
    mgr_gprs_status.time.Minutes = (uint8_t) atoi(p + 15);
    mgr_gprs_status.time_zone = (uint8_t) atoi(p + 18);
}

