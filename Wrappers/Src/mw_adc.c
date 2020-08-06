/*
 * mw_adc.c
 *
 *  Created on: Jul 25, 2020
 *      Author: Zviad
 */
#include "error.h"
#include "ioconfig.h"
#include "mw_adc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_adc.h"

/*
 * Documentation:
 * STM32F411 general datasheet
 * Chapter: 3.29
 * Page: 32
 */

#define ADC1_IN1_CH            ADC_CHANNEL_8
#define ADC1_CH_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADC1_CLK_ENABLE()      __HAL_RCC_ADC1_CLK_ENABLE()
#define ADC1_DMA_CLK_ENABLE()  __HAL_RCC_DMA2_CLK_ENABLE()
#define ADC1_FORCE_RESET()     __HAL_RCC_ADC_FORCE_RESET()
#define ADC1_RELEASE_RESET()   __HAL_RCC_ADC_RELEASE_RESET()
#define ADC1_DMA_STREAM        DMA2_Stream4
#define ADC1_DMA_IRQn          DMA2_Stream4_IRQn
#define ADC1_DMA_IRQHandler    DMA2_Stream4_IRQHandler
#define ADC1_DMA_CHANNEL       DMA_CHANNEL_0
#define ADC1_CHANNEL           ADC_CHANNEL_8

#define ADC_DATA_BUFF_LEN       1000

ADC_HandleTypeDef hadc1;
GPIO_InitTypeDef gpiob;
DMA_HandleTypeDef hdma_adc1;
uint16_t adc1_data_buff[ADC_DATA_BUFF_LEN];
uint32_t ht_counter, ft_counter;
void (*adc_ht_detected_fp)(void*, uint32_t);
void (*adc_tc_detected_fp)(void*, uint32_t);

/* Function prototypes */
void _adc1_gpio_init();
void _adc1_dma_init();

void mw_adc1_init(void* adc_ht_fp, void* adc_tc_fp) {
	adc_ht_detected_fp = adc_ht_fp;
	adc_tc_detected_fp = adc_tc_fp;

	ADC_ChannelConfTypeDef sConfig;
	ADC1_CLK_ENABLE();
	__HAL_DMA_ENABLE_IT(&hdma_adc1, DMA_IT_TC);
	HAL_NVIC_SetPriority(ADC1_DMA_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(ADC1_DMA_IRQn);
	_adc1_gpio_init();
	_adc1_dma_init();

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.NbrOfDiscConversion = 0;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		/* Initialization Error */
		error_report(15, AdcError);
	}
	sConfig.Channel = ADC1_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	sConfig.Offset = 0;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		/* Channel Configuration Error */
		error_report(16, AdcError);
	}
	__HAL_DMA_ENABLE_IT(&hdma_adc1, DMA_IT_TC);
	__HAL_DMA_ENABLE_IT(&hdma_adc1, DMA_IT_HT);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc1_data_buff, ADC_DATA_BUFF_LEN);
}

void mw_adc_deinit() {
	HAL_ADC_Stop_DMA(&hadc1);
}

void _adc1_gpio_init() {
	ADC1_CH_CLK_ENABLE();
	gpiob.Pin = ADC1_IN1_GPIO_PIN;
	gpiob.Mode = GPIO_MODE_ANALOG;
	gpiob.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ADC1_IN1_GPIO, &gpiob);
}

void _adc1_dma_init() {
	ADC1_DMA_CLK_ENABLE();
	hdma_adc1.Instance = ADC1_DMA_STREAM;
	hdma_adc1.Init.Channel = ADC1_DMA_CHANNEL;
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc1.Init.Mode = DMA_CIRCULAR;
	hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;
	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);
	HAL_DMA_Init(&hdma_adc1);
}

void ADC1_DMA_IRQHandler() {
	HAL_DMA_IRQHandler(&hdma_adc1);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc) {
	if (adc_ht_detected_fp) {
		adc_ht_detected_fp((void*) hadc->DMA_Handle->Instance->M0AR,
		ADC_DATA_BUFF_LEN >> 1);
	}
	ht_counter++;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (adc_tc_detected_fp) {
		adc_tc_detected_fp(
				(void*) hadc->DMA_Handle->Instance->M0AR
						+ (ADC_DATA_BUFF_LEN >> 1),
				ADC_DATA_BUFF_LEN >> 1);
	}
	ft_counter++;
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc) {
	HAL_ADC_Stop_DMA(hadc);
	HAL_ADC_Start_DMA(hadc, (uint32_t*) adc1_data_buff, ADC_DATA_BUFF_LEN);
	error_report(17, AdcError);
}
