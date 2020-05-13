/*
 * t_uart1_receiver.c
 *
 *  Created on: Jan 23, 2020
 *      Author: Zviad
 */

#include <inttypes.h>
#include <limits.h>
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mw_uart1.h"
#include "os_loader.h"
#include "cmdline.h"

#define UART1_BAUD_RATE			115200U

static uint8_t uart1_rx_buffer[UART1_DMA_RX_BUFFER_SIZE];

void uart1_data_received(uint8_t* databuffer, uint32_t position,
		uint32_t buf_max_len) {
	memcpy(uart1_rx_buffer, databuffer, position);
	xTaskNotifyFromISR(os_get_task_handler(Uart1RxHandler), 0,
			eSetValueWithoutOverwrite, NULL);
}

extern void thread_cmdline() {
	mw_uart1_dma_init(UART1_BAUD_RATE, uart1_data_received);
	debug_info("UART1 initialized BR: %i\n", UART1_BAUD_RATE);

	uint32_t notify_value = 0;
	for (;;) {
		static UBaseType_t watermark_current = 0;
		static UBaseType_t watermark_old = 255;
		watermark_current = uxTaskGetStackHighWaterMark( NULL);
		if (watermark_old > watermark_current) {
			watermark_old = watermark_current;
			debug_note("UART1 receiver task high watermark:%lu\n",
					watermark_current);
		}
		xTaskNotifyWait( pdFALSE, /* Don't clear bits on entry. */
		ULONG_MAX, /* Clear all bits on exit. */
		&notify_value, /* Stores the notified value. */
		portMAX_DELAY);
		cmd_process(uart1_rx_buffer);
		memset(uart1_rx_buffer, 0, UART1_DMA_RX_BUFFER_SIZE);
		debug_info("UART1 receiver task notified\n");
		vTaskDelay(0);
	}
}
