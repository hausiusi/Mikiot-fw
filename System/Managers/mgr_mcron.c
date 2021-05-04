/*
 * mgr_mcron.c
 *
 *  Created on: May 2, 2021
 *      Author: Zviad
 */
#include <string.h>
#include <inttypes.h>
#include "error.h"
#include "lib_linked_list.h"
#include "macrodefs.h"
#include "mgr_mcron.h"
#include "mgr_rtc.h"
#include "mw_timebase.h"
#include "defines.h"

#define HOUR_FORMAT 24

typedef struct {
    mcron_task_variant_t variant;
    char* description;
} mcron_task_variant_description_t;

/* @formatter:off */
static mcron_task_variant_description_t task_variant_descriptions[] = {
        {.variant = McronTaskVariantPeriodic, .description = "periodic"},
        {.variant = McronTaskVariantDelayed, .description = "delayed"},
        {.variant = McronTaskVariantPeriodicLimited, .description = "limited"},
#if (MCONF_RTC_ON == 1)
        {.variant = McronTaskVariantHourly, .description = "hourly"},
        {.variant = McronTaskVariantDaily, .description = "daily"},
        {.variant = McronTaskVariantWeekly, .description = "weekly"},
        {.variant = McronTaskVariantMonthly, .description = "monthly"},
        {.variant = McronTaskVariantYearly, .description = "yearly"},
        {.variant = McronTaskVariantOnceOnDate, .description = "onetime"}
#endif
};
/* @formatter:on */

static linked_list_t* mcron_tasks;
static uint32_t mcron_task_counter;

static bool_t _equals(void* a, void* b);
#if (MCONF_RTC_ON == 1)
static uint8_t _next_hour(rtc_time_t* time);
static uint8_t _next_day(rtc_date_t* date);
static uint8_t _next_weekday_date(rtc_date_t* date, uint8_t target_weekday);
static uint8_t _next_month(rtc_date_t* date);
static uint8_t _next_year(rtc_date_t* date);
static void _execute_mcron_function(mcron_item_t* mcron_item);

#endif

void mgr_mcron_init() {
    mcron_tasks = ll_init();
    mcron_tasks->equals = _equals;
}

void mgr_mcron_next_target_set(mcron_item_t* mcron_item) {
#if (MCONF_RTC_ON == 1)
    rtc_date_t* date = mgr_rtc_get_date();
    rtc_time_t* time = mgr_rtc_get_time();
#endif
    uint32_t ticks_now = mw_timebase_ticks_get();
    switch (mcron_item->variant) {
    case McronTaskVariantPeriodic:
    case McronTaskVariantDelayed:
    case McronTaskVariantPeriodicLimited:
        mcron_item->target.ticks.value = ticks_now
                + mcron_item->setup.ticks.value;
        break;
#if (MCONF_RTC_ON == 1)
    case McronTaskVariantHourly:
        mcron_item->target.exec_date.hour = _next_hour(time);
        break;
    case McronTaskVariantDaily:
        mcron_item->target.exec_date.day = _next_day(date);
        break;
    case McronTaskVariantWeekly:
        mcron_item->target.exec_date.day = _next_weekday_date(date,
                mcron_item->setup.exec_date.weekday);
        break;
    case McronTaskVariantMonthly:
        mcron_item->target.exec_date.month = _next_month(date);
        break;
    case McronTaskVariantYearly:
        mcron_item->target.exec_date.year = _next_year(date);
        break;
    case McronTaskVariantOnceOnDate:
        memcpy(&mcron_item->target, &mcron_item->setup, sizeof(mcron_time_t));
        break;
#endif
    }
}

