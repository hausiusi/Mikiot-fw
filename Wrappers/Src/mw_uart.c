/*
 * mw_uart.c
 *
 *  Created on: Aug 16, 2020
 *      Author: Zviad
 */

#include <string.h>
#include <error.h>
#include "mw_uart.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_gpio_ex.h"
#include "lib_busy_buffer.h"

/* Local fields */

static uint8_t uart1_rx_dma_buff[UART1_DMA_RX_BUFFER_SIZE];
static uint8_t uart2_rx_dma_buff[UART2_DMA_RX_BUFFER_SIZE];

static uint8_t uart1_tx_dma_buff[UART1_DMA_TX_BUFFER_SIZE];
static uint8_t uart2_tx_dma_buff[UART2_DMA_TX_BUFFER_SIZE];

static busy_buffer_t uart2_tx_busy_buff, uart1_tx_busy_buff;

/* Local function prototypes */

static void _uart_configure_dma();
static void _uart_configure(uart_dma_conf_t* conf);
static void _uart_interrupt_idle_set(uart_dma_conf_t* conf);

/* @formatter:off */
static uart_dma_conf_t uart_dma_configs[] = {
	{
		.uart.Instance = USART1,
		.uart.Init.BaudRate = 115200,
		.uart.Init.StopBits = UART_STOPBITS_1,
		.uart.Init.WordLength = UART_WORDLENGTH_8B,
		.uart.Init.Mode = UART_MODE_TX_RX,
		.uart.Init.Parity = UART_PARITY_NONE,
		.uart.Init.HwFlowCtl = UART_HWCONTROL_NONE,
		.gpiox_tx.gpiox = GPIOB,
		.gpiox_tx.init.Pin = GPIO_PIN_6,
		.gpiox_tx.init.Alternate = GPIO_AF7_USART1,
		.gpiox_tx.init.Mode = GPIO_MODE_AF_PP,
		.gpiox_tx.init.Speed = GPIO_SPEED_HIGH,
		.gpiox_tx.init.Pull = GPIO_PULLUP,
		.gpiox_rx.gpiox = GPIOB,
		.gpiox_rx.init.Pin = GPIO_PIN_7,
		.gpiox_rx.init.Alternate = GPIO_AF7_USART1,
		.gpiox_rx.init.Mode = GPIO_MODE_AF_PP,
		.gpiox_rx.init.Speed = GPIO_SPEED_HIGH,
		.gpiox_rx.init.Pull = GPIO_NOPULL,
		.nvic_uart.irq = USART1_IRQn,
		.nvic_uart.preempt_priority = 15,
		.nvic_uart.sub_priority = 0,
		.dma_rx.Instance = DMA2_Stream2,
		.dma_rx.Init.Channel = DMA_CHANNEL_4,
		.dma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY,
		.dma_rx.Init.PeriphInc = DMA_PINC_DISABLE,
		.dma_rx.Init.MemInc = DMA_MINC_ENABLE,
		.dma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.dma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.dma_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE,
		.dma_rx.Init.Mode = DMA_CIRCULAR,
		.nvic_dma_rx.irq = DMA2_Stream2_IRQn,
		.nvic_dma_rx.preempt_priority = 15,
		.nvic_dma_rx.sub_priority = 0,
		.dma_tx.Instance = DMA2_Stream7,
		.dma_tx.Init.Channel = DMA_CHANNEL_4,
		.dma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH,
		.dma_tx.Init.PeriphInc = DMA_PINC_DISABLE,
		.dma_tx.Init.MemInc = DMA_MINC_ENABLE,
		.dma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.dma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.dma_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE,
		.dma_tx.Init.Mode = DMA_NORMAL,
		.nvic_dma_tx.irq = DMA2_Stream7_IRQn,
		.nvic_dma_tx.preempt_priority = 15,
		.nvic_dma_tx.sub_priority = 0,
		.busy_tx = &uart1_tx_busy_buff,
		.rx_data.buffer = uart1_rx_dma_buff,
		.tx_data.buffer = uart1_tx_dma_buff,
		.rx_data.max_length = UART1_DMA_RX_BUFFER_SIZE,
		.rx_data.max_length = UART1_DMA_TX_BUFFER_SIZE,
	},
	{
		.uart.Instance = USART2,
		.uart.Init.BaudRate = 57600,
		.uart.Init.StopBits = UART_STOPBITS_1,
		.uart.Init.WordLength = UART_WORDLENGTH_8B,
		.uart.Init.Mode = UART_MODE_TX_RX,
		.uart.Init.Parity = UART_PARITY_NONE,
		.uart.Init.HwFlowCtl = UART_HWCONTROL_NONE,
		.gpiox_tx.gpiox = GPIOD,
		.gpiox_tx.init.Pin = GPIO_PIN_5,
		.gpiox_tx.init.Alternate = GPIO_AF7_USART2,
		.gpiox_tx.init.Mode = GPIO_MODE_AF_PP,
		.gpiox_tx.init.Speed = GPIO_SPEED_HIGH,
		.gpiox_tx.init.Pull = GPIO_PULLUP,
		.gpiox_rx.gpiox = GPIOD,
		.gpiox_rx.init.Pin = GPIO_PIN_6,
		.gpiox_rx.init.Alternate = GPIO_AF7_USART2,
		.gpiox_rx.init.Mode = GPIO_MODE_AF_PP,
		.gpiox_rx.init.Speed = GPIO_SPEED_HIGH,
		.gpiox_rx.init.Pull = GPIO_NOPULL,
		.nvic_uart.irq = USART2_IRQn,
		.nvic_uart.preempt_priority = 15,
		.nvic_uart.sub_priority = 0,
		.dma_rx.Instance = DMA1_Stream5,
		.dma_rx.Init.Channel = DMA_CHANNEL_4,
		.dma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY,
		.dma_rx.Init.PeriphInc = DMA_PINC_DISABLE,
		.dma_rx.Init.MemInc = DMA_MINC_ENABLE,
		.dma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.dma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.dma_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE,
		.dma_rx.Init.Mode = DMA_CIRCULAR,
		.nvic_dma_rx.irq = DMA1_Stream5_IRQn,
		.nvic_dma_rx.preempt_priority = 15,
		.nvic_dma_rx.sub_priority = 0,
		.dma_tx.Instance = DMA1_Stream6,
		.dma_tx.Init.Channel = DMA_CHANNEL_4,
		.dma_tx.Init.Direction = DMA_MEMORY_TO_PERIPH,
		.dma_tx.Init.PeriphInc = DMA_PINC_DISABLE,
		.dma_tx.Init.MemInc = DMA_MINC_ENABLE,
		.dma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.dma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.dma_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE,
		.dma_tx.Init.Mode = DMA_NORMAL,
		.nvic_dma_tx.irq = DMA1_Stream6_IRQn,
		.nvic_dma_tx.preempt_priority = 15,
		.nvic_dma_tx.sub_priority = 0,
		.busy_tx = &uart2_tx_busy_buff,
		.rx_data.buffer = uart2_rx_dma_buff,
		.tx_data.buffer = uart2_tx_dma_buff,
		.rx_data.max_length = UART2_DMA_RX_BUFFER_SIZE,
		.tx_data.max_length = UART2_DMA_TX_BUFFER_SIZE,
	},
};
/* @formatter:on */

