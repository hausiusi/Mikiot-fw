/*
 * mgr_adc.c
 *
 *  Created on: Aug 3, 2020
 *      Author: Zviad
 */
#include <inttypes.h>
#include <string.h>
#include "debug.h"
#include "defines.h"
#include "mgr_adc.h"
#include "mw_adc.h"

adc_uiconf_t adc_uiconf;

void mgr_adc_init() {
    if (!adc_uiconf.initialized) {
        mw_adc1_init(adc_ht_detected, adc_tc_detected);
        adc_uiconf.initialized = true;
    }
}

void mgr_adc_deinit() {
    if (adc_uiconf.initialized) {
        mw_adc_deinit();
        memset(&adc_uiconf, 0, sizeof(adc_uiconf_t));
    }
}

void mgr_adc_print_voltage(bool_t print) {
    adc_uiconf.print_voltage = print;
}

void mgr_adc_print_value(bool_t print) {
    adc_uiconf.print_value = print;
}

adc_uiconf_t* mgr_adc_get_uiconf() {
    return &adc_uiconf;
}

__attribute__((weak))
void adc_ht_detected(void* arg, uint32_t len) {
    debug_p("ADC half transfer detected is not implemented\n");
}

__attribute__((weak))
void adc_tc_detected(void* arg, uint32_t len) {
    debug_p("ADC transfer complete detected is not implemented\n");
}
