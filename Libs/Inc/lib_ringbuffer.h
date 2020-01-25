/*
 * lib_ringbuffer.h
 *
 *  Created on: Feb 21, 2019
 *      Author: Zviad
 */

#ifndef LIBS_LIB_RINGBUFFER_H_
#define LIBS_LIB_RINGBUFFER_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "defines.h"
#include "macrodefs.h"

/* @formatter:off */
enum {
	AllowsOverlap,
	WritePointerReachedReadPointer,
	Overlapped, // Shows the condition, if ringbuffer was overlapped after last read, after reading and crossing write position mark, this flag will be removed
} ringbuffer_status_t;
/* @formatter:on */

typedef struct _ringbuffer_t {
	uint8_t* data_aloc_start; // Where the data array starts
	uint8_t* read_pos; // Pointer to actual reading position. The last read was previous position
	uint8_t* write_pos; // Pointer to actual writing position
	uint8_t element_len; // Length of each element
	size_t capacity; // Capacity how many elements can the buffer hold
	size_t available_space; // Available free space (Difference between capacity and count).
	uint8_t status; // Shows ringbuffer status using the bits set according to @ringbuffer_status_t
	void (*ringbuffer_overlapped)(); // Function which will be called in case of overlap
} ringbuffer_t;

void lib_ringbuffer_init(ringbuffer_t* ringbuffer, uint8_t* data,
		uint8_t element_len, size_t len, bool_t allows_overlap,
		void* overlap_detected);

bool_t lib_ringbuffer_enq(ringbuffer_t* ringbuffer, uint8_t* data, size_t len);

bool_t lib_ringbuffer_deq(ringbuffer_t* ringbuffer, uint8_t* data, size_t len);

size_t lib_ringbuffer_bytes_to_read(ringbuffer_t* ringbuffer);

#ifdef __cplusplus
 }
#endif

#endif /* LIBS_LIB_RINGBUFFER_H_ */
