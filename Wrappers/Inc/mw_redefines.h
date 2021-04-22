/*
 * w_redefines.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#ifndef MW_REDEFINES_H_
#define MW_REDEFINES_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"

#define mw_init                        HAL_Init
#define mw_gpio_readpin                HAL_GPIO_ReadPin
#define mw_gpio_writepin               HAL_GPIO_WritePin
#define mw_gpio_setpin(gpio, pin)      HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET)
#define mw_gpio_resetpin(gpio, pin)    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET)
#define mw_gpio_togglepin              HAL_GPIO_TogglePin
#define mw_gpio_exti_callback          HAL_GPIO_EXTI_Callback
#define mw_tim_period_elapsed_callback HAL_TIM_PeriodElapsedCallback

/* RTC typedef section*/
typedef RTC_HandleTypeDef rtc_handle_t;
typedef RTC_TimeTypeDef rtc_time_t;
typedef RTC_DateTypeDef rtc_date_t;
typedef TIM_HandleTypeDef tim_handle_t;

/* Mikiot wrapper types */

typedef struct {
    GPIO_TypeDef* gpiox;
    uint16_t pin;
    uint32_t mode;
    uint32_t pull;
    uint32_t speed;
    uint32_t alternate;
} gpio_t;

typedef struct {
    uint16_t irq;
    uint32_t preempt_priority;
    uint32_t sub_priority;
} nvic_param_t;

typedef struct {
    GPIO_TypeDef* gpiox; /* Specifies the GPIO port to be configured.
     This parameter can be any available GPIO_port */
    GPIO_InitTypeDef init; /* GPIO configuration structure */
} gpio_init_t;

#endif /* MW_REDEFINES_H_ */
