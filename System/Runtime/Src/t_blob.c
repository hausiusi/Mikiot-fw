/*
 * t_blob.c
 *
 *  Created on: Apr 28, 2021
 *      Author: Zviad
 */
#include "bp_player.h"
#include "FreeRTOS.h"
#include "task.h"

extern void thread_blob(void* args) {
    bp_player_play((uint8_t*) args);
    vTaskDelete(NULL);
}
