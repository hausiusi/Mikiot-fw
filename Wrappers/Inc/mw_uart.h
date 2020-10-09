/*
 * mw_uart.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#ifndef INC_MW_UART_H_
#define INC_MW_UART_H_

#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_gpio_ex.h"
#include "lib_busy_buffer.h"
#include "mikiot_config.h"
#include "mw_redefines.h"

#define UART1_DMA_RX_BUFFER_SIZE           MCONF_UART1_DMA_RX_BUFFER_SIZE
#define UART1_DMA_TX_BUFFER_SIZE           MCONF_UART1_DMA_TX_BUFFER_SIZE
#define UART2_DMA_RX_BUFFER_SIZE           MCONF_UART2_DMA_RX_BUFFER_SIZE
#define UART2_DMA_TX_BUFFER_SIZE           MCONF_UART2_DMA_TX_BUFFER_SIZE

typedef void (*dma_buff_acquire_t)(uint8_t* databuffer, uint32_t position,
        uint32_t buf_max_len);

/* @formatter:off */
typedef enum {
	Uart1ConfigIndex,
	Uart2ConfigIndex,
	Uart6ConfigIndex
} uart_index_enum_t;
/* @formatter:on */

typedef struct {
    DMA_HandleTypeDef dma_rx;
    DMA_HandleTypeDef dma_tx;
    UART_HandleTypeDef uart;
    gpio_init_t gpiox_rx;
    gpio_init_t gpiox_tx;
    nvic_param_t nvic_uart;
    nvic_param_t nvic_dma_rx;
    nvic_param_t nvic_dma_tx;
    busy_buffer_t* busy_tx;
    double bit_time;
    uint8_t* rx_buffer;
    uint8_t* tx_buffer;
    uint32_t rx_buffer_size;
    uint32_t tx_buffer_size;
    dma_buff_acquire_t rec_data_process;
} uart_dma_conf_t;

/*
 * Initializes UART with passed config
 *
 * @params conf: can be requested default configuration by using mw_uart_dma_get_config(uart_index_enum_t i)
 * function or can be created from scratch. Parameter conf can be modified before pass
 *
 * @returns: void
 * */
void mw_uart_dma_init(uart_dma_conf_t* conf);

/*
 * Transfers text via UART using DMA
 *
 * @params uart_index: index of UART peripheral device declared in uart_index_enum_t
 *
 * @params text: pointer to text that is supposed to transfer
 *
 * @returns: void
 * */
void mw_uart_dma_string_transmit(uart_index_enum_t uart_index, char* text);

/*
 * Transfers data via UART using DMA
 *
 * @params uart_index: index of UART peripheral device declared in uart_index_enum_t
 *
 * @params data: pointer to bytes array that is supposed to transfer
 *
 * @params length: byte array length in bytes
 *
 * @returns: void
 *
 * */
void mw_uart_dma_data_transmit(uart_index_enum_t uart_index, uint8_t* data,
        uint32_t length);

/*
 * Gets default DMA configuration for particular UARTs
 *
 * @params uart_index: index of UART peripheral device declared in uart_index_enum_t
 *
 * @returns: default DMA configuration for requested UART
 * */
uart_dma_conf_t* mw_uart_dma_get_config(uart_index_enum_t index);

#endif /* INC_MW_UART_H_ */