void mgr_mcron_check_execute(ll_node_t* node) {
    mcron_item_t* mcron_item = (mcron_item_t*) node->item;
    uint32_t timebase_ticks = mw_timebase_ticks_get();
#if (MCONF_RTC_ON == 1)
    rtc_date_t* date = mgr_rtc_get_date();
    rtc_time_t* time = mgr_rtc_get_time();
#endif
    switch (mcron_item->variant) {
    case McronTaskVariantPeriodic:
        if (mcron_item->target.ticks.value <= timebase_ticks) {
            _execute_mcron_function(mcron_item);
            mgr_mcron_next_target_set(mcron_item);
        }
        break;
    case McronTaskVariantDelayed:
        if (mcron_item->target.ticks.value <= timebase_ticks) {
            _execute_mcron_function(mcron_item);
            mgr_mcron_remove(node);
        }
        break;
    case McronTaskVariantPeriodicLimited:
        if (mcron_item->target.ticks.value <= timebase_ticks) {
            _execute_mcron_function(mcron_item);
            if (!--mcron_item->target.ticks.exec_count) {
                mgr_mcron_remove(node);
            } else {
                mgr_mcron_next_target_set(mcron_item);
            }
        }
        break;
#if (MCONF_RTC_ON == 1)
    case McronTaskVariantHourly:
        if (mcron_item->target.exec_date.hour == time->Hours) {
            _execute_mcron_function(mcron_item);
            mgr_mcron_next_target_set(mcron_item);
        }
        break;
    case McronTaskVariantDaily:
        if (mcron_item->target.exec_date.day == date->Date) {
            _execute_mcron_function(mcron_item);
            mgr_mcron_next_target_set(mcron_item);
        }
        break;
    case McronTaskVariantWeekly:
        if ((mcron_item->target.exec_date.day == date->Date)
                && (mcron_item->target.exec_date.weekday
                        == mgr_rtc_get_date()->WeekDay)) {
            _execute_mcron_function(mcron_item);
            mgr_mcron_next_target_set(mcron_item);
        }
        break;
    case McronTaskVariantMonthly:
        if (mcron_item->target.exec_date.month == date->Month) {
            if (mcron_item->fn) {
                mcron_item->fn(mcron_item->args);
            }
            mgr_mcron_next_target_set(mcron_item);
        }
        break;
    case McronTaskVariantYearly:
        if (mcron_item->target.exec_date.year == date->Year) {
            if (mcron_item->fn) {
                mcron_item->fn(mcron_item->args);
            }
            mgr_mcron_next_target_set(mcron_item);
        }
        break;

    case McronTaskVariantOnceOnDate:
        if ((mcron_item->target.exec_date.second == time->Seconds)
                && (mcron_item->target.exec_date.minute == time->Minutes)
                && (mcron_item->target.exec_date.hour == time->Hours)
                && (mcron_item->target.exec_date.day == date->Date)
                && (mcron_item->target.exec_date.month == date->Month)
                && (mcron_item->target.exec_date.year == date->Year)) {
            if (mcron_item->fn) {
                mcron_item->fn(mcron_item->args);
            }
            mgr_mcron_remove(node);
        }
        break;
#endif
    }
}

bool_t mgr_mcron_add(mcron_item_t* mcron_item) {
    mcron_item->id = mcron_task_counter++;
    mgr_mcron_next_target_set(mcron_item);
    ll_node_t* node = ll_get_node(mcron_item, sizeof(mcron_item_t));
    if (!ll_add(mcron_tasks, node)) {
        debug_error("MCron periodic task add failed\n");
        return false;
    }
    debug_info("Mcron item (id=%i) added successfully\n", mcron_item->id);
    return true;
}

bool_t mgr_mcron_remove_by_item_id(mcron_item_t* item) {
    ll_node_t* node = ll_find(mcron_tasks, item);
    return mgr_mcron_remove(node);
}

bool_t mgr_mcron_remove(ll_node_t* node) {
    mcron_item_t* mcron_item = node->item;
    mgr_mcron_free_args(mcron_item);
    uint32_t id = mcron_item->id;
    if (!ll_remove(mcron_tasks, node)) {
        debug_error("Mcron item (id=%i) coudln't be removed\n", id);
        return false;
    }
    debug_info("Mcron item (id=%i) removed successfully\n", id);
    return true;
}

bool_t mgr_mcron_deep_copy_args(mcron_item_t* mcron_item, void* args,
        uint32_t len) {
    /* If args are NULL then we don't have to allocate anything*/
    if (args == NULL) {
        return true;
    }
    void* args_alloc = mcron_malloc(len);
    if (args_alloc == NULL) {
        error_report(26, HeapError);
        return false;
    }
    memcpy(args_alloc, args, len);
    mcron_item->args = args_alloc;
    mcron_item->free = mconf_free;
    debug_info("Successfully allocated %i bytes for mcron item arguments\n",
            len);
    return true;
}

void mgr_mcron_free_args(mcron_item_t* mcron_item) {
    if (mcron_item->free) {
        mcron_item->free(mcron_item->args);
        debug_info("Successfully freed space after mcron item arguments\n");
    }
}

void mgr_mcron_update() {
    foreach(ll_node_t, node, mcron_tasks)
    {
        mgr_mcron_check_execute(node);
    }
}

void mgr_mcron_tasks_list_print() {
    if (mcron_tasks->count == 0) {
        debug_p("There are no mcron tasks running currently\n");
        return;
    }
    debug_p("Count of mcron tasks running currently is %i:\n",
            mcron_tasks->count);
    uint32_t count = 0;
    foreach(ll_node_t, node, mcron_tasks)
    {
        mcron_item_t* item = (mcron_item_t*) node->item;
        debug_p("Task %i\n", ++count);
        mgr_mcron_item_info_print(item);
    }
}

