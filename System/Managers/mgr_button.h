/*
 * mgr_button.h
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#ifndef MANAGERS_MGR_BUTTON_H_
#define MANAGERS_MGR_BUTTON_H_

tim_handle_t htim11;

typedef enum {
	ButtonNormalOpen = 0, /* Normal open button */
	ButtonNormalClose = 1 /* Normal close button */
} btn_enum_t;

typedef enum {
	ButtonOff = 0, /* Normal open button */
	ButtonOn = 1 /* Normal close button */
} _btn_state;

typedef struct {
	GPIO_TypeDef* gpiox;
	uint16_t pin;
	uint32_t mode;
	uint32_t pull;
} gpio_t;

typedef struct {
	uint16_t irq;
	uint32_t preempt_priority;
	uint32_t sub_priority;
} nvic_param_t;

typedef struct {
	gpio_t gpio;
	btn_enum_t type;
	uint32_t on_count;
	uint32_t off_count;
	_btn_state state;
	_btn_state confirmed_state;
	uint32_t top_lvl;
	nvic_param_t nvic;
} btn_t;

#define timer_init_typedef TIM_HandleTypeDef

btn_t* get_button(uint32_t i);
void mgr_button_init(uint32_t tim_period_us);
void key_press_handle();
void mgr_button_detect_state(uint16_t gpio_pin);

#endif /* MANAGERS_MGR_BUTTON_H_ */
