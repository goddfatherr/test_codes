#include <stdint.h>
#include "app_sensors.h"
#include "common.h"


/* Global Variables to record sensor measurements*/
uint16_t temperature = 0;
uint16_t humidity    = 0;
uint16_t light       = 0; 

void hw_init(void)
{
    temperature = 0;
    humidity    = 0;
    light       = 0;

}

static void read_dht11(void)
{
    //read dht11 and set temperature and humidity
    temperature = 23;
    humidity = 35;
}

static void read_ldr(void)
{
    light = 34;
}

void taskScanSensors(void *pvParameters)
{
   while(1)
   {
     read_dht11();
     read_ldr();
     xSemaphoreGive(taskSyncSemaphore);
   }
}