void mgr_mcron_item_info_print(mcron_item_t* mcron_item) {
    debug_p(" Id:        %i\n", mcron_item->id);
    debug_p(" Variant:   %s\n",
            mgr_mcron_variant_to_description(mcron_item->variant));
    debug_p(" Time-base: ");
    uint32_t next = mcron_item->target.ticks.value - mw_timebase_ticks_get();
    switch (mcron_item->variant) {
    case McronTaskVariantPeriodic:
        debug_p("%i ms (periodically, ", mcron_item->setup.ticks.value);
        debug_p("next in %i ms)\n", next);
        break;
    case McronTaskVariantDelayed:
        debug_p("%i ms (once, ", mcron_item->setup.ticks.value);
        debug_p("next in %i ms)\n", next);
        break;
    case McronTaskVariantPeriodicLimited:
        debug_p("%i ms (several times, ", mcron_item->setup.ticks.value);
        debug_p("next in %i ms, ", next);
        debug_p("left amount %i)\n", mcron_item->target.ticks.exec_count);
        break;
    case McronTaskVariantHourly:
        debug_p("every hour. (next at %i:00)\n",
                mcron_item->setup.exec_date.hour);
        break;
    case McronTaskVariantDaily:
        debug_p("every day. (next on %i)\n", mcron_item->setup.exec_date.day);
        break;
    case McronTaskVariantWeekly:
        debug_p("every week. (next on %i)\n", mcron_item->setup.exec_date.day);
        break;
    case McronTaskVariantMonthly:
        debug_p("every month. (next in %i)\n",
                mcron_item->setup.exec_date.month);
        break;
    case McronTaskVariantYearly:
        debug_p("every year. (next in %i)\n", mcron_item->setup.exec_date.year);
        break;
    case McronTaskVariantOnceOnDate:
        debug_p("once on %i.%i.%i %i:%i:%i)\n", mcron_item->setup.exec_date.day,
                mcron_item->setup.exec_date.month,
                mcron_item->setup.exec_date.year,
                mcron_item->setup.exec_date.hour,
                mcron_item->setup.exec_date.minute,
                mcron_item->setup.exec_date.second);
        break;
    }
    debug_p(" Function:  at %p\n", mcron_item->fn);
}

void mgr_mcron_time_setup_variants_print() {
    for (int i = 0; i < array_count(task_variant_descriptions); i++) {
        debug_p("%s\n", task_variant_descriptions[i].description);
    }
}

char* mgr_mcron_variant_to_description(mcron_task_variant_t variant) {
    for (int i = 0; i < array_count(task_variant_descriptions); i++) {
        if (task_variant_descriptions[i].variant == variant) {
            return task_variant_descriptions[i].description;
        }
    }
    return "";
}

mcron_task_variant_t mgr_mcron_description_to_variant(char* description) {
    for (int i = 0; i < array_count(task_variant_descriptions); i++) {
        uint32_t len = strlen(task_variant_descriptions[i].description);
        if (!strncmp(task_variant_descriptions[i].description, description,
                len)) {
            return task_variant_descriptions[i].variant;
        }
    }
    return 0;
}

static bool_t _equals(void* a, void* b) {
    return ((mcron_item_t*) a)->id == ((mcron_item_t*) b)->id;
}

static void _execute_mcron_function(mcron_item_t* mcron_item) {
    if (mcron_item->fn) {
        mcron_item->fn(mcron_item->args);
    }
}

#if (MCONF_RTC_ON == 1)
static uint8_t _next_hour(rtc_time_t* time) {
    return (time->Hours + 1) >= MCONF_RTC_MAX_HOURS ? 0 : time->Hours + 1;
}

static uint8_t _next_day(rtc_date_t* date) {
    uint8_t max_day = mgr_rtc_get_max_days_in_month(bcd2bin_8b(date->Month),
            date->Year);
    return (date->Date + 1) > max_day ? 1 : date->Date + 1;
}

static uint8_t _next_weekday_date(rtc_date_t* date, uint8_t target_weekday) {
    int8_t days_left = (date->WeekDay - target_weekday) + 7;
    uint8_t max_day = mgr_rtc_get_max_days_in_month(bcd2bin_8b(date->Month),
            date->Year);
    return (date->Date + days_left) > max_day ?
            (date->Date + days_left) - max_day : date->Date + days_left;
}

static uint8_t _next_month(rtc_date_t* date) {
    return date->Month + 1 > 12 ? 1 : date->Month + 1;
}

static uint8_t _next_year(rtc_date_t* date) {
    return date->Year + 1 > 99 ? 0 : date->Year + 1;
}
#endif
