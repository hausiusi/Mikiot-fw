/*
 * t_mcron.c
 *
 *  Created on: May 1, 2021
 *      Author: Zviad
 */

#include "FreeRTOS.h"
#include "task.h"

#include "lib_linked_list.h"
#include "mgr_mcron.h"
#include "iterator.h"
#include "debug.h"

extern void thread_mcron(void* args) {
    vTaskDelay(100);
    mgr_mcron_init();
    for (;;) {
        mgr_mcron_update();
        vTaskDelay(1);
    }
}
