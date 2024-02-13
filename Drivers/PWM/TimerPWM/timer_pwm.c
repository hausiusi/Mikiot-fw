/*
 * timer_pwm.c
 *
 *  Created on: Feb 6, 2024
 *      Author: Zviad
 */

#include "error.h"
#include "timer_pwm.h"
#include "hal_helpers.h"

typedef struct {
    TIM_HandleTypeDef* htim;
    timer_pwm_channel_gpio_config_t* config;
} timer_and_channel_t;

/* @formatter:off */
static timer_pwm_config_t _timer_pwm_configs[] = {
    {
        .htim.Instance = TIM4,
        .htim.Init.Prescaler = 10,
        .htim.Init.Period = 62499,
        .htim.Init.CounterMode = TIM_COUNTERMODE_UP,
        .htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE,
        .tmaster_config.MasterOutputTrigger = TIM_TRGO_RESET,
        .tmaster_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE,
        .channel_gpio_config[0] = {
            .gpiox = GPIOB,
            .gpio_init.Pin = GPIO_PIN_8,
            .gpio_init.Mode = GPIO_MODE_AF_PP,
            .gpio_init.Pull = GPIO_NOPULL,
            .gpio_init.Speed = GPIO_SPEED_FREQ_LOW,
            .gpio_init.Alternate = GPIO_AF2_TIM4,
            .channel_config.OCMode = TIM_OCMODE_PWM1,
            .channel_config.Pulse = 10000,
            .channel_config.OCPolarity = TIM_OCPOLARITY_HIGH,
            .channel_config.OCFastMode = TIM_OCFAST_DISABLE,
            .channel = TIM_CHANNEL_3
        },
        .channel_gpio_config[1] = {
            .gpiox = GPIOB,
            .gpio_init.Pin = GPIO_PIN_9,
            .gpio_init.Mode = GPIO_MODE_AF_PP,
            .gpio_init.Pull = GPIO_NOPULL,
            .gpio_init.Speed = GPIO_SPEED_FREQ_LOW,
            .gpio_init.Alternate = GPIO_AF2_TIM4,
            .channel_config.OCMode = TIM_OCMODE_PWM1,
            .channel_config.Pulse = 10000,
            .channel_config.OCPolarity = TIM_OCPOLARITY_HIGH,
            .channel_config.OCFastMode = TIM_OCFAST_DISABLE,
            .channel = TIM_CHANNEL_4
        },
        .channel_gpio_config[2] = {
            .gpiox = GPIOD,
            .gpio_init.Pin = GPIO_PIN_12,
            .gpio_init.Mode = GPIO_MODE_AF_PP,
            .gpio_init.Pull = GPIO_NOPULL,
            .gpio_init.Speed = GPIO_SPEED_FREQ_LOW,
            .gpio_init.Alternate = GPIO_AF2_TIM4,
            .channel_config.OCMode = TIM_OCMODE_PWM1,
            .channel_config.Pulse = 1000,
            .channel_config.OCPolarity = TIM_OCPOLARITY_HIGH,
            .channel_config.OCFastMode = TIM_OCFAST_DISABLE,
            .channel = TIM_CHANNEL_1
        },
        .channel_gpio_config[3] = {
            .gpiox = GPIOD,
            .gpio_init.Pin = GPIO_PIN_13,
            .gpio_init.Mode = GPIO_MODE_AF_PP,
            .gpio_init.Pull = GPIO_NOPULL,
            .gpio_init.Speed = GPIO_SPEED_FREQ_LOW,
            .gpio_init.Alternate = GPIO_AF2_TIM4,
            .channel_config.OCMode = TIM_OCMODE_PWM1,
            .channel_config.Pulse = 10000,
            .channel_config.OCPolarity = TIM_OCPOLARITY_HIGH,
            .channel_config.OCFastMode = TIM_OCFAST_DISABLE,
            .channel = TIM_CHANNEL_2
        },
    }
};

static timer_and_channel_t _timer_channels_map[] = {
    {
        .htim = &_timer_pwm_configs[0].htim,
        .config = &_timer_pwm_configs[0].channel_gpio_config[2]
    },
    {
        .htim = &_timer_pwm_configs[0].htim,
        .config = &_timer_pwm_configs[0].channel_gpio_config[1]
    },
    {
        .htim = &_timer_pwm_configs[0].htim,
        .config = &_timer_pwm_configs[0].channel_gpio_config[3]
    },
    {
        .htim = &_timer_pwm_configs[0].htim,
        .config = &_timer_pwm_configs[0].channel_gpio_config[4]
    }
};
/* @formatter:on */

void timer_pwm_init() {
    for (uint8_t i = 0; i < COUNT(_timer_pwm_configs); i++) {
        timer_pwm_config_t* tconf = &_timer_pwm_configs[i];
        if (tconf->htim.Instance == TIM3) {
            debug_info("Configuring PWM timer TIM3\n");
            __HAL_RCC_TIM3_CLK_ENABLE();
        } else if (tconf->htim.Instance == TIM4) {
            debug_info("Configuring PWM timer TIM4\n");
            __HAL_RCC_TIM4_CLK_ENABLE();
        }

        if (HAL_TIM_PWM_Init(&tconf->htim) != HAL_OK) {
            error_report(31, InitError);
            return;
        }

        if (HAL_TIMEx_MasterConfigSynchronization(&tconf->htim,
                &tconf->tmaster_config) != HAL_OK) {
            error_report(32, InitError);
            return;
        }

        for (uint8_t j = 0; j < COUNT(tconf->channel_gpio_config); j++) {
            debug_info("Configuring PWM channel %i\n", j);
            timer_pwm_channel_gpio_config_t* occonf =
                    &tconf->channel_gpio_config[j];
            if (HAL_TIM_PWM_ConfigChannel(&tconf->htim, &occonf->channel_config,
                    occonf->channel) != HAL_OK) {
                error_report(33, InitError);
                return;
            }

            hal_helper_gpio_clock_enable(occonf->gpiox);
            HAL_GPIO_Init(occonf->gpiox, &occonf->gpio_init);
            HAL_TIM_PWM_Start(&tconf->htim, occonf->channel);
        }
    }
}

void timer_pwm_set_value(pwm_channel_enum_t channel, uint32_t pulse) {
    if (channel >= COUNT(_timer_channels_map)) {
        return;
    }

    timer_and_channel_t timer_channel = _timer_channels_map[channel];

    __HAL_TIM_SET_COMPARE(timer_channel.htim, timer_channel.config->channel,
            pulse);
}

