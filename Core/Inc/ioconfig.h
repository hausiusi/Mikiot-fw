/*
 * ioconfig.h
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#include "stm32f4xx_hal.h"

#ifndef INC_IOCONFIG_H_
#define INC_IOCONFIG_H_

/* GPIO */
#define BTN1_GPIO               GPIOA
#define BTN1_GPIO_PIN           GPIO_PIN_0

#define BTN2_GPIO               GPIOA
#define BTN2_GPIO_PIN           GPIO_PIN_1

#define BTN3_GPIO               GPIOB
#define BTN3_GPIO_PIN           GPIO_PIN_2

/* ADC */
#define ADC1_IN1_GPIO           GPIOB
#define ADC1_IN1_GPIO_PIN       GPIO_PIN_0

#endif /* INC_IOCONFIG_H_ */
