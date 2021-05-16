/*
 * applink.h
 *
 *  Created on: May 17, 2021
 *      Author: Zviad
 */

#ifndef UI_INC_APPLINK_H_
#define UI_INC_APPLINK_H_

#include <inttypes.h>
#include "defines.h"
#include "mw_uart.h"

#define APPLINK_ID_LEN      0
#define APPLINK_ID_DID      2
#define APPLINK_ID_PF       3
#define APPLINK_ID_FID      4
#define APPLINK_ID_DATA     5

#define applink_calculate_checksum(buffer, len)
#define applink_send_data(data, len) {mw_uart_dma_data_transmit(Uart6ConfigIndex, data, len); }

typedef void (*applink_func_t)(void*);

typedef struct {
    applink_func_t fp;
    char* help;
} applink_commands_t;

typedef struct {
    uint16_t len;
    uint8_t did;
    uint8_t pf;
    uint8_t fid;
    uint8_t* data;
    uint16_t crc;
} __attribute__((packed, aligned(1))) applink_packet_t;

void applink_packet_process(uint8_t* data);

#endif /* UI_INC_APPLINK_H_ */