/* Global functions */

void mw_uart_dma_init(uart_dma_conf_t* conf) {
    conf->bit_time = 1.0 / conf->uart.Init.BaudRate;
    _uart_configure(conf);
    _uart_interrupt_idle_set(conf);
    _uart_configure_dma(conf);
    lib_busy_buffer_init(conf->busy_tx, conf->tx_data.buffer);
}

void mw_uart_dma_string_transmit(uart_index_enum_t uart_index, char* text) {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(uart_index);
    size_t len = strlen(text);
    size_t data_time_avg = (conf->bit_time * 1000) * (len << 3) + 1; // Length in bytes << 3 and * 1000 to use milliseconds instead of seconds
    lib_busy_buffer_write(conf->busy_tx, (uint8_t*) text, len);
    __HAL_LINKDMA(&conf->uart, hdmatx, conf->dma_tx);
    HAL_DMA_Init(&conf->dma_tx);
    lib_busy_buffer_engage(conf->busy_tx, data_time_avg << 1, HAL_GetTick());
    HAL_UART_Transmit_DMA(&conf->uart, conf->tx_data.buffer, len);
}

void mw_uart_dma_data_transmit(uart_index_enum_t uart_index, uint8_t* data,
        uint32_t length) {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(uart_index);
    size_t data_time_avg = (conf->bit_time * 1000) * (length << 3); // Length in bytes << 3 and * 1000 to use milliseconds instead of seconds
    lib_busy_buffer_write(conf->busy_tx, data, length);
    __HAL_LINKDMA(&conf->uart, hdmatx, conf->dma_tx);
    HAL_DMA_Init(&conf->dma_tx);
    lib_busy_buffer_engage(conf->busy_tx, data_time_avg << 1, HAL_GetTick());
    HAL_UART_Transmit_DMA(&conf->uart, conf->tx_data.buffer, length);
}

