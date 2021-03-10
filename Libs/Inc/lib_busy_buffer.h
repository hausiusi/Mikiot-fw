/*
 * lib_busy_buffer.h
 *
 *  Created on: Feb 22, 2019
 *      Author: Zviad
 */

#ifndef LIB_BUSY_BUFFER_H_
#define LIB_BUSY_BUFFER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "defines.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
    bool_t is_busy;
    uint32_t engaged_at;
} busy_t;

typedef struct {
    uint8_t* data;
    busy_t availability;
    uint32_t busy_wait_timeout;
} busy_buffer_t;

void lib_busy_buffer_init(busy_buffer_t* busy_buf, uint8_t* data);

void lib_busy_buffer_engage(busy_buffer_t* busy_buf, uint32_t timeout,
        uint32_t engaged_at);

void lib_busy_buffer_release(busy_buffer_t* busy_buf);

void lib_busy_buffer_write(busy_buffer_t* busy_buf, uint8_t* data, size_t len);

#ifdef __cplusplus
 }
#endif

#endif /* LIB_BUSY_BUFFER_H_ */
