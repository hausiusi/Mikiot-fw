/*
 * defines.h
 *
 *  Created on: Feb 18, 2019
 *      Author: Zviad
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define MAX_BUSY_BUF_TIMEOUT 0xFFFFFFFFUL
#define RTOS_TASKMGR_USE	 1
#define MIN_CPU_FREQ_MHZ	 16
#define MAX_CPU_FREQ_MHZ	 100

/**
 * \brief Get count of statically allocated array
 */
#define COUNT(x)            (sizeof(x) / sizeof((x)[0]))

typedef enum {
    MW_OK = 0x00U,
    MW_ERROR = 0x01U,
    MW_BUSY = 0x02U,
    MW_TIMEOUT = 0x03U,
    MW_EMPTY = 0x04U
} status_enum_t;

typedef enum {
    false = 0, true = !false
} bool_t;

inline void def_wait_cycles(unsigned long long cycles) {
    while (cycles--) {
        asm("NOP");
    }
}

#ifdef __cplusplus
 }
#endif

#endif /* DEFINES_H_ */
