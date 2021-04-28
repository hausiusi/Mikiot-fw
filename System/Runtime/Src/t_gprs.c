/*
 * t_gprs.c
 *
 *  Created on: Sep 1, 2020
 *      Author: Zviad
 */
#include <inttypes.h>
#include <string.h>
#include "debug.h"
#include "defines.h"
#include "mw_timebase.h"
#include "mw_uart.h"
#include "os_loader.h"
#include "mgr_gprs.h"
#include "utils.h"

#include "FreeRTOS.h"
#include "task.h"

static uint8_t _tx_buff[SIM800_CMD_BUFFER_SIZE];
static uint8_t _rx_buff[SIM800_RESP_BUFFER_SIZE];
static char _rx_expect_buff[SIM800_EXPECT_BUFFER_SIZE];
static gprs_state_t _gprs_state;

static void _gprs_data_received(uart_data_t* uart_data) {
    memcpy(_gprs_state.rx_buffer, uart_data->buffer,
            uart_data->allocated_length);
    _gprs_state.received_bytes_count = uart_data->allocated_length;
    _gprs_state.rx_status = Sim800StatusReady;
}

static void _debug_gprs_state() {
    static char trimbuffer[64];
    strtrim((char*) _gprs_state.tx_buffer, trimbuffer, sizeof(trimbuffer));
    debug_p("AT command %s completed with status \"%s\"\n", trimbuffer,
            gprs_status_tostr(_gprs_state.overall_status));
    strtrim((char*) _gprs_state.rx_expecting, trimbuffer, sizeof(trimbuffer));
    debug_p(" Expected: %s\n", trimbuffer);
    strtrim((char*) _gprs_state.rx_buffer, trimbuffer, sizeof(trimbuffer));
    debug_p(" Got:      %s\n", trimbuffer);
    debug_p(" Timeout:  %ims\n", _gprs_state.timeout_ms);
    debug_p(" Elapsed:  %ims\n", _gprs_state.elapsed_ms);
}

extern void thread_gprs(void* args) {
    uart_dma_conf_t* conf = mw_uart_dma_get_config(Uart2ConfigIndex);
    conf->rec_data_process = _gprs_data_received;
    mw_uart_dma_init(conf);
    _gprs_state.tx_buffer = _tx_buff;
    _gprs_state.rx_buffer = _rx_buff;
    _gprs_state.tx_buffer_max_length = sizeof(_tx_buff);
    _gprs_state.rx_buffer_max_length = sizeof(_rx_buff);
    _gprs_state.rx_expecting_max_length = sizeof(_rx_expect_buff);
    _gprs_state.rx_expecting = _rx_expect_buff;
    _gprs_state.tx_status = Sim800StatusReady;
    _gprs_state.rx_status = Sim800StatusUnknown;
    _gprs_state.overall_status = Sim800StatusUnknown;
    mgr_gprs_init(&_gprs_state, true);
    uint32_t start_time = 0;
    for (;;) {
        if (_gprs_state.rx_status == Sim800StatusReady) {
            if (strstr((char*) _gprs_state.rx_buffer, "ERROR")) {
                _gprs_state.overall_status = Sim800StatusFail;
            } else if (strstr((char*) _gprs_state.rx_buffer,
                    (char*) _gprs_state.rx_expecting)) {
                _gprs_state.overall_status = Sim800StatusSuccess;
            } else {
                // Here we can get if received data is incomplete as there
                // are some commands that are responding more than once and
                // not the first response is important for us. Setting status
                // to waiting will continue waiting until the valid response,
                // timeout or ERROR
                _gprs_state.rx_status = Sim800StatusWaiting;
                continue;
            }
            _gprs_state.rx_status = Sim800StatusProcessed;
            _gprs_state.tx_status = Sim800StatusReady;
            if (_gprs_state.callback) {
                _gprs_state.callback(&_gprs_state);
            } else if (_gprs_state.overall_status == Sim800StatusUnknown) {
                mgr_gprs_process_output(&_gprs_state);
            }
            _debug_gprs_state();
            gprs_state_reset();
        }
        if (_gprs_state.rx_status == Sim800StatusWaiting) {
            if (_gprs_state.elapsed_ms == 0) {
                start_time = mw_timebase_ticks_get();
                _gprs_state.elapsed_ms = 1;
            } else {
                _gprs_state.elapsed_ms = mw_timebase_ticks_get() - start_time;
                if (_gprs_state.elapsed_ms > _gprs_state.timeout_ms) {
                    _gprs_state.overall_status = Sim800StatusTimeout;
                    _gprs_state.tx_status = Sim800StatusReady;
                    _debug_gprs_state();
                    gprs_state_reset();
                }
            }
        }

        vTaskDelay(1);
    }
}
