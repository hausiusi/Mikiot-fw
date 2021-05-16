/*
 * os_loader.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Zviad
 */

#ifndef OS_STARTER_H_
#define OS_STARTER_H_

#include "FreeRTOS.h"
#include "task.h"

/* @formatter:off */

typedef enum task_index {
    InitHandler,
    TestHandler,
    CliHandler,
    TaskMgrHandler,
    OverallStateMonitorHandler,
    IoMgrHandler,
    AdcHandler,
    GprsHandler,
    McronHandler,
    AppLinkHandler,
}task_index_enum_t;

/* @formatter:on */

void os_start();

TaskHandle_t os_get_task_handler(task_index_enum_t task_index);

#endif /* OS_STARTER_H_ */
