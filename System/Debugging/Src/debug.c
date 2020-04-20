/*
 * debug.c
 *
 *  Created on: Apr 20, 2020
 *      Author: Zviad
 */

#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "macrodefs.h"

typedef struct {
	char* fmt;
	void (*fn)(char** buff_ptr, va_list* ap);
} formats_t;

char dbg_buff[DEBUG_BUFFER_SIZE];

static inline void _lu(char** buff_ptr, va_list* ap) {
	uint32_t number;
	number = va_arg(*ap, uint32_t);
	char str[11];
	utoa(number, str, 10);
	char* str_ptr = str;
	while (*str_ptr) {
		**buff_ptr = *str_ptr++;
		(*buff_ptr)++;
	}
}

static inline void _l(char** buff_ptr, va_list* ap) {
	int32_t number;
	number = va_arg(*ap, int32_t);
	char str[11];
	itoa(number, str, 10);
	char* str_ptr = str;
	while (*str_ptr) {
		**buff_ptr = *str_ptr++;
		(*buff_ptr)++;
	}
}

static inline void _x(char** buff_ptr, va_list* ap) {
	uint32_t number;
	number = va_arg(*ap, uint32_t);
	char str[14];
	str[0] = '0';
	str[1] = 'x';
	utoa(number, str + 2, 16);
	char* str_ptr = str;
	while (*str_ptr) {
		**buff_ptr = *str_ptr++;
		(*buff_ptr)++;
	}
}

static inline void _p(char** buff_ptr, va_list* ap) {
	uint32_t* number;
	number = va_arg(*ap, uint32_t*);
	char str[16];
	str[0] = '<';
	str[1] = '0';
	str[2] = 'x';
	utoa((uint32_t) number, str + 3, 16);
	char* str_ptr = str;
	str[strlen(str)] = '>';
	while (*str_ptr) {
		**buff_ptr = *str_ptr++;
		(*buff_ptr)++;
	}
}

static inline void _s(char** buff_ptr, va_list* ap) {
	char* str_ptr;
	str_ptr = va_arg(*ap, char*);
	while (*str_ptr) {
		**buff_ptr = *str_ptr++;
		(*buff_ptr)++;
	}
}

static inline void _c(char** buff_ptr, va_list* ap) {
	int c;
	c = va_arg(*ap, int);
	**buff_ptr = (char) c;
	(*buff_ptr)++;
}

/* @formatter:off */
static formats_t formats[] = {
		{"lu", _lu},
		{"l", _l},
		{"u", _lu},
		{"i", _l},
		{"x", _x},
		{"p", _p},
		{"s", _s},
		{"c", _c},
};
/* @formatter:on */

void debugf(const char* str_buff, ...) {
	va_list ap;
	va_start(ap, str_buff);
	char* str_buff_ptr = (char*) str_buff;
	char* dbg_buff_ptr = dbg_buff;
	for (;;) {
		if (*str_buff_ptr == '\0') {
			*dbg_buff_ptr = *str_buff_ptr;
			break;
		}
		if (*str_buff_ptr == '%') {
			str_buff_ptr++;
			for (int i = 0; i < array_count(formats); i++) {
				int len = strlen((char*) formats[i].fmt);
				if (!strncmp(formats[i].fmt, str_buff_ptr, len)) {
					formats[i].fn(&dbg_buff_ptr, &ap);
					str_buff_ptr += len;
				}
			}
		} else {
			*dbg_buff_ptr++ = *str_buff_ptr++;
		}
	}
	va_end(ap);
	mw_uart1_dma_string_transmit(dbg_buff);
}

