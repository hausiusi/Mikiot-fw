/*
 * mw_io.c
 *
 *  Created on: Jun 4, 2020
 *      Author: saturn
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "mw_redefines.h"
#include "inttypes.h"
#include "mw_io.h"

void mw_gpio_init(gpio_init_t* gpio_init_struct) {
    /* GPIO Ports Clock Enable */
    SET_BIT(RCC->AHB1ENR,
            (0x1UL << (((uint32_t)gpio_init_struct->gpiox - (uint32_t)GPIOA)/0x0400UL)));
    /* Delay after an RCC peripheral clock enabling */
    __IO uint32_t tmpreg = 0;
    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);
    UNUSED(tmpreg);
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(gpio_init_struct->gpiox, gpio_init_struct->init.Pin,
            GPIO_PIN_RESET);
    HAL_GPIO_Init(gpio_init_struct->gpiox, &gpio_init_struct->init);
}

void mw_gpio_nvic_init(IRQn_Type irq, uint32_t preempt_priority,
        uint32_t sub_priority) {
    /* EXTI interrupt init */
    HAL_NVIC_SetPriority(irq, preempt_priority, sub_priority);
    HAL_NVIC_EnableIRQ(irq);
}

char mw_gpio_get_char(GPIO_TypeDef* gpiox) {
    uint32_t gpiox_addr = (uint32_t) gpiox;
    switch (gpiox_addr) {
    case (uint32_t) GPIOA:
        return 'A';
    case (uint32_t) GPIOB:
        return 'B';
    case (uint32_t) GPIOC:
        return 'C';
    case (uint32_t) GPIOD:
        return 'D';
    case (uint32_t) GPIOE:
        return 'E';
    case (uint32_t) GPIOH:
        return 'H';
    default:
        return '-';
    }

    return '-';
}

void EXTI0_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
