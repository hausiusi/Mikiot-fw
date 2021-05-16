/*
 * applink.c
 *
 *  Created on: May 17, 2021
 *      Author: Zviad
 */
#include <string.h>
#include "applink.h"
#include "mgr_version.h"
#include "macrodefs.h"

static void _version(void* args);
static void _send_error(void* args);

uint8_t applink_tx_buffer[MCONF_APPLINK_BUFFER_SIZE];

static applink_commands_t _applink_functions[] = { {
        _version, "Gets FW, FreeRTOS and HAL versions" }, };

static void _version(void* args) {
    uint16_t len = 0, index = 0;
    mikiot_versions_t* versions = mgr_versions_get();
    memset(applink_tx_buffer, 0, sizeof(applink_tx_buffer));
    applink_tx_buffer[APPLINK_ID_DID] = 0;
    applink_tx_buffer[APPLINK_ID_PF] = 0;
    applink_tx_buffer[APPLINK_ID_FID] = 0;
    len = sizeof(mikiot_versions_t);
    memcpy(&applink_tx_buffer[APPLINK_ID_DATA], (uint8_t*) versions, len);
    versions = (mikiot_versions_t*) (&applink_tx_buffer[APPLINK_ID_DATA]);
    index = APPLINK_ID_DATA + len;
    len = strlen(versions->fw_code_name) + 1;
    strcpy((char*) (&applink_tx_buffer[index]), versions->fw_code_name);
    versions->fw_code_name = (char*) ((uint32_t) index);
    index += len;
    len = strlen(versions->fw_release_date) + 1;
    strcpy((char*) (&applink_tx_buffer[index]), versions->fw_release_date);
    versions->fw_release_date = (char*) ((uint32_t) index);
    len += index;
    applink_calculate_checksum(applink_tx_buffer, len);
    len += 2;
    applink_tx_buffer[APPLINK_ID_LEN] = len;
    applink_send_data(applink_tx_buffer, len);
}

static void _send_error(void* args) {

}

void applink_packet_process(uint8_t* data) {
    applink_packet_t* packet = (applink_packet_t*) data;
    packet->crc = *(uint16_t*) (&data[APPLINK_ID_LEN - 2]);
    if (packet->fid >= array_count(_applink_functions)) {
        _send_error(data);
        return;
    }
    _applink_functions[packet->fid].fp(packet->data);
}