/* Local functions */

static void _uart_configure(uart_dma_conf_t* conf) {
    if (conf->uart.Instance == USART1) {
        /* @formatter:off */
		__USART1_CLK_ENABLE();/* @formatter:on */
    } else if (conf->uart.Instance == USART2) {
        /* @formatter:off */
		__USART2_CLK_ENABLE();/* @formatter:on */
    } else if (conf->uart.Instance == USART6) {
        /* @formatter:off */
		__USART6_CLK_ENABLE();/* @formatter:on */
    } else {
        error_report(18, UartError);
        return;
    }
    SET_BIT(RCC->AHB1ENR,
            (0x1UL << (((uint32_t)conf->gpiox_rx.gpiox - (uint32_t)GPIOA)/0x0400UL)));
    /* Delay after an RCC peripheral clock enabling */
    __IO uint32_t tmpreg = 0;
    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
    UNUSED(tmpreg);
    /* Tx pin for UART2 */
    HAL_GPIO_Init(conf->gpiox_tx.gpiox, &conf->gpiox_tx.init);

    /* Rx pin for UART2 */
    HAL_GPIO_Init(conf->gpiox_rx.gpiox, &conf->gpiox_rx.init);

    /* UART initialization */
    HAL_UART_Init(&conf->uart);
}

static void _uart_interrupt_idle_set(uart_dma_conf_t* conf) {
    if (!conf->rec_data_process)
        return;
    /* Set USART interrupt when IDLE will be detected */
    __HAL_UART_ENABLE_IT(&conf->uart, UART_IT_IDLE);
    HAL_NVIC_SetPriority(conf->nvic_uart.irq, conf->nvic_uart.preempt_priority,
            conf->nvic_uart.sub_priority);
    HAL_NVIC_EnableIRQ(conf->nvic_uart.irq);
}

