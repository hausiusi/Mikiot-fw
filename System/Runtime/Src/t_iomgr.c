/*
 * t_iomgr.c
 *
 *  Created on: Jun 19, 2020
 *      Author: saturn
 */

#include <stddef.h>
#include <string.h>
#include <limits.h>
#include "debug.h"
#include "mgr_inputs.h"
#include "mw_io.h"
#include "lib_ringbuffer.h"
#include "utils.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mw_redefines.h"
#include "mgr_tasks.h"
#include "os_loader.h"

static ringbuffer_t input_rb;
static input_t detected_inputs[5];

static void _gpio_init() {
    gpio_init_t gpio = { 0 };
    /*Configure GPIO pins : PD12 PD14 */
    gpio.gpiox = GPIOD;
    gpio.init.Pin = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_15;
    gpio.init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.init.Pull = GPIO_NOPULL;
    gpio.init.Speed = GPIO_SPEED_FREQ_LOW;
    mw_gpio_init(&gpio);
}

static void _input_detected(input_t* input) {
    lib_ringbuffer_enq(&input_rb, (uint8_t*) input, sizeof(input_t));
    xTaskNotifyFromISR(os_get_task_handler(IoMgrHandler), 0,
            eSetValueWithoutOverwrite, NULL);
}

static void _input_detection_overlapped() {
    debug_warn("Inputs are too fast, some recordings will be overlapped\n");
}

static void _toggle_leds(GPIO_TypeDef* gpio, uint16_t pin) {
    if (gpio == GPIOB && pin == GPIO_PIN_2) {
        mw_gpio_togglepin(GPIOD, GPIO_PIN_13);
    } else if (gpio == GPIOA && pin == GPIO_PIN_1) {
        mw_gpio_togglepin(GPIOD, GPIO_PIN_14);
    } else if (gpio == GPIOA && pin == GPIO_PIN_0) {
        mw_gpio_togglepin(GPIOD, GPIO_PIN_15);
    }
}

extern void thread_iomgr(void* args) {
    lib_ringbuffer_init(&input_rb, (uint8_t*) detected_inputs, sizeof(input_t),
            sizeof(detected_inputs), true, _input_detection_overlapped);
    _gpio_init();
    mgr_inputs_init(1000, _input_detected);
    input_t input = { 0 };
    uint32_t notify_value = 0;
    for (;;) {
        xTaskNotifyWait(pdFALSE, /* Don't clear bits on entry. */
        ULONG_MAX, /* Clear all bits on exit. */
        &notify_value, /* Stores the notified value. */
        portMAX_DELAY);
        lib_ringbuffer_deq(&input_rb, (uint8_t*) &input, sizeof(input));
        debug_p("Input state change detected:\n");
        debug_p(" Name        : %s\n", input.name);
        debug_p(" State       : %i\n", input.confirmed_state);
        debug_p(" Default     : %i\n", input.type);
        debug_p(" confirms in : %i ms\n", input.top_lvl);
        _toggle_leds(input.gpio.gpiox, input.gpio.pin);
        vTaskDelay(0);
    }
}
