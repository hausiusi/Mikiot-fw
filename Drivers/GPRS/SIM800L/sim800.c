/*
 * sim800.c
 *
 * Created: Sep 10, 2020
 *  Author: Zviad
 */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "sim800.h"
#include "mw_redefines.h"
#include "mw_io.h"
#include "FreeRTOS.h"
#include "task.h"

#define SIM800_MODEM_RESET_PIN            MCONF_GPRS_RESET_PIN
#define SIM800_MODEM_RESET_PORT           MCONF_GPRS_RESET_PORT

/* @formatter:off */
static gpio_init_t sim800_rst_conf = {
	.gpiox = SIM800_MODEM_RESET_PORT,
	/* Can be used PB2 later as it has no alternate function
	 * current choice is because of location, it is close with PD6 and PD5
	 * on the board */
	.init.Pin = SIM800_MODEM_RESET_PIN,
	.init.Mode = GPIO_MODE_OUTPUT_PP,
	.init.Pull = GPIO_NOPULL,
	.init.Speed = GPIO_SPEED_FREQ_LOW,
};
/* @formatter:on */

static void _sim800_inouts_init();
static void _restart_if_fails(sim800_state_t* state);
static void _last_overall_forget();
static void _wait_until_cmd_process();

static sim800_state_t* _state;
static bool_t _debug_enabled;
static sim800_status_t _last_overall_status;

#define delay(milliseconds)                         vTaskDelay(milliseconds);
#define send_string(str_text)                       mw_uart_dma_string_transmit(Uart2ConfigIndex, str_text)
#define send_bytes(bytes, len)                      mw_uart_dma_data_transmit(Uart2ConfigIndex, bytes, len)
#define reset_pin_set_hi()                          mw_gpio_setpin(sim800_rst_conf.gpiox, sim800_rst_conf.init.Pin)
#define reset_pin_set_low()                         mw_gpio_resetpin(sim800_rst_conf.gpiox, sim800_rst_conf.init.Pin)
#define clean_buff(buffer)                          do {memset(buffer, 0, sizeof(buffer)); } while(0)
#define _debug(str_format, vargs...)                do { if(_debug_enabled) debugf(str_format, ##vargs); } while(0)

/*
 * Returns from function with false if `expr` is false.
 */
#define return_if_false(expr)                       do {bool_t __check = expr; if (!__check) return __check; } while(0)
/*
 * Switches context until `expr` is true and exits when it is false or when context will be switched more than max_cnt
 * and prints error text that is provided
 *
 * @params expr expression that must be true, can be a function (that will be called every time, comparison etc.
 * @params t task delay (sleep) time between context switches
 * @params max_cnt max count of context switching to avoid infinite loops
 * @erptr pointer to error string that will be printed in case of _cnt reached max_cnt
 */
#define switch_ctx_until(expr, t, max_cnt, erptr)  do { \
                                                          uint32_t _cnt = 0; \
                                                          while(expr) { \
                                                              delay(t); \
                                                              if (_cnt++ > max_cnt) {\
                                                                  _debug(erptr); \
                                                                  break; \
                                                              } \
                                                          }\
                                                      } while(0)

bool_t sim800_init(sim800_state_t* state) {
    if (!state) {
        // TODO: Handle error
        return false;
    }
    _state = state;
    _last_overall_status = Sim800StatusUnknown;
    _sim800_inouts_init();
    reset_pin_set_hi();
    sim800_cmd_send("ATE0\r\n", "OK", 10, _restart_if_fails);
    return true;
}

void sim800_restart() {
    _debug("Restarting sim800\n");
    reset_pin_set_hi();
    delay(1000);
    reset_pin_set_low();
    delay(2000);
    reset_pin_set_hi();
    delay(5000);
}

void sim800_debug_set(bool_t enable) {
    _debug_enabled = enable;
}

sim800_status_t sim800_last_command_status() {
// If status is more than processed means that success, fail or timeout is set.
    return _state->overall_status > Sim800StatusProcessed ?
            _state->overall_status : _last_overall_status;
}

