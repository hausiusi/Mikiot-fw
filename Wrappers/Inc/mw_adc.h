/*
 * mw_adc.h
 *
 *  Created on: Jul 25, 2020
 *      Author: Zviad
 */

#ifndef INC_MW_ADC_H_
#define INC_MW_ADC_H_

#include <stdint.h>

void mw_adc1_init(void* adc_ht_fp, void* adc_tc_fp);
void mw_adc_deinit();

#endif /* INC_MW_ADC_H_ */
