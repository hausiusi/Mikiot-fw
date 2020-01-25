/*
 * mw_uart.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#ifndef INC_MW_UART1_H_
#define INC_MW_UART1_H_

#include "inttypes.h"

/* Defines */

#define UART1_DMA_RX_BUFFER_SIZE 128
#define UART1_DMA_TX_BUFFER_SIZE 512

typedef void (*dma_buff_acquire_t)(uint8_t* databuffer, uint32_t position,
		uint32_t buf_max_len);

/*
 * Initializes UART with passed baud rate
 * */
void mw_uart1_dma_init(uint32_t baud_rate,
		dma_buff_acquire_t rx_idle_detected_fp);

/*
 * Transfers text via UART3 (PD8) using DMA
 * */
void mw_uart1_dma_string_transmit(char* text);

/*
 * Transfers data via UART3 (PD8) using DMA
 * */

#endif /* INC_MW_UART1_H_ */
