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

#define prf_func_exect_time_get(_fn_) 					({prf_start_measurement(); _fn_; uint32_t _fet = prf_end_measurement(); _fet;})
#define prf_func_exect_time_log(_fn_, delay) 		({ debug_p("Measuring execution time for function '%s'\n", #_fn_);\
														uint32_t __fet = prf_func_exect_time_get(_fn_);\
														if (delay) def_wait_cycles(delay); \
													debug_p("Function execution took %lu us\n", __fet); __fet;})

#ifdef __cplusplus
 }
#endif /* PERFORMANCE_H_ */
