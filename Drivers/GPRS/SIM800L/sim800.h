/*
 * sim800.h
 *
 * Created: 22-Aug-16 17:33:46
 *  Author: Zviad
 */

#ifndef SIM800_H_
#define SIM800_H_

#include "defines.h"
#include "debug.h"
#include "mw_redefines.h"
#include "mw_uart.h"
#include "mikiot_config.h"

#define SIM800_CMD_BUFFER_SIZE            MCONF_GPRS_CMD_BUFFER_SIZE
#define SIM800_RESP_BUFFER_SIZE           MCONF_GPRS_RESP_BUFFER_SIZE
#define SIM800_EXPECT_BUFFER_SIZE         MCONF_GPRS_EXPECT_BUFFER_SIZE

/* @formatter:off */
// Don't rearrange statuses. They are arranged the way from the beginning to end of command status
// And are used in `sim800_last_command_status` function to decide if command is processed already
// or not. Also it is used in sim800_status_tostr function with the same order.
typedef enum {
	Sim800StatusUnknown,
	Sim800StatusForgotten,
	Sim800StatusReady,
	Sim800StatusBusy,
	Sim800StatusWaiting,
	Sim800StatusProcessed,
	Sim800StatusSuccess,
	Sim800StatusFail,
	Sim800StatusTimeout,
} sim800_status_t;
/* @formatter:on */

typedef struct {
    rtc_date_t date;
    rtc_time_t time;
    uint8_t time_zone;
} sim800_datetime_t;

typedef struct sim800_state_t {
    uint8_t* tx_buffer;
    uint32_t tx_buffer_length;
    uint32_t tx_buffer_max_length;
    uint8_t* rx_buffer;
    uint32_t rx_buffer_max_length;
    uint32_t received_bytes_count;
    uint32_t timeout_ms;
    uint32_t elapsed_ms;
    char* rx_expecting;
    uint32_t rx_expecting_max_length;
    sim800_status_t tx_status;
    sim800_status_t rx_status;
    sim800_status_t overall_status;
    void (*callback)(struct sim800_state_t* self);
} sim800_state_t;

/*
 * Initializes reset pin for SIM800 and disables echo
 * In case of fail restarts it
 *
 * @returns:
 * 	   true initialization successful and command send successfully
 * 	   false initialization failed due to incorrect parameter
 */
bool_t sim800_init(sim800_state_t* state);

/*
 * Restarts sim800 module using reset pin
 *
 */
void sim800_restart();

/*
 * Enables or disables debug
 *
 * @param enable true or false according to desired option if debug
 * needs to be set or not
 */
void sim800_debug_set(bool_t enable);

/*
 * Sends command to the sim800 modem using dedicated (configurable) interface
 *
 * @param cmd command
 * @param expecting data that we expect as a success response for the command
 *        can be an empty string when can't be predicted or the response is changing
 * @param timeout maximum amount of time in milliseconds to wait for the response
 *        until it will be marked as timed out
 * @param callback function func(sim800_state_t*) that will be called after any
 *        response from the modem. Can be null. Won't be called in case of timeout
 *
 * @returns
 *        true if send succeeded
 *        false if send failed
 */
bool_t sim800_cmd_send(const char* cmd, const char* expecting, uint32_t timout,
        void* callback);

/*
 * Sends bytes to the sim800 modem using dedicated (configurable) interface
 *
 * @param bytes pointer to array to send
 * @param len length of the array to send
 * @param expecting data that we expect as a success response for the command
 *        can be an empty string when can't be predicted or the response is changing
 * @param timeout maximum amount of time in milliseconds to wait for the response
 *        until it will be marked as timed out
 * @param callback function func(sim800_state_t*) that will be called after any
 *        response from the modem. Can be null. Won't be called in case of timeout
 *
 * @returns
 *        true if send succeeded
 *        false if send failed
 */
bool_t sim800_bytes_send(const uint8_t* bytes, uint32_t len,
        const char* expecting, uint32_t timeout, void* callback);

/*
 * Calls the number
 *
 * @param country_code without + sign
 * @param number of the recipient
 *
 * @returns
 *        true if succeeded
 *        false if failed
 */
bool_t sim800_call(uint32_t country_code, uint32_t number);

/*
 * Sends the sms
 *
 * @param country_code without + sign
 * @param number of the recipient
 * @param message text for the sms
 *
 * @returns
 *        true if send succeeded
 *        false if send failed
 */
bool_t sim800_sms_send(uint32_t country_code, uint32_t number,
        const char* message);

bool_t sim800_rssi_percentage(void* callback);

/*
 * Resets current sim800_state structure to its defaults, also clears the buffers
 *
 * @returns
 *        true if succeeded
 *        false if failed
 */
bool_t sim800_state_reset();

/*
 * Gets last command status if it exists
 *
 * @returns last overall status
 */
sim800_status_t sim800_last_command_status();

/*
 *  Gets GPRS time
 *
 *  @params  callback calls function after answer is returned
 *
 * @returns
 *        true if succeeded
 *        false if failed
 */
bool_t sim800_time_get(void* callback);

/*
 * Sets GPRS time
 *
 * @params dt sets time according to its value
 */
bool_t sim800_time_set(char* dt);

/*
 * Joins APN
 *
 * @params apn name of provider
 * @params user_name for the APN
 * @params password for the provided APN and user name
 *
 * @returns
 *        true if succeeded
 *        false if failed
 */
bool_t sim800_join(const char* apn, const char* user_name, const char* password);

bool_t sim800_send_tcp_data(char* server_ip, uint16_t server_port,
        uint8_t* data, uint8_t len);

bool_t sim800_connection_close(uint8_t index);

bool_t sim800_send_data_inline(uint8_t* data, uint8_t len);

bool_t sim800_start_server(uint16_t port);

/*
 * Converts passed status enumerator into string
 *
 * @returns status name as a constant string
 */
const char* sim800_status_tostr(sim800_status_t status);

#endif /* SIM800_H_ */
