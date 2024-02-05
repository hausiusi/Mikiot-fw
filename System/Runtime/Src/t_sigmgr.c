/*
 * t_sigmgr.c
 *
 *  Created on: Feb 4, 2024
 *      Author: Zviad
 */

#include "ioconfig.h"
#include "lib_linked_list.h"
#include "mgr_signal.h"
#include "os_loader.h"
#include "utils.h"

static void _process_signal_measurements(linked_list_t* params_list);

/* @formatter:off */
signal_param_t _sig_params[] = {
    {
        .gpio.gpiox = SIG1_GPIO,
        .gpio.pin = SIG1_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI3_IRQn,
        .nvic.preempt_priority = 15,
        .nvic.sub_priority = 15,
        .name = "SIG1_IN"
    },
    {
        .gpio.gpiox = SIG2_GPIO,
        .gpio.pin = SIG2_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI4_IRQn,
        .nvic.preempt_priority = 15,
        .nvic.sub_priority = 15,
        .name = "SIG2_IN"
    },
    {
        .gpio.gpiox = SIG3_GPIO,
        .gpio.pin = SIG3_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 15,
        .nvic.sub_priority = 15,
        .name = "SIG3_IN"
    },
    {
        .gpio.gpiox = SIG4_GPIO,
        .gpio.pin = SIG4_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 15,
        .nvic.sub_priority = 15,
        .name = "SIG4_IN"
    },
    {
        .gpio.gpiox = SIG5_GPIO,
        .gpio.pin = SIG5_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 15,
        .nvic.sub_priority = 15,
        .name = "SIG5_IN"
    },
    {
        .gpio.gpiox = SIG6_GPIO,
        .gpio.pin = SIG6_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 15,
        .nvic.sub_priority = 15,
        .name = "SIG6_IN"
    }
};
/* @formatter:on */

extern void thread_sigmgr(void* args) {
    mgr_signal_init();
    vTaskDelay(1000);
    for (uint8_t i = 0; i < COUNT(_sig_params); i++) {
        mgr_signal_add(&_sig_params[i]);
    }

    vTaskDelay(10);
    linked_list_t* params_list = mgr_signal_params_list_get();
    for (;;) {
        _process_signal_measurements(params_list);

        vTaskDelay(500);
    }
}

void _process_signal_measurements(linked_list_t* params_list) {
    foreach(ll_node_t, node2, params_list) {
        signal_param_t* param = (signal_param_t*) node2->item;
        if (param->state == SignalMeasuringFinished) {
            debug_info("Signal for P%c%i measured as %uus\n",
                    mw_gpio_get_char(param->gpio.gpiox),
                    singlebitpos(param->gpio.pin), param->measured_length);
            param->state = SignalMeasuringReady;
        }
    }

    signal_measurement_stats_t* stats = mgr_signal_stats_get();
    debug_info(
            "Signal measurement stats - count: %u, misses: %u, total time: %uus\n",
            stats->measurements_count, stats->misses_count,
            stats->total_time_spent);
}
