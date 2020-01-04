/*
 * mw_uart.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#ifndef INC_MW_UART_H_
#define INC_MW_UART_H_


void mw_uart_write_bytes(uint8_t* data, size_t len);

void mw_uart_dma_write_bytes(uint8_t* data, size_t len);

void mw_uart_write_text(char* txt);

void mw_uart_dma_write_text(char* txt);

#endif /* INC_MW_UART_H_ */
