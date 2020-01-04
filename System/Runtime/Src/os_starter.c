/*
 * os_starter.c
 *
 *  Created on: Feb 20, 2019
 *      Author: Zviad
 */

#include <stddef.h>
#include <stdio.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "defines.h"
#include "stm32f4xx.h"
#include "os_starter.h"

// os startup process list
// probably will be a struct
// having handlers for each process when they will be created
// we will call only wrappers or our OS functions not directly cmsis just to avoid later porting problems

typedef struct startup_task {
	BaseType_t result;
	TaskFunction_t thread;
	char* name;
	configSTACK_DEPTH_TYPE stack_depth;
	void* const parameters;
	UBaseType_t priority;
	TaskHandle_t created_task;
}startup_task_t;

void thread_init();
void thread_test();

startup_task_t startup_tasks[] =
{
		{.result = 0, .thread = thread_init, .name = "Init", .stack_depth = configMINIMAL_STACK_SIZE, .parameters = NULL, .priority = 1},
		{.result = 0, .thread = thread_test, .name = "Test", .stack_depth = configMINIMAL_STACK_SIZE, .parameters = NULL, .priority = 1},
};


static void _startup_tasks_create() {
	for(int i = 0; i < COUNT(startup_tasks); i++)
	{
		startup_tasks[i].result =
		xTaskCreate(
		startup_tasks[i].thread,
		startup_tasks[i].name,
		startup_tasks[i].stack_depth,
		startup_tasks[i].parameters,
		startup_tasks[i].priority,
		&startup_tasks[i].created_task);
	}
}

void os_start() {
	_startup_tasks_create();
	vTaskStartScheduler();
}



