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

extern void thread_init() {
	for (int a = 0; a < 1000000; a++) {
		asm("NOP");
	}
	for (;;) {
		vTaskDelay(10);
	}
}

extern void thread_test() {
	for (int a = 0; a < 1000000; a++) {
		asm("NOP");
	}
	for (;;) {
		vTaskDelay(0);
	}
}
