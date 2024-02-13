/*
 * pwm.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Zviad
 */

#ifndef PWM_PWM_H_
#define PWM_PWM_H_

#define PWM_TIMER

#define PWM_OUT_PORT
#define PWM_OUT_PIN

#include "TimerPWM/timer_pwm.h"

/*
 STM32F411 datasheet:
 The STM32F411xC/xE devices are 4 full-featured general-purpose timers: TIM2, TIM5,
 TIM3, and TIM4.The TIM2 and TIM5 timers are based on a 32-bit auto-reload
 up/downcounter and a 16-bit prescaler. The TIM3 and TIM4 timers are based on a 16-
 bit auto-reload up/downcounter and a 16-bit prescaler. They all feature four
 independent channels for input capture/output compare, PWM or one-pulse mode
 output. This gives up to 15 input capture/output compare/PWMs.
 The TIM2, TIM3, TIM4, TIM5 general-purpose timers can work together, or with the
 other general-purpose timers and the advanced-control timer TIM1 via the Timer Link
 feature for synchronization or event chaining.
 Any of these general-purpose timers can be used to generate PWM outputs.
 TIM2, TIM3, TIM4, TIM5 all have independent DMA request generation. They are
 capable of handling quadrature (incremental) encoder signals and the digital outputs
 from 1 to 4 hall-effect sensors.
 */

#define pwm_init                               timer_pwm_init
#define pwm_set_value(channel, pulse)          timer_pwm_set_value(channel, pulse)

#endif /* PWM_PWM_H_ */
