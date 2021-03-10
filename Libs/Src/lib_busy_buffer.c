/*
 * lib_busy_buffer.c
 *
 *  Created on: Feb 22, 2019
 *      Author: Zviad
 */

#include "lib_busy_buffer.h"
#include "stm32f4xx.h"
#include <string.h>

static void _busy_buffer_wait_for_release(busy_buffer_t* busy_buf);
static void _wait_1ms();

void lib_busy_buffer_init(busy_buffer_t* busy_buf, uint8_t* data) {
    busy_buf->availability.is_busy = false;
    busy_buf->data = data;
    busy_buf->busy_wait_timeout = 0;
}

void lib_busy_buffer_engage(busy_buffer_t* busy_buf, uint32_t timeout,
        uint32_t engaged_at) {
    busy_buf->availability.is_busy = true;
    busy_buf->busy_wait_timeout = timeout;
    busy_buf->availability.engaged_at = engaged_at;
}

void lib_busy_buffer_release(busy_buffer_t* busy_buf) {
    busy_buf->availability.is_busy = false;
}

void lib_busy_buffer_write(busy_buffer_t* busy_buf, uint8_t* data, size_t len) {
    _busy_buffer_wait_for_release(busy_buf);
    memcpy(busy_buf->data, data, len);
}

static void _busy_buffer_wait_for_release(busy_buffer_t* busy_buf) {
    uint32_t tickstart = HAL_GetTick();
    uint32_t busy_wait = busy_buf->busy_wait_timeout;
    uint32_t elapsed = tickstart - busy_buf->availability.engaged_at;
    if (busy_wait < HAL_MAX_DELAY) {
        busy_wait += (uint32_t) (HAL_GetTickFreq());

        if (busy_wait <= elapsed) {
            return;
        }
        busy_wait -= elapsed;
    }

    while (busy_buf->availability.is_busy && busy_wait--) {
        _wait_1ms();
    }

    busy_buf->availability.is_busy = false;
}

/*
 * This function is a workaround for cases
 * when tick-counter doesn't increase because of blocked interrupts.
 */
static void _wait_1ms() {
    uint32_t cnt = SystemCoreClock / 4000;
    while (cnt--) {
        asm("NOP");
    }
}
