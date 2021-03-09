/*
 * debug.c
 *
 *  Created on: Apr 20, 2020
 *      Author: Zviad
 */

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char str[14] = { 0 };
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

static inline void _f(char** buff_ptr, va_list* ap) {
    double d;
    d = va_arg(*ap, double);
    char padding[3] = { 0 };
    int whole = (int) d;
    double rest = d - whole;
    rest *= 1000;
    int frac = abs((int) rest);
    if (d < 0) {
        **buff_ptr = '-';
        (*buff_ptr)++;
    }
    whole = abs(whole);
    if (frac < 10) {
        padding[0] = '0';
        padding[1] = '0';
    } else if (frac < 100) {
        padding[0] = '0';
    }
    // Printing into the buffer
    char str[11];
    itoa(whole, str, 10);
    char* str_ptr = str;
    while (*str_ptr) {
        **buff_ptr = *str_ptr++;
        (*buff_ptr)++;
    }
    **buff_ptr = '.';
    (*buff_ptr)++;
    str_ptr = padding;
    while (*str_ptr) {
        **buff_ptr = *str_ptr++;
        (*buff_ptr)++;
    }
    utoa(frac, str, 10);
    str_ptr = str;
    while (*str_ptr) {
        **buff_ptr = *str_ptr++;
        (*buff_ptr)++;
    }
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
		{"f", _f},
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
    debug_put_log(dbg_buff);
}

void debug_set_level(int level) {
    debug_level = level;
}

int debug_get_level() {
    return debug_level;
}

