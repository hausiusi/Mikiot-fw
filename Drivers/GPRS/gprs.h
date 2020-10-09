/*
 * gprs.h
 *
 *  Created on: Sep 17, 2020
 *      Author: Zviad
 */

#ifndef GPRS_GPRS_H_
#define GPRS_GPRS_H_

#define USE_GPRS_MODEM_SIM800L 1

#if (USE_GPRS_MODEM_SIM800L == 1)

#include "SIM800L/sim800.h"

typedef sim800_state_t gprs_state_t;

#define gprs_init(state)                                                sim800_init(state)

#define gprs_restart                                                    sim800_restart
#define gprs_cmd_send(cmd, exp, timeout, callback)                      sim800_cmd_send(cmd, exp, timeout, callback)
#define gprs_state_reset                                                sim800_state_reset
#define gprs_debug_set(enable)                                          sim800_debug_set(enable)
#define gprs_status_tostr(status)                                       sim800_status_tostr(status)
#define gprs_call(country_code, number)                                 sim800_call(country_code, number)
#define gprs_sms_send(country_code, number, message)                    sim800_sms_send(country_code, number, message)
#define gprs_last_cmd_status                                            sim800_last_command_status
#define gprs_rssi_percentage(callback)                                  sim800_rssi_percentage(callback)
#define gprs_time_set(dt)                                               sim800_time_set(dt)
#define gprs_time_get(callback)                                         sim800_time_get(callback)
#define gprs_join(apn, user_name, password)                             sim800_join(apn, user_name, password)
#define gprs_send_tcp_data(server_ip, server_port, data, len)           sim800_send_tcp_data(server_ip, server_port, data, len)
#define gprs_connection_close(index)                                    sim800_connection_close(index)
#define gprs_start_server(port)                                         sim800_start_server(port)
#define gprs_send_data_inline(data, len)                                sim800_send_data_inline(data, ulen)
#else
#error "There is no valid GPRS modem defined"
#endif

#endif /* GPRS_GPRS_H_ */
