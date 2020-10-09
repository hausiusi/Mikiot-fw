/*
 * mikiot_config.h
 *
 *  Created on: Jul 16, 2020
 *      Author: Zviad
 */

#ifndef INC_MIKIOT_CONFIG_H_
#define INC_MIKIOT_CONFIG_H_

#include "mock_error.h"
#include "error.h"
#include "stm32f4xx_hal_gpio.h"

/* When set to 1 project will be compiled with unit tests. */
#define COMPILE_WITH_UNIT_TESTS           0

/* UART buffer sizes */
#define MCONF_UART1_DMA_RX_BUFFER_SIZE    512
#define MCONF_UART1_DMA_TX_BUFFER_SIZE    512
#define MCONF_UART2_DMA_RX_BUFFER_SIZE    256
#define MCONF_UART2_DMA_TX_BUFFER_SIZE    256

/* GPRS buffers' sizes depend on corresponding UART buffer size */
#define MCONF_GPRS_RESP_BUFFER_SIZE       MCONF_UART2_DMA_RX_BUFFER_SIZE
#define MCONF_GPRS_CMD_BUFFER_SIZE        MCONF_UART2_DMA_TX_BUFFER_SIZE
#define MCONF_GPRS_EXPECT_BUFFER_SIZE     MCONF_UART2_DMA_RX_BUFFER_SIZE

/* GPRS modem IO pins */
#define MCONF_GPRS_RESET_PORT             GPIOB
#define MCONF_GPRS_RESET_PIN              GPIO_PIN_3

/* Size of debug buffer array depends on corresponding UART buffer size */
#define MCONF_DEBUG_BUFFER_SIZE           MCONF_UART1_DMA_TX_BUFFER_SIZE

char tf_debug_array[MCONF_DEBUG_BUFFER_SIZE];

#if (COMPILE_WITH_UNIT_TESTS == 1)
#define mikiot_error_report(id, type)     (mock_error_report(id, type))
#define debug_put_log(dbg_array)          (memcpy(tf_debug_array, dbg_array, MCONF_DEBUG_BUFFER_SIZE))
#else
#define mikiot_error_report(id, type)     error_report_(__FILE__, __LINE__, __FUNCTION__, id, #type)
#define debug_put_log(dbg_array)          (mw_uart_dma_string_transmit(Uart1ConfigIndex, dbg_array))
#endif

#endif /* INC_MIKIOT_CONFIG_H_ */
