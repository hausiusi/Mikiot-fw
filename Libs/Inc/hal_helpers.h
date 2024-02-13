/*
 * hal_helpers.h
 *
 *  Created on: Feb 9, 2024
 *      Author: Zviad
 */

#ifndef INC_HAL_HELPERS_H_
#define INC_HAL_HELPERS_H_

#define hal_helper_gpio_clock_enable(gpiox)     do { SET_BIT(RCC->AHB1ENR,\
                                                    (0x1UL << (((uint32_t)gpiox - (uint32_t)GPIOA)/0x0400UL)));\
                                                    /* Delay after an RCC peripheral clock enabling */\
                                                    __IO uint32_t tmpreg = 0;\
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);\
                                                    UNUSED(tmpreg); } while(0)

#endif /* INC_HAL_HELPERS_H_ */
