/*
 * mw_io.h
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#ifndef INC_MW_IO_H_
#define INC_MW_IO_H_

#include <inttypes.h>
#include "mw_redefines.h"
#include "error.h"

void mw_gpio_init(gpio_init_t* gpio_init_struct);
void mw_gpio_nvic_init(IRQn_Type irq, uint32_t preempt_priority,
        uint32_t sub_priority);
char mw_gpio_get_char(GPIO_TypeDef* gpiox);
int8_t mw_gpio_get_id(GPIO_TypeDef* gpiox);

#endif /* INC_MW_IO_H_ */
