/*
 * mgr_signal.c
 *
 *  Created on: Feb 4, 2024
 *      Author: Zviad
 */

#include "debug.h"
#include "defines.h"
#include "lib_linked_list.h"
#include "mgr_signal.h"
#include "mw_io.h"
#include "mw_timebase.h"
#include "utils.h"

tim_handle_t htim10;
static linked_list_t* _signal_params;
static signal_measurement_stats_t _measurement_stats;
static bool_t _equals(void* a, void* b);
static void _set_value(uint16_t pin);

void mgr_signal_init() {
    _signal_params = ll_init();
    _signal_params->equals = _equals;
}

bool_t mgr_signal_add(signal_param_t* params) {
    ll_node_t* node = ll_get_node(params, sizeof(signal_param_t));
    if (!ll_add(_signal_params, node)) {
        debug_error("Signal parameter adding failed\n");
        return false;
    }

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

    debug_info("Signal item for P%c%i added successfully\n",
            mw_gpio_get_char(params->gpio.gpiox),
            singlebitpos(params->gpio.pin));
    return true;
}

linked_list_t* mgr_signal_params_list_get() {
    return _signal_params;
}

signal_measurement_stats_t* mgr_signal_stats_get() {
    return &_measurement_stats;
}

static bool_t _equals(void* a, void* b) {
    signal_param_t* sig_a = (signal_param_t*) a;
    signal_param_t* sig_b = (signal_param_t*) b;

    return sig_a->gpio.pin == sig_b->gpio.pin
            && sig_a->gpio.gpiox == sig_b->gpio.gpiox;
}

static void _set_value(uint16_t pin) {
    ll_node_t* node = _signal_params->head;
    if ((uint32_t) node == 0x20020000) {
        _measurement_stats.misses_count++;
        return;
    }

    prf_start_measurement();
    do {
        signal_param_t* param = (signal_param_t*) node->item;
        if (param == NULL) {
            return;
        }

        if (param->gpio.pin == pin) {
            if (param->state == SignalMeasuringReady) {
                if (param->detection_type == DetectSignalHigh) {
                    if (mw_gpio_readpin(param->gpio.gpiox, param->gpio.pin)
                            == GPIO_PIN_SET) {
                        param->previous_tick = mw_timebase_ticks_get_us();
                        param->state = SignalMeasuringStarted;
                        break;
                    }
                }
            } else if (param->state == SignalMeasuringStarted) {
                if (param->detection_type == DetectSignalHigh) {
                    if (mw_gpio_readpin(param->gpio.gpiox, param->gpio.pin)
                            == GPIO_PIN_RESET) {
                        param->measured_length = mw_timebase_ticks_get_us()
                                - param->previous_tick;
                        param->state = SignalMeasuringFinished;
                        break;
                    }
                }
            }
        }

        node = node->next;
    } while (node != NULL);

    _measurement_stats.total_time_spent += prf_end_measurement();
    _measurement_stats.measurements_count++;
}

void EXTI9_5_IRQHandler(void) {
    _set_value(GPIO_PIN_5);
    _set_value(GPIO_PIN_6);
    _set_value(GPIO_PIN_7);
    _set_value(GPIO_PIN_8);
    _set_value(GPIO_PIN_9);
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

void EXTI3_IRQHandler(void) {
    _set_value(GPIO_PIN_3);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void) {
    _set_value(GPIO_PIN_4);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
