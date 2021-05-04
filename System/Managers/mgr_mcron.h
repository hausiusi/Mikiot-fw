/*
 * mgr_mcron.h
 *
 *  Created on: May 2, 2021
 *      Author: Zviad
 */

#ifndef MANAGERS_MGR_MCRON_H_
#define MANAGERS_MGR_MCRON_H_

#include <inttypes.h>
#include "lib_linked_list.h"
#include "mikiot_config.h"

#define mcron_malloc                mconf_malloc
#define mcron_free                  mconf_free

typedef enum {
    McronTaskVariantPeriodic = 1,
    McronTaskVariantDelayed = 2,
    McronTaskVariantPeriodicLimited = 3,
#if (MCONF_RTC_ON == 1)
    McronTaskVariantHourly = 4,
    McronTaskVariantDaily = 5,
    McronTaskVariantWeekly = 6,
    McronTaskVariantMonthly = 7,
    McronTaskVariantYearly = 8,
    McronTaskVariantOnceOnDate = 9
#endif
} mcron_task_variant_t;

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t weekday;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} mcron_date_t;

typedef struct {
    uint32_t value;
    uint32_t exec_count;
} mcron_task_ticks_t;

typedef union {
    mcron_date_t exec_date;
    mcron_task_ticks_t ticks;
} mcron_time_t;

typedef struct {
    uint32_t id;
    mcron_task_variant_t variant;
    mcron_time_t setup;
    void (*fn)(void* args);
    void* args;
    void (*free)(void* args);
    mcron_time_t target;
} mcron_item_t;

void mgr_mcron_init();

bool_t mgr_mcron_add(mcron_item_t* mcron_item);

bool_t mgr_mcron_remove(ll_node_t* node);

bool_t mgr_mcron_remove_by_item_id(mcron_item_t* item);

bool_t mgr_mcron_deep_copy_args(mcron_item_t* mcron_item, void* args,
        uint32_t len);

void mgr_mcron_free_args(mcron_item_t* mcron_item);

void mgr_mcron_update();

void mgr_mcron_tasks_list_print();

void mgr_mcron_item_info_print(mcron_item_t* mcron_item);

void mgr_mcron_time_setup_variants_print();

char* mgr_mcron_variant_to_description(mcron_task_variant_t variant);

mcron_task_variant_t mgr_mcron_description_to_variant(char* description);

#endif /* MANAGERS_MGR_MCRON_H_ */
