/*
 * mgr_signal.c
 *
 *  Created on: Feb 4, 2024
 *      Author: Zviad
 */

#include <string.h>
#include "debug.h"
#include "defines.h"
#include "mgr_signal.h"
#include "mw_io.h"
#include "mw_timebase.h"
#include "utils.h"

#define MAX_PINS  16
#define MAX_GPIOS 8

typedef struct {
    signal_param_t* pin_params[MAX_PINS];
} pin_params_t;

static pin_params_t _gpio_sig_lookup_table[MAX_GPIOS];

tim_handle_t htim10;
static signal_measurement_stats_t _measurement_stats;
static void _set_value(uint16_t pin, uint64_t tick);

void mgr_signal_init() {
    memset((void*) _gpio_sig_lookup_table, 0, sizeof(_gpio_sig_lookup_table));
}

bool_t mgr_signal_add(signal_param_t* params) {
    int8_t gpio_id = mw_gpio_get_id(params->gpio.gpiox);
    if (gpio_id == -1 || gpio_id >= MAX_GPIOS) {
        debug_error("Signal GPIO id is wrong\n");
        return false;
    }

    uint8_t pin_pos = singlebitpos(params->gpio.pin);
    if (pin_pos >= MAX_PINS) {
        debug_error("Signal GPIO pin %i is out of range\n", pin_pos);
        return false;
    }

    _gpio_sig_lookup_table[gpio_id].pin_params[pin_pos] = params;

    /*Configure GPIO for pin */
    gpio_init_t gpio = { 0 };
    gpio.gpiox = params->gpio.gpiox;
    gpio.init.Pin = params->gpio.pin;
    gpio.init.Mode = params->gpio.mode;
    gpio.init.Pull = params->gpio.pull;
    mw_gpio_init(&gpio);
    if (params->use_interrupts) {
        mw_gpio_nvic_init(params->nvic.irq, params->nvic.preempt_priority,
                params->nvic.sub_priority);
    }

    debug_info("Signal item for P%c%i (%s) added successfully\n",
            mw_gpio_get_char(params->gpio.gpiox), pin_pos, params->name);
    return true;
}

signal_measurement_stats_t* mgr_signal_stats_get() {
    return &_measurement_stats;
}

static void _set_value(uint16_t pin, uint64_t tick) {
    uint8_t pin_pos = singlebitpos(pin);

    for (uint8_t i = 0; i < MAX_GPIOS; i++) {
        signal_param_t* param = _gpio_sig_lookup_table[i].pin_params[pin_pos];
        if (param == NULL) {
            continue;
        }

        if (param->state == SignalMeasuringReady) {
            if (param->detection_type == DetectSignalHigh) {
                if (param->gpio.gpiox->IDR & param->gpio.pin) {
                    param->state = SignalMeasuringStarted;
                    param->previous_tick = tick;
                    break;
                }
            }
        } else if (param->state == SignalMeasuringStarted) {
            if (param->detection_type == DetectSignalHigh) {
                if (!(param->gpio.gpiox->IDR & param->gpio.pin)) {
                    param->state = SignalMeasuringFinished;
                    param->measured_length = tick - param->previous_tick;
                    break;
                }
            }
        }
    }
}

void EXTI9_5_IRQHandler(void) {
    uint64_t tick = mw_timebase_ticks_get_us();

    _set_value(GPIO_PIN_5, tick);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void) {

    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void EXTI0_IRQHandler(void) {
    uint64_t tick = mw_timebase_ticks_get_us();
    _set_value(GPIO_PIN_0, tick);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void) {
    uint64_t tick = mw_timebase_ticks_get_us();
    _set_value(GPIO_PIN_1, tick);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void) {
    uint64_t tick = mw_timebase_ticks_get_us();
    _set_value(GPIO_PIN_2, tick);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void) {
    uint64_t tick = mw_timebase_ticks_get_us();
    _set_value(GPIO_PIN_3, tick);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void) {
    uint64_t tick = mw_timebase_ticks_get_us();
    _set_value(GPIO_PIN_4, tick);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
