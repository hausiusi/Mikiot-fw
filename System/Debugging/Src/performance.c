/*
 * performance.c
 *
 *  Created on: Feb 17, 2019
 *      Author: Zviad
 */

#include "performance.h"

static TIM_HandleTypeDef htim5, htim4;

static void _timer_measurer_init();
static void _timer5_32_bit_init(uint32_t freq);
static void _timer4_16_bit_init(uint32_t freq);
static inline void _timer5_cnt_reset();
static inline void _timer4_cnt_reset();

void prf_timer_osmeasurer_init() {
	_timer4_16_bit_init(PERF_OSTASKMEASURER_FREQ);
	_timer4_cnt_reset();
}

uint32_t prf_timer_osmeasurer_count_get() {
	return htim4.Instance->CNT;
}

inline void prf_start_measurement() {
	_timer_measurer_init();
	_timer5_cnt_reset(PERF_COUNTER_FREQ);
}

inline uint32_t prf_end_measurement() {
	uint32_t cnt = htim5.Instance->CNT;
	HAL_TIM_Base_Stop(&htim5);
	__HAL_RCC_TIM5_CLK_DISABLE();
	return cnt;
}

static void _timer_measurer_init() {
	_timer5_32_bit_init(PERF_COUNTER_FREQ);
}

static void _timer5_32_bit_init(uint32_t freq) {
	uint32_t prescaler_value = (uint32_t) ((SystemCoreClock) / freq) - 1;
	htim5.Instance = PERFORMANCE_COUNTER_TIMER;
	htim5.Init.Prescaler = prescaler_value;
	htim5.Init.Period = 0xFFFFFFFFUL;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	__HAL_RCC_TIM5_CLK_ENABLE();
	HAL_TIM_Base_Init(&htim5);
	HAL_TIM_Base_Start(&htim5);
}

static void _timer4_16_bit_init(uint32_t freq) {
	uint32_t prescaler_value = (uint32_t) ((SystemCoreClock / 4) / freq) - 1;
	htim4.Instance = OSTASKMEASURER_TIMER;
	htim4.Init.Prescaler = prescaler_value;
	htim4.Init.Period = freq - 1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	__HAL_RCC_TIM4_CLK_ENABLE();
	HAL_TIM_Base_Init(&htim4);
	HAL_TIM_Base_Start(&htim4);
}

static inline void _timer5_cnt_reset() {
	htim5.Instance->CNT = 0;
}

static inline void _timer4_cnt_reset() {
	htim4.Instance->CNT = 0;
}
