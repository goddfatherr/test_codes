#ifndef APP_MQTT_H_
#define APP_MQTT_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* POSIX includes. */
#include <unistd.h>

/* Include Demo Config as the first non-system header. */
#include "demo_config.h"

/* MQTT API headers. */
#include "core_mqtt.h"
#include "core_mqtt_state.h"

/* OpenSSL sockets transport implementation. */
#include "network_transport.h"

/*Include backoff algorithm header for retry logic.*/
#include "backoff_algorithm.h"

/* Clock for timer. */
#include "clock.h"

#include "common.h"


#define PAYLOAD_FORMAT \
    "{\n" \
    "    \"temperature\": %u,\n" \
    "    \"humidity\": %u,\n" \
    "    \"light_intensity\": %u\n" \
    "}"


#define TOPIC           "home/sensors"   
#define TOPIC_SIZE      ((uint16_t)(sizeof(TOPIC) - 1 ))

/**
 * @brief Sends an MQTT PUBLISH to #MQTT_EXAMPLE_TOPIC defined at
 * the top of the file.
 *
 * @param[in] pMqttContext MQTT context pointer.
 *
 * @return EXIT_SUCCESS if PUBLISH was successfully sent;
 * EXIT_FAILURE otherwise.
 */
int publishToTopic( MQTTContext_t * pMqttContext);

void taskPublishAws(void *pvParameters);

#endif