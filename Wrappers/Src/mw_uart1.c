/*
 * mw_uart3.c
 *
 *  Created on: Jan 11, 2020
 *      Author: Zviad
 */

#include <mw_uart1.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_gpio_ex.h"
#include "lib_busy_buffer.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

/* Local fields */
static uint8_t uart_rx_dma_buff[UART1_DMA_RX_BUFFER_SIZE];
static uint8_t uart_tx_dma_buff[UART1_DMA_TX_BUFFER_SIZE];

static DMA_HandleTypeDef hdma_usart1_rx, hdma_usart1_tx;
static UART_HandleTypeDef huart1;
static busy_buffer_t uart1_tx_busy_buff;
static double uart1_bit_time;

dma_buff_acquire_t uart_rec_data_process;

/* Local function prototypes */

static void _usart1_dma_configure();
static void _usart1_configure(uint32_t baud_rate);
static void _usart1_interrupt_idle_set(dma_buff_acquire_t fp);

/* Public functions */

void mw_uart1_dma_init(uint32_t baud_rate,
		dma_buff_acquire_t rx_idle_detected_fp) {
	uart1_bit_time = 1.0 / baud_rate;
	_usart1_configure(baud_rate);
	_usart1_interrupt_idle_set(rx_idle_detected_fp);
	_usart1_dma_configure(&huart1);
	lib_busy_buffer_init(&uart1_tx_busy_buff, uart_tx_dma_buff);
}

void mw_uart1_dma_string_transmit(char* text) {
	size_t len = strlen(text);
	size_t data_time_avg = (uart1_bit_time * 1000) * (len << 3) + 1; // Length in bytes << 3 and * 1000 to use milliseconds instead of seconds
	lib_busy_buffer_write(&uart1_tx_busy_buff, (uint8_t*) text, len);
	__HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);
	HAL_DMA_Init(&hdma_usart1_tx);
	lib_busy_buffer_engage(&uart1_tx_busy_buff, data_time_avg << 1,
			HAL_GetTick());
	HAL_UART_Transmit_DMA(&huart1, uart_tx_dma_buff, len);
	//HAL_UART_Transmit(&huart1, uart_tx_dma_buff, 17, 100);
}

void mw_uart1_dma_data_transmit(uint8_t* data, uint32_t length) {
	size_t data_time_avg = (uart1_bit_time * 1000) * (length << 3); // Length in bytes << 3 and * 1000 to use milliseconds instead of seconds
	lib_busy_buffer_write(&uart1_tx_busy_buff, data, length);
	__HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);
	HAL_DMA_Init(&hdma_usart1_tx);
	lib_busy_buffer_engage(&uart1_tx_busy_buff, data_time_avg << 1,
			HAL_GetTick());
	HAL_UART_Transmit_DMA(&huart1, uart_tx_dma_buff, length);
}

/* Local functions */

static void _usart1_configure(uint32_t baud_rate) {
	/* @formatter:off */
	__USART1_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	/* @formatter:on */
	GPIO_InitTypeDef igpio;
	/* Tx pin for UART1 */
	igpio.Pin = GPIO_PIN_6;
	igpio.Alternate = GPIO_AF7_USART1;
	igpio.Mode = GPIO_MODE_AF_PP;
	igpio.Speed = GPIO_SPEED_HIGH;
	igpio.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &igpio);

	/* Rx pin for UART1 */
	igpio.Pin = GPIO_PIN_7;
	igpio.Alternate = GPIO_AF7_USART1;
	igpio.Mode = GPIO_MODE_AF_PP;
	igpio.Speed = GPIO_SPEED_HIGH;
	igpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &igpio);

	/* UART initialization */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = baud_rate;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&huart1);
}

static void _usart1_interrupt_idle_set(dma_buff_acquire_t fp) {
	if (!fp)
		return;

	uart_rec_data_process = fp;
	/* Set USART interrupt when IDLE will be detected */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

static void _usart1_dma_configure() {
	__HAL_RCC_DMA2_CLK_ENABLE();
	//__HAL_DMA_ENABLE_IT(&hdma_usart3_rx, DMA_IT_DME);

	hdma_usart1_rx.Instance = DMA2_Stream2;
	hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;

	__HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);
	HAL_DMA_Init(&hdma_usart1_rx);
	HAL_UART_Receive_DMA(&huart1, uart_rx_dma_buff, UART1_DMA_RX_BUFFER_SIZE);
	/* Cortex-M uses the reversed priority numbering.
	 So, priority 0 corresponds to the highest priority */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	__HAL_DMA_ENABLE_IT(&hdma_usart1_tx, DMA_IT_TC);
	hdma_usart1_tx.Instance = DMA2_Stream7;
	hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_usart1_tx.Init.Mode = DMA_NORMAL;
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	__HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);
	HAL_DMA_Init(&hdma_usart1_tx);
}

/* Interrupt service routines */

void USART1_IRQHandler() {
	if (__HAL_UART_GET_FLAG(&huart1, USART_FLAG_IDLE) == SET) {
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		if (uart_rec_data_process) {
			/* Passes last recorded address of DMA buffer */
			uart_rec_data_process(uart_rx_dma_buff,
			UART1_DMA_RX_BUFFER_SIZE - hdma_usart1_rx.Instance->NDTR,
			UART1_DMA_RX_BUFFER_SIZE);
			HAL_UART_DMAStop(&huart1);
			HAL_UART_Receive_DMA(&huart1, uart_rx_dma_buff,
			UART1_DMA_RX_BUFFER_SIZE);
		}
	}
	HAL_UART_IRQHandler(&huart1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
	lib_busy_buffer_release(&uart1_tx_busy_buff);
	__HAL_DMA_DISABLE(&hdma_usart1_tx);
}

void DMA2_Stream2_IRQHandler() {
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void DMA2_Stream7_IRQHandler() {
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
}