bool_t sim800_state_reset() {
    if (!_state->rx_buffer || !_state->tx_buffer || !_state->rx_expecting) {
        //TODO: Handle error
        return false;
    }
    memset(_state->rx_buffer, 0, _state->rx_buffer_max_length);
    memset(_state->tx_buffer, 0, _state->tx_buffer_max_length);
    memset(_state->rx_expecting, 0, _state->rx_expecting_max_length);
// Save last overall status before resetting
    _last_overall_status = _state->overall_status;
    _state->tx_status = Sim800StatusReady;
    _state->rx_status = Sim800StatusUnknown;
    _state->overall_status = Sim800StatusUnknown;
    _state->callback = NULL;
    _state->timeout_ms = 0;
    _state->elapsed_ms = 0;
    return true;
}

bool_t sim800_cmd_send(const char* cmd, const char* expecting, uint32_t timeout,
        void* callback) {
    return sim800_bytes_send((uint8_t*) cmd, strlen(cmd), expecting, timeout,
            callback);
}

bool_t sim800_bytes_send(const uint8_t* bytes, uint32_t len,
        const char* expecting, uint32_t timeout, void* callback) {
    if (_state->tx_status != Sim800StatusReady
            || _state->overall_status == Sim800StatusBusy) {
        return false;
    }
    _last_overall_forget();
    _state->tx_buffer_length = len;
    _state->tx_status = Sim800StatusBusy;

    memset(_state->tx_buffer, 0, _state->tx_buffer_max_length);
    memset(_state->rx_expecting, 0, _state->rx_expecting_max_length);
    memset(_state->rx_buffer, 0, _state->rx_buffer_max_length);

    memcpy(_state->tx_buffer, bytes, len);
    memcpy(_state->rx_expecting, expecting, strlen(expecting));
    _state->rx_status = Sim800StatusWaiting;
    _state->timeout_ms = timeout;
    _state->callback = callback;
    send_bytes(_state->tx_buffer, len);
    _state->overall_status = Sim800StatusBusy;
    _state->elapsed_ms = 0;
    return true;
}

bool_t sim800_call(uint32_t country_code, uint32_t number) {
    char cmd[21] = { 0 };
    sprintf(cmd, "ATD+ +%lu%lu;", country_code, number);
    return sim800_cmd_send(cmd, "OK", 100, 0);
}

bool_t sim800_sms_send(uint32_t country_code, uint32_t number,
        const char* message) {
    return_if_false(
            sim800_cmd_send("AT+CMGF=1\r\n", "OK", 20, _restart_if_fails));
    _wait_until_cmd_process();
    return_if_false(sim800_last_command_status() == Sim800StatusSuccess);
    char cmd[25] = { 0 };
    sprintf(cmd, "AT+CMGS=\"+%lu%lu\"\r\n", country_code, number);
    delay(10);
    return_if_false(sim800_cmd_send(cmd, ">", 5000, _restart_if_fails));
    _wait_until_cmd_process();
    return_if_false(sim800_last_command_status() == Sim800StatusSuccess);
    delay(10);
    return_if_false(sim800_cmd_send(message, "", 10, 0));
    delay(10);
    uint8_t end = 26;
    send_bytes(&end, 1);
    return true;
}

bool_t sim800_rssi_percentage(void* callback) {
    return sim800_cmd_send("AT+CSQ\r\n", "+CSQ:", 10, callback);
}

bool_t sim800_time_get(void* callback) {
    return sim800_cmd_send("AT+CCLK?\r\n", "+CCLK:", 20, callback);
}

bool_t sim800_time_set(char* dt) {
    char cmd[31] = { 0 };
    sprintf(cmd, "AT+CCLK=\"%s\"\r\n", dt);
    return sim800_cmd_send(cmd, "OK", 50, 0);
}

bool_t sim800_join(const char* apn, const char* user_name, const char* password) {
    sim800_cmd_send("AT+CIFSR\r\n", "", 50, 0);
    _wait_until_cmd_process();
    if (sim800_last_command_status() != Sim800StatusSuccess) {
        sim800_cmd_send("AT+CIPSHUT\r\n", "SHUT OK", 5000, 0);
        _wait_until_cmd_process();
        uint8_t len = strlen(apn) + strlen(user_name) + strlen(password) + 20;
        char tmp[len];
        memset(tmp, 0, len);
        sprintf(tmp, "AT+CSTT=\"%s\",\"%s\",\"%s\"\r\n", apn, user_name,
                password);
        sim800_cmd_send(tmp, "OK", 1000, 0);
        _wait_until_cmd_process();
        return_if_false(sim800_last_command_status() == Sim800StatusSuccess);
        sim800_cmd_send("AT+CIPSCONT\r\n", "OK", 20, 0);
        _wait_until_cmd_process();
        sim800_cmd_send("AT+CIICR\r\n", "OK", 2000, 0);
        _wait_until_cmd_process();
        sim800_cmd_send("AT+CIFSR\r\n", "", 50, 0);
        _wait_until_cmd_process();
    }
    return sim800_last_command_status() == Sim800StatusSuccess;
}

