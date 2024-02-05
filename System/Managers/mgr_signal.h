/*
 * mgr_signal.h
 *
 *  Created on: Feb 4, 2024
 *      Author: Zviad
 */

#ifndef MANAGERS_MGR_SIGNAL_H_
#define MANAGERS_MGR_SIGNAL_H_

#include "defines.h"
#include "mw_io.h"
#include <inttypes.h>

typedef void (*signal_dectection_callback_fp)(void* params);

/* @formatter:off */
typedef enum {
    DetectSignalLow = 0,
    DetectSignalHigh = 1,
    DetectSignalBoth = 2
} signal_detection_type_enum_t;

typedef enum {
    SignalMeasuringReady = 0,
    SignalMeasuringStarted = 1,
    SignalMeasuringFinished = 2,
} signal_measure_state_enum_t;

/* @formatter:on */

typedef struct {
    gpio_t gpio;
    signal_detection_type_enum_t detection_type;
    signal_measure_state_enum_t state;
    uint64_t previous_tick; /* Tick when previously signal change was detected */
    uint32_t measured_length; /* The most recent signal length*/
    bool_t use_interrupts;
    nvic_param_t nvic;
    signal_dectection_callback_fp callback;
    const char name[15];
} signal_param_t;

typedef struct {
    uint32_t total_time_spent;
    uint32_t measurements_count;
    uint32_t misses_count;
} signal_measurement_stats_t;

void mgr_signal_init();
bool_t mgr_signal_add(signal_param_t* params);
linked_list_t* mgr_signal_params_list_get();
signal_measurement_stats_t* mgr_signal_stats_get();

#endif /* MANAGERS_MGR_SIGNAL_H_ */