static void _uart_configure_dma(uart_dma_conf_t* conf) {
    // USART1 and USART6 uses DMA2, USART2 - DMA1, other cases are invalid
    if (conf->uart.Instance == USART1 || conf->uart.Instance == USART6) {
        __HAL_RCC_DMA2_CLK_ENABLE();
    } else if (conf->uart.Instance == USART2) {
        __HAL_RCC_DMA1_CLK_ENABLE();
    } else {
        error_report(19, UartError);
        return;
    }
    __HAL_LINKDMA(&conf->uart, hdmarx, conf->dma_rx);
    HAL_DMA_Init(&conf->dma_rx);
    HAL_UART_Receive_DMA(&conf->uart, conf->rx_data.buffer,
            conf->rx_data.max_length);
    /* Cortex-M uses the reversed priority numbering.
     So, priority 0 corresponds to the highest priority */
    HAL_NVIC_SetPriority(conf->nvic_dma_rx.irq,
            conf->nvic_dma_rx.preempt_priority, conf->nvic_dma_rx.sub_priority);
    HAL_NVIC_EnableIRQ(conf->nvic_dma_rx.irq);

    __HAL_DMA_ENABLE_IT(&conf->dma_tx, DMA_IT_TC);
    HAL_NVIC_SetPriority(conf->nvic_dma_tx.irq,
            conf->nvic_dma_tx.preempt_priority, conf->nvic_dma_tx.sub_priority);
    HAL_NVIC_EnableIRQ(conf->nvic_dma_tx.irq);

    __HAL_LINKDMA(&conf->uart, hdmatx, conf->dma_tx);
    HAL_DMA_Init(&conf->dma_tx);
}

static uart_dma_conf_t* _uart_get_config(UART_HandleTypeDef* huart) {
    if (huart->Instance == USART1) {
        return mw_uart_dma_get_config(Uart1ConfigIndex);
    } else if (huart->Instance == USART2) {
        return mw_uart_dma_get_config(Uart2ConfigIndex);
    } else if (huart->Instance == USART6) {
        return mw_uart_dma_get_config(Uart6ConfigIndex);
    } else {
        error_report(20, UartError);
        return NULL;
    }
}

/* Interrupt service routines */

void USART1_IRQHandler() {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart1ConfigIndex);
    if (__HAL_UART_GET_FLAG(&conf->uart, USART_FLAG_IDLE) == SET) {
        __HAL_UART_CLEAR_IDLEFLAG(&conf->uart);
        if (conf->rec_data_process) {
            conf->rx_data.allocated_length = conf->rx_data.max_length
                    - conf->dma_rx.Instance->NDTR;
            conf->rec_data_process(&conf->rx_data);
            HAL_UART_DMAStop(&conf->uart);
            HAL_UART_Receive_DMA(&conf->uart, conf->rx_data.buffer,
                    conf->rx_data.max_length);
        }
    }
    HAL_UART_IRQHandler(&conf->uart);
}

void USART2_IRQHandler() {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart2ConfigIndex);
    if (__HAL_UART_GET_FLAG(&conf->uart, USART_FLAG_IDLE) == SET) {
        __HAL_UART_CLEAR_IDLEFLAG(&conf->uart);
        if (conf->rec_data_process) {
            conf->rx_data.allocated_length = conf->rx_data.max_length
                    - conf->dma_rx.Instance->NDTR;
            conf->rec_data_process(&conf->rx_data);
            HAL_UART_DMAStop(&conf->uart);
            HAL_UART_Receive_DMA(&conf->uart, conf->rx_data.buffer,
                    conf->rx_data.max_length);
        }
    }
    HAL_UART_IRQHandler(&conf->uart);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    uart_dma_conf_t* conf = _uart_get_config(huart);
    lib_busy_buffer_release(conf->busy_tx);
    __HAL_DMA_DISABLE(&conf->dma_tx);
}

// UART1 DMA handlers
void DMA2_Stream2_IRQHandler() {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart1ConfigIndex);
    HAL_DMA_IRQHandler(&conf->dma_rx);
}

void DMA2_Stream7_IRQHandler() {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart1ConfigIndex);
    HAL_DMA_IRQHandler(&conf->dma_tx);
}

// UART2 DMA handlers
void DMA1_Stream5_IRQHandler() {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart2ConfigIndex);
    HAL_DMA_IRQHandler(&conf->dma_rx);
}

void DMA1_Stream6_IRQHandler() {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart2ConfigIndex);
    HAL_DMA_IRQHandler(&conf->dma_tx);
}

uart_dma_conf_t* mw_uart_dma_get_config(uart_index_enum_t index) {
    return &uart_dma_configs[index];
}