bool_t sim800_send_tcp_data(char* server_ip, uint16_t server_port,
        uint8_t* data, uint8_t len) {
    sim800_cmd_send("AT+CIPSHUT\r\n", "SHUT OK", 5000, 0);
    _wait_until_cmd_process();
    sim800_cmd_send("AT+CIPMUX=0\r\n", "OK", 3000, 0);
    _wait_until_cmd_process();
    uint8_t tmp_len = strlen(server_ip) + 31;
    char tmp[tmp_len];
    memset(tmp, 0, tmp_len);
    sprintf(tmp, "AT+CIPSTART=\"TCP\",\"%s\",\"%u\"\r\n", server_ip,
            server_port);
    sim800_cmd_send(tmp, "CONNECT OK", 10000, 0);
    _wait_until_cmd_process();
    return_if_false(sim800_last_command_status() == Sim800StatusSuccess);
    sprintf(tmp, "AT+CIPSEND=%u\r\n", len);
    sim800_cmd_send(tmp, ">", 100, 0);
    _wait_until_cmd_process();
    return_if_false(sim800_last_command_status() == Sim800StatusSuccess);
    sim800_bytes_send(data, len, "SEND OK", 10000, 0);
    _wait_until_cmd_process();
    return_if_false(sim800_last_command_status() == Sim800StatusSuccess);
    return true;
}

bool_t sim800_connection_close(uint8_t index) {
    sim800_cmd_send("AT+CIPCLOSE\r\n", "CLOSE OK", 50, 0);
    _wait_until_cmd_process();
    return sim800_last_command_status() == Sim800StatusSuccess;
}

bool_t sim800_send_data_inline(uint8_t* data, uint8_t len) {
    sim800_bytes_send(data, len, "SEND OK", 10000, 0);
    _wait_until_cmd_process();
    return_if_false(sim800_last_command_status() == Sim800StatusSuccess);
    return true;
}

bool_t sim800_start_server(uint16_t port) {
    sim800_cmd_send("AT+CIPSTATUS\r\n", "SERVER LISTENING", 50, 0);
    _wait_until_cmd_process();
    if (sim800_last_command_status() == Sim800StatusSuccess) {
        return true;
    }
    sim800_cmd_send("AT+CIPSHUT\r\n", "SHUT OK", 5000, 0);
    _wait_until_cmd_process();
    char tmp[23];
    sprintf(tmp, "AT+CIPSERVER=1,%u\r\n", port);
    sim800_cmd_send(tmp, "SERVER OK", 3000, 0);
    return false;
}

const char* sim800_status_tostr(sim800_status_t status) {
    /* @formatter:off */
	const char* statuses[] = {
		"Unknown",
		"Forgotten",
		"Ready",
		"Busy",
		"Waiting",
		"Processed",
		"Success",
		"Fail",
		"Timeout",
	};
	/* @formatter:on */
    return statuses[status];
}

/* local functions */

static void _sim800_inouts_init() {
    mw_gpio_init(&sim800_rst_conf);
}

static void _restart_if_fails(sim800_state_t* state) {
    if (state->overall_status != Sim800StatusSuccess) {
        _debug("Command %s failed. restarting as requested\n",
                state->tx_buffer);
        sim800_restart();
        // This line sends sim800 to permanent restart if disable echo will fail
        sim800_cmd_send("ATE0\r\n", "OK", 10, _restart_if_fails);
    }
}

static void _last_overall_forget() {
    _last_overall_status = Sim800StatusForgotten;
}

static void _wait_until_cmd_process() {
    static const char* error =
            "AT process takes too long. Make sure that you call sim800_state_reset function after processing the response";
    // While overal_status is busy means we are waiting for receive
    switch_ctx_until(_state->overall_status == Sim800StatusBusy, 1, 30000,
            error);
    // After every processing state_reset function must be called. It resets overal_status to StatusUnknown
    // then we know that all job is done from user side
    switch_ctx_until(_state->overall_status != Sim800StatusUnknown, 1, 30000,
            error);
    // Additional delay if there are text to print (can be later considered and removed)
    delay(10);
}
