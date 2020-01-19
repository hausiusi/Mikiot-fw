/*
 * t_init.c
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#include "mw_redefines.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mw_uart1.h"
#include "mw_clock.h"
#include "debug.h"

uint32_t tmp, sysclk;

extern void thread_init() {
	for (int a = 0; a < 1000000; a++) {
		asm("NOP");
	}
	for (;;) {
		vTaskDelay(10);
	}
}

void uart_idle_detected(uint8_t* databuffer, uint32_t position, uint32_t buf_max_len) {
	debug_p("Idle detected\n");
}

extern void thread_test() {
	for (int a = 0; a < 1000000; a++) {
		asm("NOP");
	}
	int mhz = 30;



	for (;;) {
		tmp = HAL_RCC_GetPCLK2Freq();
		sysclk = HAL_RCC_GetSysClockFreq();
		mw_internal_clock_init();
		mw_external_clock_init(mhz--);
		mw_uart1_init(9600, uart_idle_detected);
		debug_p("123456789\n");
		vTaskDelay(500);
	}
}
