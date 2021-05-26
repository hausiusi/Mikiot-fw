/*
 * monitor.c
 *
 *  Created on: May 31, 2020
 *      Author: Zviad
 */

#include "debug.h"
#include "overall_state.h"
#include "mikiot_config.h"
#include "lib_linked_list.h"

static bool_t _equals(void* a, void* b);
static void _update_memory_states(tasks_statuses_t* task_statuses);
static void _analyze_memory_states();

linked_list_t* memory_states;

void overal_state_init() {
    memory_states = ll_init();
    memory_states->equals = _equals;
}

void overal_state_analyze(overall_t* overall_state) {
    /* TODO: This function must check overall state structure and
     * display the errors via different interfaces
     */
    if (!overall_state->base_time_ok) {
        debug_warn("Base time doesn't work\n");
    }
#if (MCONF_RTC_ON == 1)
    if (!overall_state->rtc_ok) {
        debug_warn("RTC seems stopped\n");
    }
#endif

    // Check task statuses and warn if possible stack overflow is discovered
    tasks_statuses_t* task_statuses = overall_state->task_statuses;
    if (task_statuses->is_available) {
        _update_memory_states(task_statuses);
        _analyze_memory_states();
    }
}

static bool_t _equals(void* a, void* b) {
    return ((task_memory_info_t*) a)->task_id =
            ((task_memory_info_t*) b)->task_id;
}

static uint32_t _get_memory_state_mask(uint16_t watermark) {
    uint32_t ret = 0;
    if (watermark >= WATERMARK_NORMAL_MIN) {
        ret = TaskMemStateNormal;
    } else if (watermark >= WATERMARK_LOW_MIN) {
        ret = TaskMemStateLow;
    } else if (watermark >= WATERMARK_EXTREME_LOW_MIN) {
        ret = TaskMemStateExtremeLow;
    } else {
        ret = TaskMemStateOverflow;
    }
    return ret;
}

static void _update_memory_states(tasks_statuses_t* task_statuses) {
    task_memory_info_t mem_info;
    ll_node_t* node;
    ll_node_t* next = memory_states->head;
    bool_t found = false;
    while (next) {
        for (uint8_t i = 0; i < task_statuses->current_count; i++) {
            if (task_statuses->current[i].xTaskNumber
                    == ((task_memory_info_t*) next->item)->task_id) {
                found = true;
                break;
            }
        }
        if (!found) {
            ll_remove(memory_states, next);
        }
        next = next->next;
        found = false;

    }
    for (uint8_t i = 0; i < task_statuses->current_count; i++) {
        mem_info.task_id = task_statuses->current[i].xTaskNumber;
        mem_info.memory_state = _get_memory_state_mask(
                task_statuses->current[i].usStackHighWaterMark);
        mem_info.memory_state_reported = 0;
        node = ll_find(memory_states, &mem_info);
        if (node == NULL) {
            node = ll_get_node(&mem_info, sizeof(task_memory_info_t));
            ll_add(memory_states, node);
        } else {
            ((task_memory_info_t*) node->item)->memory_state =
                    mem_info.memory_state;
        }
    }
}

static void _analyze_memory_states() {
    uint32_t mem_state = 0, reported = 0;
    foreach(ll_node_t, node, memory_states)
    {

        mem_state = ((task_memory_info_t*) node->item)->memory_state;
        reported = ((task_memory_info_t*) node->item)->memory_state_reported;
        if ((mem_state == TaskMemStateNormal)
                && (reported < TaskMemStateNormal)) {
            ((task_memory_info_t*) node->item)->memory_state_reported =
                    TaskMemStateNormal;
        } else if ((mem_state == TaskMemStateLow)
                && (reported < TaskMemStateLow)) {
            ((task_memory_info_t*) node->item)->memory_state_reported =
                    TaskMemStateLow;
            debug_warn("Task (id=%i) has left low memory (%i bytes)\n",
                    ((task_memory_info_t* )node->item)->task_id, mem_state * 4);
        } else if ((mem_state == TaskMemStateExtremeLow)
                && reported < TaskMemStateExtremeLow) {
            ((task_memory_info_t*) node->item)->memory_state_reported =
                    TaskMemStateExtremeLow;
            debug_warn("Task (id=%i) has left extreme low memory (%i bytes)\n",
                    ((task_memory_info_t* )node->item)->task_id, mem_state * 4);
        } else if ((mem_state == TaskMemStateOverflow)
                && reported < TaskMemStateOverflow) {
            ((task_memory_info_t*) node->item)->memory_state_reported =
                    TaskMemStateOverflow;
            debug_warn("Task (id=%i) has possible overflow (%i bytes)\n",
                    ((task_memory_info_t* )node->item)->task_id, mem_state * 4);
        }
    }
}
