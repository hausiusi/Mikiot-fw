/*
 * lib_ringbuffer.c
 *
 *  Created on: Feb 21, 2019
 *      Author: Zviad
 */

/*
 rp      wp
 __>|0|1|2|3|4|5|6|7|8|9|__
 |						   |
 |__________________________|


 */

#include "lib_ringbuffer.h"
#include <stdlib.h>

#define _ringbuffer_allows_overlap(ringbuffer_ptr)	(check_bit(ringbuffer_ptr->status, AllowsOverlap))
#define _ringbuffer_wp_reached_rp(ringbuffer_ptr)	(check_bit(ringbuffer_ptr->status, WritePointerReachedReadPointer))
#define _ringbuffer_overlapped(ringbuffer_ptr)		(check_bit(ringbuffer_ptr->status, Overlapped))

static void _ringbuffer_write(ringbuffer_t* ringbuffer, uint8_t* data,
        size_t len);
static void _ringbuffer_read(ringbuffer_t* ringbuffer, uint8_t* data,
        size_t len);
static inline size_t _ringbuffer_available_space(ringbuffer_t* ringbuffer);
static inline size_t _ringbuffer_space_left_till_end(ringbuffer_t* ringbuffer);
static inline size_t _ringbuffer_read_left_till_end(ringbuffer_t* ringbuffer);
static inline uint8_t* _ringbuffer_data_last_addr(ringbuffer_t* ringbuffer);
static inline void _ringbuffer_inc_write_pos(ringbuffer_t* ringbuffer,
        size_t by);
static inline void _ringbuffer_inc_read_pos(ringbuffer_t* ringbuffer, size_t by);

void lib_ringbuffer_init(ringbuffer_t* ringbuffer, uint8_t* data,
        uint8_t element_len, size_t len, bool_t allows_overlap,
        void* overlap_detected) {
    ringbuffer->capacity = len;
    ringbuffer->data_aloc_start = data;
    ringbuffer->read_pos = data;
    ringbuffer->write_pos = data;
    ringbuffer->available_space = len;
    ringbuffer->element_len = element_len;
    if (allows_overlap) {
        set_bit(ringbuffer->status, AllowsOverlap);
        ringbuffer->ringbuffer_overlapped = overlap_detected;
    }
}

size_t lib_ringbuffer_bytes_to_read(ringbuffer_t* ringbuffer) {
    return ringbuffer->capacity - _ringbuffer_available_space(ringbuffer); // TODO: modify as overlapping can affect this part badly it should be calculated according to overlap or wp reached rp bit
}

bool_t lib_ringbuffer_enq(ringbuffer_t* ringbuffer, uint8_t* data, size_t len) {
    if (len > ringbuffer->capacity)
        return false;
    bool_t overlapping = len > _ringbuffer_available_space(ringbuffer);
    if (!_ringbuffer_allows_overlap(ringbuffer)) {
        /* If ringbuffer doesn't support overlapping it will block the thread until
         * enough space won't be available, so it must be freed: dequeued by another
         * thread, timer or by another core
         */
        if (overlapping) {
            while (len >= _ringbuffer_available_space(ringbuffer))
                ;
        }
        overlapping = len > _ringbuffer_available_space(ringbuffer);
    }
    _ringbuffer_write(ringbuffer, data, len);
    if (overlapping) {
        ringbuffer->read_pos = ringbuffer->write_pos;
        set_bit(ringbuffer->status, Overlapped);
        if (ringbuffer->ringbuffer_overlapped) {
            ringbuffer->ringbuffer_overlapped();
        }
    }
    if (ringbuffer->read_pos == ringbuffer->write_pos) {
        set_bit(ringbuffer->status, WritePointerReachedReadPointer);
    }
    return true;
}

bool_t lib_ringbuffer_deq(ringbuffer_t* ringbuffer, uint8_t* data, size_t len) {
    if (ringbuffer->read_pos
            == ringbuffer->write_pos&& !_ringbuffer_wp_reached_rp(ringbuffer))
        return false;

    if (len > ringbuffer->capacity
            || len > lib_ringbuffer_bytes_to_read(ringbuffer))
        return false;

    _ringbuffer_read(ringbuffer, data, len);
    if (_ringbuffer_wp_reached_rp(ringbuffer)) {
        clear_bit(ringbuffer->status, WritePointerReachedReadPointer);
    }
    if (_ringbuffer_overlapped(ringbuffer)) {
        clear_bit(ringbuffer->status, Overlapped);
    }
    return true;
}

static size_t _ringbuffer_available_space(ringbuffer_t* ringbuffer) {
    if (_ringbuffer_wp_reached_rp(ringbuffer))
        return 0;
    uint16_t wp = (uint32_t) ringbuffer->write_pos;
    uint16_t rp = (uint32_t) ringbuffer->read_pos;

    ringbuffer->available_space =
            rp > wp ? rp - wp : ringbuffer->capacity - (wp - rp);
    return ringbuffer->available_space;
}

static inline size_t _ringbuffer_space_left_till_end(ringbuffer_t* ringbuffer) {
    return _ringbuffer_data_last_addr(ringbuffer) - ringbuffer->write_pos;
}

static inline size_t _ringbuffer_read_left_till_end(ringbuffer_t* ringbuffer) {
    return _ringbuffer_data_last_addr(ringbuffer) - ringbuffer->read_pos;
}

static inline uint8_t* _ringbuffer_data_last_addr(ringbuffer_t* ringbuffer) {
    return ringbuffer->data_aloc_start + ringbuffer->capacity;
}

static inline void _ringbuffer_inc_write_pos(ringbuffer_t* ringbuffer,
        size_t by) {
    if (_ringbuffer_space_left_till_end(ringbuffer) > by) {
        ringbuffer->write_pos += by;
    } else {
        ringbuffer->write_pos = ringbuffer->data_aloc_start
                + (_ringbuffer_space_left_till_end(ringbuffer) - by);
        _ringbuffer_available_space(ringbuffer);
    }
}

static inline void _ringbuffer_inc_read_pos(ringbuffer_t* ringbuffer, size_t by) {
    if (_ringbuffer_read_left_till_end(ringbuffer) > by) {
        ringbuffer->read_pos += by;
    } else {
        ringbuffer->read_pos = ringbuffer->data_aloc_start
                + (_ringbuffer_read_left_till_end(ringbuffer) - by);
    }
}

static void _ringbuffer_write(ringbuffer_t* ringbuffer, uint8_t* data,
        size_t len) {
    size_t slte = _ringbuffer_space_left_till_end(ringbuffer);
    if (slte >= len) {
        memcpy(ringbuffer->write_pos, data, len);
    } else {
        memcpy(ringbuffer->write_pos, data, slte);
        memcpy(ringbuffer->data_aloc_start, data, len - slte);
    }
    _ringbuffer_inc_write_pos(ringbuffer, len);
}

static void _ringbuffer_read(ringbuffer_t* ringbuffer, uint8_t* data,
        size_t len) {
    size_t rlte = _ringbuffer_read_left_till_end(ringbuffer);
    if ((ringbuffer->write_pos > ringbuffer->read_pos) || (rlte > len)) {
        memcpy(data, ringbuffer->read_pos, len);
    } else {
        memcpy(data, ringbuffer->read_pos, rlte);
        memcpy(data, ringbuffer->data_aloc_start, len - rlte);
    }
    _ringbuffer_inc_read_pos(ringbuffer, len);
}
