/*
 * t_sigmgr.c
 *
 *  Created on: Feb 4, 2024
 *      Author: Zviad
 */

#include "ioconfig.h"
#include "mgr_signal.h"
#include "os_loader.h"
#include "utils.h"
#include "pwm.h"

#define RC_SIG_AILERON      SignalInputOne
#define RC_SIG_ELEVATOR     SignalInputTwo
#define RC_SIG_THROTTLE     SignalInputThree
#define RC_SIG_RUDDER       SignalInputFour
#define RC_SIG_VRB          SignalInputFive
#define RC_SIG_VRA          SignalInputSix

static void _process_signal_measurements();

/* @formatter:off */
static signal_param_t _sig_params[] = {
    {
        .gpio.gpiox = SIG1_GPIO,
        .gpio.pin = SIG1_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI3_IRQn,
        .nvic.preempt_priority = 5,
        .nvic.sub_priority = 6,
        .name = "SIG1_IN",
        .input_name_id = RC_SIG_AILERON
    },
    {
        .gpio.gpiox = SIG2_GPIO,
        .gpio.pin = SIG2_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI4_IRQn,
        .nvic.preempt_priority = 5,
        .nvic.sub_priority = 6,
        .name = "SIG2_IN",
        .input_name_id = RC_SIG_ELEVATOR
    },
    {
        .gpio.gpiox = SIG3_GPIO,
        .gpio.pin = SIG3_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 5,
        .nvic.sub_priority = 6,
        .name = "SIG3_IN",
        .input_name_id = RC_SIG_THROTTLE
    },
    {
        .gpio.gpiox = SIG4_GPIO,
        .gpio.pin = SIG4_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 5,
        .nvic.sub_priority = 6,
        .name = "SIG4_IN",
        .input_name_id = RC_SIG_RUDDER
    },
    {
        .gpio.gpiox = SIG5_GPIO,
        .gpio.pin = SIG5_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 5,
        .nvic.sub_priority = 6,
        .name = "SIG5_IN",
        .input_name_id = RC_SIG_VRB
    },
     {
        .gpio.gpiox = SIG6_GPIO,
        .gpio.pin = SIG6_GPIO_PIN,
        .gpio.mode = GPIO_MODE_IT_RISING_FALLING,
        .gpio.pull = GPIO_NOPULL,
        .detection_type = DetectSignalHigh,
        .use_interrupts = true,
        .nvic.irq = EXTI9_5_IRQn,
        .nvic.preempt_priority = 5,
        .nvic.sub_priority = 6,
        .name = "SIG6_IN",
        .input_name_id = RC_SIG_VRA
    }
};
/* @formatter:on */

extern void thread_sigmgr(void* args) {
    mgr_signal_init();
    vTaskDelay(1000);
    for (uint8_t i = 0; i < COUNT(_sig_params); i++) {
        mgr_signal_add(&_sig_params[i]);
    }

    pwm_init();

    vTaskDelay(10);
    for (;;) {
        _process_signal_measurements();

        vTaskDelay(1);
    }
}

void _process_signal_measurements() {
    for (uint8_t i = 0; i < COUNT(_sig_params); i++) {
        signal_param_t* param = &_sig_params[i];
        if (param->state == SignalMeasuringFinished) {
            /* debug_info("Signal for P%c%i measured as %uus\n",
             mw_gpio_get_char(param->gpio.gpiox),
             singlebitpos(param->gpio.pin), param->measured_length); */

            int32_t pulse = map_int(param->measured_length, 1000, 2000, 0,
                    65535);

            /* debug_info("Signal value mapped for PWM as %i\n", pulse); */

            pwm_set_value(param->input_name_id, pulse);
            param->state = SignalMeasuringReady;
        }
    }

    /*
     signal_measurement_stats_t* stats = mgr_signal_stats_get();
     debug_info(
     "Signal measurement stats - count: %u, misses: %u, total time: %uus\n",
     stats->measurements_count, stats->misses_count,
     stats->total_time_spent); */
}
