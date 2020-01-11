/*
 * performance.h
 *
 *  Created on: Feb 17, 2019
 *      Author: Zviad
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"
#include <stdint.h>
#include "debug.h"
#include "defines.h"

/*
 * Should contain counters for function execution time
 * Should contain functions for system speed change, sleep, power management etc.
 *
 */

#define PERF_COUNTER_FREQ						1000000UL
#define PERF_OSTASKMEASURER_FREQ				10000UL
#define PERFORMANCE_COUNTER_TIMER 				TIM5
#define OSTASKMEASURER_TIMER					TIM4


void prf_measure_func_time();
void prf_start_measurement();
uint32_t prf_end_measurement();
void prf_timer_osmeasurer_init();
uint32_t prf_timer_osmeasurer_count_get();

#define prfMeasureFuncExecTime(_fn_) 					({prf_start_measurement(); _fn_; uint32_t _fet = prf_end_measurement(); _fet;});
#define prfMeasureFucnExecTimeAndLog(_fn_, delay) 		({ debug_uart3("Measuring execution time for function '%s'\n", #_fn_);\
														uint32_t __fet = prfMeasureFuncExecTime(_fn_);\
														if (delay) WL_wait_cycles(delay); \
													debug_uart3("Function execution took %luus\n", __fet); __fet;})





#ifdef __cplusplus
 }
#endif /* PERFORMANCE_H_ */
