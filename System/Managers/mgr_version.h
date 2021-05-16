/*
 * mgr_version.h
 *
 *  Created on: May 16, 2021
 *      Author: Zviad
 */

#ifndef MANAGERS_MGR_VERSION_H_
#define MANAGERS_MGR_VERSION_H_

#include <inttypes.h>

typedef union {
    /* Compile according to byte order */
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    struct {
        uint8_t sub3;
        uint8_t sub2;
        uint8_t sub1;
        uint8_t main;
    };
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    struct {
        uint8_t main;
        uint8_t sub1;
        uint8_t sub2;
        uint8_t sub3;
        };
#else
#error "Endianess is not defined"
#endif
    uint32_t joined;
} version_t;

typedef struct {
    version_t fw_version;
    char* fw_code_name;
    char* fw_release_date;
    version_t rtos_version;
    version_t hal_version;
} mikiot_versions_t;

mikiot_versions_t* mgr_versions_get();

#endif /* MANAGERS_MGR_VERSION_H_ */
