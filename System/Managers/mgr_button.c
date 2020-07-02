/*
 * mgr_button.c
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#include "debug.h"
#include "defines.h"
#include "error.h"
#include "ioconfig.h"
#include "mgr_button.h"
#include "mw_io.h"

static void _timebase_init();
static void _tim_init(uint32_t period);
static btn_t* _find_button(uint16_t gpio_pin);

/* @formatter:off */

// Please don't collide same pins on different ports, or upgrade the function find_button
btn_t _buttons[] = {
	{
		.gpio.gpiox = BTN1_GPIO,
		.gpio.pin = BTN1_GPIO_PIN,
		.gpio.mode = GPIO_MODE_IT_RISING_FALLING,
		.gpio.pull = GPIO_NOPULL,
		.type = ButtonNormalOpen,
		.on_count = 0,
		.off_count = 0,
		.top_lvl = 25,
		.nvic.irq = EXTI0_IRQn,
		.nvic.preempt_priority = 0,
		.nvic.sub_priority = 0
	},
	{
		.gpio.gpiox = BTN2_GPIO,
		.gpio.pin = BTN2_GPIO_PIN,
		.gpio.mode = GPIO_MODE_IT_RISING_FALLING,
		.gpio.pull = GPIO_PULLUP,
		.type = ButtonNormalClose,
		.on_count = 0,
		.off_count = 0,
		.top_lvl = 25,
		.nvic.irq = EXTI1_IRQn,
		.nvic.preempt_priority = 0,
		.nvic.sub_priority = 0
	},
	{
		.gpio.gpiox = BTN3_GPIO,
		.gpio.pin = BTN3_GPIO_PIN,
		.gpio.mode = GPIO_MODE_IT_RISING_FALLING,
		.gpio.pull = GPIO_PULLUP,
		.type = ButtonNormalClose,
		.on_count = 0,
		.off_count = 0,
		.top_lvl = 25,
		.nvic.irq = EXTI2_IRQn,
		.nvic.preempt_priority = 0,
		.nvic.sub_priority = 0
	}
};

/* @formatter:on */

void mgr_button_init(uint32_t tim_period_us) {
	for (int i = 0; i < COUNT(_buttons); i++) {
		gpio_init_t gpio = { 0 };

		/*Configure GPIO for pin */
		gpio.gpiox = _buttons[i].gpio.gpiox;
		gpio.init.Pin = _buttons[i].gpio.pin;
		gpio.init.Mode = _buttons[i].gpio.mode;
		gpio.init.Pull = _buttons[i].gpio.pull;
		mw_gpio_init(&gpio);
		mw_gpio_nvic_init(_buttons[i].nvic.irq,
				_buttons[i].nvic.preempt_priority,
				_buttons[i].nvic.sub_priority);
	}
	_tim_init(tim_period_us);
}

static void _button_pressed(GPIO_TypeDef* gpio, uint16_t pin) {
	debug_p("Button %i click\n", pin);
	if (gpio == GPIOB && pin == GPIO_PIN_2) {
		mw_gpio_togglepin(GPIOD, GPIO_PIN_13);
	} else if (gpio == GPIOA && pin == GPIO_PIN_1) {
		mw_gpio_togglepin(GPIOD, GPIO_PIN_14);
	} else if (gpio == GPIOA && pin == GPIO_PIN_0) {
		mw_gpio_togglepin(GPIOD, GPIO_PIN_15);
	}
}

void key_press_handle() {
// Read button state
	for (int i = 0; i < COUNT(_buttons); i++) {
		if (_buttons[i].confirmed_state == _buttons[i].state) {
			continue;
		}
		if (_buttons[i].state == ButtonOn) {
			_buttons[i].off_count = 0;
			if (_buttons[i].on_count >= _buttons[i].top_lvl) {
				_buttons[i].confirmed_state = ButtonOn;
			} else {
				_buttons[i].on_count++;
			}
		} else if (_buttons[i].state == ButtonOff) {
			_buttons[i].on_count = 0;
			if (_buttons[i].off_count >= _buttons[i].top_lvl) {
				_buttons[i].confirmed_state = ButtonOff;
				_button_pressed(_buttons[i].gpio.gpiox, _buttons[i].gpio.pin);
			} else {
				_buttons[i].off_count++;
			}
		}

	}
}

static btn_t* _find_button(uint16_t gpio_pin) {
	for (int i = 0; i < COUNT(_buttons); i++) {
		btn_t* btn = &_buttons[i];
		if (gpio_pin == btn->gpio.pin) {
			GPIO_PinState state = mw_gpio_readpin(btn->gpio.gpiox,
					btn->gpio.pin);
			if (btn->type == ButtonNormalOpen) {
				if (btn->state != (_btn_state) state)
					return btn;
			} else if (btn->type == ButtonNormalClose) {
				if (btn->state == (_btn_state) state)
					return btn;
			}
		}
	}
	return NULL;
}

void mgr_button_detect_state(uint16_t gpio_pin) {
	btn_t* btn = _find_button(gpio_pin);
	if (gpio_pin == btn->gpio.pin) {
		GPIO_PinState state = mw_gpio_readpin(btn->gpio.gpiox, btn->gpio.pin);
		if (btn->type == ButtonNormalOpen) {
			if (state == GPIO_PIN_SET) {
				btn->state = ButtonOn;
			} else {
				btn->state = ButtonOff;
			}
		} else {
			if (state == GPIO_PIN_RESET) {
				btn->state = ButtonOn;
			} else {
				btn->state = ButtonOff;
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
	}
	_timebase_init();
}

static void _timebase_init() {
	if (htim11.Instance == TIM11) {
		__HAL_RCC_TIM11_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
	}
}

void mw_tim11_reset_clk() {
	__HAL_RCC_TIM11_CLK_ENABLE();
}

static void __attribute__((unused)) _timebase_deinit() {
	if (htim11.Instance == TIM11) {
		__HAL_RCC_TIM11_CLK_DISABLE();
		HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
	}
}

void TIM1_TRG_COM_TIM11_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim11);
}

