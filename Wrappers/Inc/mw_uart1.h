/*
 * mw_uart.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#ifndef INC_MW_UART1_H_
#define INC_MW_UART1_H_

#include "inttypes.h"

/*
 * Initializes UART with passed baud rate
 * */
void mw_uart1_init(uint32_t baud_rate, void* rx_idle_detected_fp);

/*
 * Transfers text via UART3 (PD8) using DMA
 * */
void mw_uart1_dma_string_transmit(char* text);

/*
 * Transfers data via UART3 (PD8) using DMA
 * */

#endif /* INC_MW_UART1_H_ */
