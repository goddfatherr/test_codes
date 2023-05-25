#ifndef COMMON_H_
#define COMMON_H_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <stdint.h>

/*Semaphore to synchronise tasks */
extern SemaphoreHandle_t taskSyncSemaphore; 

/* Global Variables to record sensor measurements*/
extern uint16_t temperature;
extern uint16_t humidity;
extern uint16_t light; 

#endif