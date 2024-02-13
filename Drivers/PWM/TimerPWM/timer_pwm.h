/*
 * timer_pwm.h
 *
 *  Created on: Feb 6, 2024
 *      Author: Zviad
 */

#ifndef PWM_TIMERPWM_TIMER_PWM_H_
#define PWM_TIMERPWM_TIMER_PWM_H_

#include "stm32f4xx_hal.h"
#include "defines.h"
#include <inttypes.h>

typedef struct {
    GPIO_TypeDef* gpiox;
    GPIO_InitTypeDef gpio_init;
    TIM_OC_InitTypeDef channel_config;
    uint32_t channel;
} timer_pwm_channel_gpio_config_t;

typedef struct {
    TIM_HandleTypeDef htim;
    TIM_MasterConfigTypeDef tmaster_config;
    timer_pwm_channel_gpio_config_t channel_gpio_config[4];
} timer_pwm_config_t;

typedef enum {
    ChannelOne = 0,
    ChannelTwo = 1,
    ChannelThree = 2,
    ChannelFour = 3,
    ChannelFive = 4,
    ChannelSix = 5
} pwm_channel_enum_t;

void timer_pwm_init();
void timer_pwm_set_value(pwm_channel_enum_t channel, uint32_t pulse);

#endif /* PWM_TIMERPWM_TIMER_PWM_H_ */
