/*
 * mgr_adc.h
 *
 *  Created on: Jul 29, 2020
 *      Author: Zviad
 */

#ifndef MANAGERS_MGR_ADC_H_
#define MANAGERS_MGR_ADC_H_

#include <inttypes.h>
#include "defines.h"

/* @formatter:off */
typedef struct {
	uint32_t initialized :1;        /* Points whether ADC is initialized or not */
	uint32_t print_value :1;        /* Points whether converted values must be printed or not */
	uint32_t print_voltage :1;      /* Points whether converted values must be converted to voltage and printed or not */
	uint32_t permanent :1;          /* ADC won't deinitialize automatically if this bit is set */
	uint32_t amount;                /* Counts how many times converted values must be processed (can be used by thread or loop)
                                       after that adc will be automatically deinitialized (is ignored when permanent flag is set*/
	uint32_t delay;                 /* Artificial delay after every processing before the next one starts */
	double ref_voltage;             /* Reference voltage value */
} adc_uiconf_t;
/* @formatter:on */

void mgr_adc_init();
void mgr_adc_deinit();
void mgr_adc_print_value(bool_t print);
void mgr_adc_print_voltage(bool_t print);
adc_uiconf_t* mgr_adc_get_uiconf();
void adc_ht_detected(void* arg, uint32_t len);
void adc_tc_detected(void* arg, uint32_t len);

#endif /* MANAGERS_MGR_ADC_H_ */
