/*
 * t_adc.c
 *
 *  Created on: Jul 27, 2020
 *      Author: Zviad
 */
#include <limits.h>
#include "debug.h"
#include "mgr_adc.h"
#include "mw_adc.h"
#include "os_loader.h"

#include "FreeRTOS.h"
#include "task.h"

void* adc_data_buffer;

void adc_ht_detected(void* arg, uint32_t len) {
	adc_data_buffer = arg;
	xTaskNotifyFromISR(os_get_task_handler(AdcHandler), len,
			eSetValueWithoutOverwrite, NULL);
}

void adc_tc_detected(void* arg, uint32_t len) {
	adc_data_buffer = arg;
	xTaskNotifyFromISR(os_get_task_handler(AdcHandler), len,
			eSetValueWithoutOverwrite, NULL);
}

extern void thread_adc() {
	adc_uiconf_t* adc_uiconf = mgr_adc_get_uiconf();
	uint32_t notify_value = 0;
	uint32_t notified = pdFALSE;
	for (;;) {
		// Switch context until ADC is off
		while (!adc_uiconf->initialized) {
			vTaskDelay(0);
		}
		notified = xTaskNotifyWait(pdFALSE, /* Don't clear bits on entry. */
		ULONG_MAX, /* Clear all bits on exit. */
		&notify_value, /* Stores the notified value. */
		1);
		if (notified == pdTRUE) {
			uint16_t adc_value = *(uint16_t*) adc_data_buffer;
			if (adc_uiconf->print_value) {
				debug_p("ADC value: %i\n", adc_value);
			}
			if (adc_uiconf->print_voltage) {
				double voltage = (adc_value * adc_uiconf->ref_voltage) / 4096.0;

				debug_p("ADC voltage: %f\n", voltage);
			}
			if (!adc_uiconf->permanent && !--adc_uiconf->amount) {
				mgr_adc_deinit();
			} else {
				vTaskDelay(adc_uiconf->delay);
			}
		}
		vTaskDelay(10);
	}
}

