/*
 * t_cli.c
 *
 *  Created on: Jan 23, 2020
 *      Author: Zviad
 */

#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include "cli.h"
#include "mw_uart.h"
#include "os_loader.h"
#include "FreeRTOS.h"
#include "task.h"

static uint8_t cli_rx_buffer[UART1_DMA_RX_BUFFER_SIZE];

static void _cli_data_received(uart_data_t* uart_data) {
    memcpy(cli_rx_buffer, uart_data->buffer, uart_data->allocated_length);
    xTaskNotifyFromISR(os_get_task_handler(CliHandler), 0,
            eSetValueWithoutOverwrite, NULL);
}

extern void thread_cli(void* args) {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart1ConfigIndex);
    conf->rec_data_process = _cli_data_received;
    mw_uart_dma_init(conf);
    debug_info("UART1 initialized BR: %i\n", conf->uart.Init.BaudRate);
    uint32_t notify_value = 0;

    for (;;) {
        xTaskNotifyWait( pdFALSE, /* Don't clear bits on entry. */
        ULONG_MAX, /* Clear all bits on exit. */
        &notify_value, /* Stores the notified value. */
        portMAX_DELAY);
        cli_cmd_process(cli_rx_buffer);
        memset(cli_rx_buffer, 0, UART1_DMA_RX_BUFFER_SIZE);
        vTaskDelay(0);
    }
}
