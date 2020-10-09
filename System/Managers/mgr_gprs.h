/*
 * mgr_gprs.h
 *
 *  Created on: Sep 8, 2020
 *      Author: Zviad
 */

#ifndef MANAGERS_MGR_GPRS_H_
#define MANAGERS_MGR_GPRS_H_

#include "gprs.h"

typedef struct {
    uint8_t rssi_percent;
    rtc_date_t date;
    rtc_time_t time;
    uint8_t time_zone;
    uint32_t ip;
} mgr_gprs_status_t;

mgr_gprs_status_t mgr_gprs_status;

bool_t mgr_gprs_init(gprs_state_t* state, bool_t enable_debug);

void mgr_gprs_process_output(gprs_state_t* state);

#endif /* MANAGERS_MGR_GPRS_H_ */
