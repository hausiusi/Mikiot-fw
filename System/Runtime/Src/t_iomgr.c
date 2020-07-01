/*
 * t_iomgr.c
 *
 *  Created on: Jun 19, 2020
 *      Author: saturn
 */

#include "debug.h"
#include "mgr_button.h"
#include "mw_io.h"
#include "string.h"
#include "utils.h"
#include "stddef.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mw_redefines.h"
#include "mgr_tasks.h"

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

extern void thread_iomgr() {
	_gpio_init();
	mgr_button_init(1000);
	for (;;) {
		vTaskDelay(5);
	}
}

void mw_tim_period_elapsed_callback(TIM_HandleTypeDef* htim) {
	key_press_handle();
}

void mw_gpio_exti_callback(uint16_t GPIO_Pin) {
	mgr_button_detect_state(GPIO_Pin);
}

