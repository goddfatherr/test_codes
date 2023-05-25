#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "esp_log.h"

#include "app_mqtt.h"
#include "app_sensors.h"

//#include "dht.h"

//int aws_iot_demo_main( int argc, char ** argv );

static const char *TAG = "MQTT_EXAMPLE";

SemaphoreHandle_t taskSyncSemaphore; 

void app_main()
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %"PRIu32" bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    
    /* Initialize NVS partition */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS partition was truncated
         * and needs to be erased */
        ESP_ERROR_CHECK(nvs_flash_erase());

        /* Retry nvs_flash_init */
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

     /*Initialize the hardware*/
    hw_init();

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());

    //aws_iot_demo_main(0,NULL);


    // Create the semaphore
    taskSyncSemaphore = xSemaphoreCreateBinary();
    if (taskSyncSemaphore != NULL)
    {
        xTaskCreate(taskScanSensors, "scanSensors", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(taskPublishAws, "PublishAws", 5*configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    }
    else
    {
        // Semaphore creation failed, handle the error
        //Error handling will be done later 
    }
}
