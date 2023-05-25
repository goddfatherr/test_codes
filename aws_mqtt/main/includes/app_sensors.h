#ifndef APP_SENSORS_H_
#define APP_SENSORS_H_

/**Sensor pin defines */
#define DHT11_PIN 17

void taskScanSensors(void *pvParameters);
void hw_init(void);



#endif