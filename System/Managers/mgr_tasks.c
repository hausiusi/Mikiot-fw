/*
 * mgr_tasks.c
 *
 *  Created on: Apr 25, 2020
 *      Author: Zviad
 */

#include <string.h>
#include "mgr_tasks.h"
#include "utils.h"

static tasks_statuses_t system_tasks_state;
static TaskStatus_t* tasks_statuses_current = NULL,
        * tasks_statuses_prev = NULL;

typedef struct {
    const TaskFunction_t task_function;
    char* const name;
} runnable_t;

void thread_init(void* args);
void thread_test(void* args);
void thread_cli(void* args);
void thread_taskmgr(void* args);
void thread_overal_state_monitor(void* args);
void thread_iomgr(void* args);
void thread_adc(void* args);
void thread_gprs(void* args);

/* @formatter:off */
static const runnable_t runnables[] = {
        { .task_function = thread_init, .name = "init" },
        { .task_function = thread_test, .name = "test" },
        { .task_function = thread_cli, .name = "cli" },
        { .task_function = thread_taskmgr, .name = "taskmgr" },
        { .task_function = thread_overal_state_monitor, .name = "overal" },
        { .task_function = thread_iomgr, .name = "iomgr" },
        { .task_function = thread_adc, .name = "adc" },
        { .task_function = thread_gprs, .name = "gprs" }
};
/* @formatter:on */

tasks_statuses_t* mgr_tasks_get_system_state() {
    uint32_t total_runtime;
    static uint32_t alloc_len_buffer = 0;
    static uint32_t alloc_len_last = 0;
    // First, make a copy of tasks statuses if it already exists, if not - skip.
    // Allocate memory only if it is needed
    if (alloc_len_last) {
        if (alloc_len_buffer != alloc_len_last || tasks_statuses_prev == NULL) {
            if (tasks_statuses_prev) {
                vPortFree(tasks_statuses_prev);
            }
            tasks_statuses_prev = pvPortMalloc(alloc_len_last);
            if (!tasks_statuses_prev) {
                debug_p(
                        "Unable to allocate %lu bytes of memory to copy last task statuses \n",
                        alloc_len_last);
                system_tasks_state.is_available = false;
                return &system_tasks_state;
            }
        }
        // If above operations are successful save this previously measured tasks status state
        // in the buffer to have a reference for next check
        memcpy(tasks_statuses_prev, tasks_statuses_current, alloc_len_last);
    }
    // Update last task allocation length which after this line is current tasks allocation length
    alloc_len_buffer = alloc_len_last;
    system_tasks_state.current_count = uxTaskGetNumberOfTasks();
    alloc_len_last = system_tasks_state.current_count * sizeof(TaskStatus_t);
    // Allocate memory only if it is needed
    // Buffer allocation length is the same as for tasks during previous check
    if (alloc_len_buffer != alloc_len_last || tasks_statuses_current == NULL) {
        if (tasks_statuses_current) {
            vPortFree(tasks_statuses_current);
        }
        tasks_statuses_current = pvPortMalloc(alloc_len_last);
        if (!tasks_statuses_current) {
            debug_p(
                    "Unable to allocate %lu bytes of memory for task statuses\n",
                    alloc_len_last);
            system_tasks_state.is_available = false;
            return &system_tasks_state;
        }

    }
    system_tasks_state.current_count = uxTaskGetSystemState(
            tasks_statuses_current, system_tasks_state.current_count,
            &total_runtime);
    system_tasks_state.previous = tasks_statuses_prev;
    system_tasks_state.current = tasks_statuses_current;
    system_tasks_state.is_available = tasks_statuses_prev
            && tasks_statuses_current;
    return &system_tasks_state;
}

bool_t mgr_tasks_kill_by_name(const char* task_name) {
    task_handle_t task_handle = mgr_tasks_get_task_handle_from_name(task_name);
    if (task_handle == NULL) {
        debug_error("Couldn't terminate a null handle\n");
        return false;
    }
    vTaskDelete(task_handle);
    return true;
}

task_handle_t mgr_tasks_get_task_handle_from_name(const char* task_name) {
    uint32_t task_name_len = strlen(task_name);
    if (task_name_len == 0) {
        debug_error("Task name must not be empty\n");
        return NULL;
    }
    for (int i = 0; i < system_tasks_state.current_count; i++) {
        if (!strncmp(task_name, system_tasks_state.current[i].pcTaskName,
                task_name_len)) {
            return system_tasks_state.current[i].xHandle;
        }
    }
    debug_error("Couldn't find task by name '%s'\n", task_name);
    return NULL;
}

bool_t mgr_tasks_create_by_name(char* task_name, char* name_to_assign,
        uint16_t stack_depth, uint32_t priority, void* const parameters) {
    task_function_t thread = mgr_tasks_get_thread_by_name(task_name);
    if (strnullempty(task_name) | strnullempty(name_to_assign)) {
        debug_error(
                "Task initialization or registration name shouldn't be empty\n");
        return false;
    }
    if (stack_depth < configMINIMAL_STACK_SIZE) {
        debug_error("Task stack depth shouldn't  be less than %i bytes\n",
                configMINIMAL_STACK_SIZE);
        return false;
    }
    if (priority > configMAX_PRIORITIES) {
        debug_error("Task priority shouldn't be more than %i\n",
                configMAX_PRIORITIES);
    }
    return xTaskCreate(thread, name_to_assign, stack_depth, parameters,
            priority,
            NULL) == pdPASS;
}

task_function_t mgr_tasks_get_thread_by_name(char* task_name) {
    uint32_t task_name_len = strlen(task_name);
    if (task_name_len == 0) {
        debug_error("Task name must not be empty\n");
        return NULL;
    }
    for (int i = 0; i < COUNT(runnables); i++) {
        if (!strncmp(task_name, runnables[i].name, task_name_len)) {
            return runnables[i].task_function;
        }
    }
    debug_error("Couldn't find task by name '%s'\n", task_name);
    return NULL;
}

void mgr_tasks_list_available_runnables() {
    debug_p("Available tasks to create:\n");
    for (int i = 0; i < COUNT(runnables); i++) {
        debug_p("  %s\n", runnables[i].name);
    }
}

tasks_statuses_t* mgr_tasks_get_last_measured() {
    return &system_tasks_state;
}
