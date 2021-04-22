/*
 * mgr_inputs.h
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#ifndef MANAGERS_MGR_INPUTS_H_
#define MANAGERS_MGR_INPUTS_H_

#include "mw_io.h"

tim_handle_t htim11;

typedef enum {
    InputIdleStateLow = 0, /* Input low idle state. Triggers when the low-high transition happens */
    InputIdleStateHigh = 1 /* Input high idle state. Triggers when the high-low transition happens */
} input_enum_t;

typedef enum {
    InputStateLow = 0, /* Logical low state of the input */
    InputStateHigh = 1 /* Logical high state of the input */
} input_state_enum_t;

typedef struct {
    gpio_t gpio;
    input_enum_t type;
    uint32_t on_count;
    uint32_t off_count;
    input_state_enum_t state;
    input_state_enum_t confirmed_state;
    uint32_t top_lvl;
    nvic_param_t nvic;
    const char name[15];
} input_t;

#define timer_init_typedef TIM_HandleTypeDef

/*
 * Initialization of GPIO and timer
 */
void mgr_inputs_init(uint32_t tim_period_us, void* fp);

/*
 * Handles input high-low transition or vice versa depending on input idle type respectively
 */
void mgr_input_trigger_handle();

/*
 * Detects input state high or low depending on input state
 */
void mgr_input_detect_state(uint16_t gpio_pin);

#endif /* MANAGERS_MGR_INPUTS_H_ */
