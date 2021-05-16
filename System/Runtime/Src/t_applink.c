/*
 * t_applink.c
 *
 *  Created on: May 16, 2021
 *      Author: Zviad
 */

#include <limits.h>
#include <string.h>
#include "mw_uart.h"
#include "os_loader.h"
#include "FreeRTOS.h"
#include "task.h"
#include "applink.h"

static uint8_t applink_rx_buffer[UART6_DMA_RX_BUFFER_SIZE];

static void _applink_data_received(uart_data_t* uart_data) {
    memcpy(applink_rx_buffer, uart_data->buffer, uart_data->allocated_length);
    xTaskNotifyFromISR(os_get_task_handler(AppLinkHandler), 0,
            eSetValueWithoutOverwrite, NULL);
}

extern void thread_applink(void* args) {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart6ConfigIndex);
    conf->rec_data_process = _applink_data_received;
    mw_uart_dma_init(conf);
    vTaskDelay(100);
    debug_info("UART6 initialized BR: %i\n", conf->uart.Init.BaudRate);
    uint32_t notify_value = 0;
    for (;;) {
        xTaskNotifyWait(pdFALSE, /* Don't clear bits on entry. */
        ULONG_MAX, /* Clear all bits on exit. */
        &notify_value, /* Stores the notified value. */
        portMAX_DELAY);
        applink_packet_process(applink_rx_buffer);
        memset(applink_rx_buffer, 0, UART6_DMA_RX_BUFFER_SIZE);
        vTaskDelay(0);
    }
}
