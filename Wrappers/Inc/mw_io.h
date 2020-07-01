/*
 * mw_io.h
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#ifndef INC_MW_IO_H_
#define INC_MW_IO_H_

#include "mw_redefines.h"
#include "inttypes.h"
#include "error.h"

typedef struct {
	GPIO_TypeDef* gpiox; /* Specifies the GPIO port to be configured.
	 This parameter can be any available GPIO_port */
	GPIO_InitTypeDef init; /* GPIO configuration structure */
} gpio_init_t;

void mw_gpio_init(gpio_init_t* gpio_init_struct);
void mw_gpio_nvic_init(IRQn_Type irq, uint32_t preempt_priority,
		uint32_t sub_priority);

#endif /* INC_MW_IO_H_ */
