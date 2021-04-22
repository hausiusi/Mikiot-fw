/*
 * mgr_inputs.c
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#include "debug.h"
#include "defines.h"
#include "error.h"
#include "ioconfig.h"
#include "mgr_inputs.h"
#include "mw_io.h"

static void (*_input_detected_fp)(input_t*);

static void _tim_init(uint32_t period);
static input_t* _find_input(uint16_t gpio_pin);

/* @formatter:off */

// Please don't collide same pins on different ports, or upgrade the function _find_input
input_t _inputs[] = {
	{
		.gpio.gpiox = BTN1_GPIO,
		.gpio.pin = BTN1_GPIO_PIN,
		.gpio.mode = GPIO_MODE_IT_RISING_FALLING,
		.gpio.pull = GPIO_NOPULL,
		.type = InputIdleStateLow,
		.on_count = 0,
		.off_count = 0,
		.top_lvl = 25,
		.nvic.irq = EXTI0_IRQn,
		.nvic.preempt_priority = 15,
		.nvic.sub_priority = 15,
		.name = "BTN1_GPIOA"
	},
	{
		.gpio.gpiox = BTN2_GPIO,
		.gpio.pin = BTN2_GPIO_PIN,
		.gpio.mode = GPIO_MODE_IT_RISING_FALLING,
		.gpio.pull = GPIO_PULLUP,
		.type = InputIdleStateHigh,
		.on_count = 0,
		.off_count = 0,
		.top_lvl = 25,
		.nvic.irq = EXTI1_IRQn,
		.nvic.preempt_priority = 15,
		.nvic.sub_priority = 15,
		.name = "BTN2_GPIOA"
	},
	{
		.gpio.gpiox = BTN3_GPIO,
		.gpio.pin = BTN3_GPIO_PIN,
		.gpio.mode = GPIO_MODE_IT_RISING_FALLING,
		.gpio.pull = GPIO_PULLUP,
		.type = InputIdleStateHigh,
		.on_count = 0,
		.off_count = 0,
		.top_lvl = 25,
		.nvic.irq = EXTI2_IRQn,
		.nvic.preempt_priority = 15,
		.nvic.sub_priority = 15,
		.name = "BTN3_GPIOB"
	}
};

/* @formatter:on */

void mgr_inputs_init(uint32_t tim_period_us, void* fp) {
    for (int i = 0; i < COUNT(_inputs); i++) {
        gpio_init_t gpio = { 0 };

        /*Configure GPIO for pin */
        gpio.gpiox = _inputs[i].gpio.gpiox;
        gpio.init.Pin = _inputs[i].gpio.pin;
        gpio.init.Mode = _inputs[i].gpio.mode;
        gpio.init.Pull = _inputs[i].gpio.pull;
        mw_gpio_init(&gpio);
        mw_gpio_nvic_init(_inputs[i].nvic.irq, _inputs[i].nvic.preempt_priority,
                _inputs[i].nvic.sub_priority);
    }
    _tim_init(tim_period_us);
    _input_detected_fp = fp;
}

void mgr_input_trigger_handle() {
    bool_t state_confirmed = false;
    for (int i = 0; i < COUNT(_inputs); i++) {
        if (_inputs[i].confirmed_state == _inputs[i].state) {
            continue;
        }
        if (_inputs[i].state == InputStateHigh) {
            _inputs[i].off_count = 0;
            if (_inputs[i].on_count >= _inputs[i].top_lvl) {
                _inputs[i].confirmed_state = InputStateHigh;
                state_confirmed = true;
            } else {
                _inputs[i].on_count++;
            }
        } else if (_inputs[i].state == InputStateLow) {
            _inputs[i].on_count = 0;
            if (_inputs[i].off_count >= _inputs[i].top_lvl) {
                _inputs[i].confirmed_state = InputStateLow;
                state_confirmed = true;
            } else {
                _inputs[i].off_count++;
            }
        }
        if (state_confirmed && _input_detected_fp) {
            _input_detected_fp(&_inputs[i]);
            state_confirmed = false;
        }
    }
}

static input_t* _find_input(uint16_t gpio_pin) {
    for (int i = 0; i < COUNT(_inputs); i++) {
        input_t* input = &_inputs[i];
        if (gpio_pin == input->gpio.pin) {
            GPIO_PinState state = mw_gpio_readpin(input->gpio.gpiox,
                    input->gpio.pin);
            if (input->type == InputIdleStateLow) {
                if (input->state != (input_state_enum_t) state) {
                    return input;
                }
            } else if (input->type == InputIdleStateHigh) {
                if (input->state == (input_state_enum_t) state) {
                    return input;
                }
            }
        }
    }
    return NULL;
}

void mgr_input_detect_state(uint16_t gpio_pin) {
    input_t* input = _find_input(gpio_pin);
    if (gpio_pin == input->gpio.pin) {
        GPIO_PinState state = mw_gpio_readpin(input->gpio.gpiox,
                input->gpio.pin);
        if (input->type == InputIdleStateLow) {
            if (state == GPIO_PIN_SET) {
                input->state = InputStateHigh;
            } else {
                input->state = InputStateLow;
            }
        } else {
            if (state == GPIO_PIN_RESET) {
                input->state = InputStateHigh;
            } else {
                input->state = InputStateLow;
            }
        }
    }
}

static void _tim_init(uint32_t period) {
    htim11.Instance = TIM11;
    htim11.Init.Prescaler = 48;
    htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim11.Init.Period = period;
    htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim11) != HAL_OK) {
        error_report(3, InitError);
        return;
    }
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 15, 3);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    __HAL_RCC_TIM11_CLK_ENABLE();
    HAL_TIM_Base_Start_IT(&htim11);
}

static void __attribute__((unused)) _timebase_deinit() {
    if (htim11.Instance == TIM11) {
        __HAL_RCC_TIM11_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    }
}

void TIM1_TRG_COM_TIM11_IRQHandler(void) {
    mgr_input_trigger_handle();
    HAL_TIM_IRQHandler(&htim11);
}

