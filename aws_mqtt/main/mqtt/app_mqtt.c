#include "app_mqtt.h"
#include "common.h"
#include "mqtt_demo_mutual_auth.h"


int publishToTopic( MQTTContext_t * pMqttContext)
{
    int returnStatus = EXIT_SUCCESS;
    MQTTStatus_t mqttStatus = MQTTSuccess;
    uint8_t publishIndex = MAX_OUTGOING_PUBLISHES;

    //uint16_t topicWidth =  ((uint16_t)(sizeof(TEMP_TOPIC) - 1 ));
    //uint16_t payloadWidth =  ((uint16_t)(sizeof(payload) - 1 ));

    // Generate the payload string
    //char payload[200];  
    //snprintf(payload, sizeof(payload), PAYLOAD_FORMAT, temperature, humidity, light);

    // Determine the required size for the payload
    size_t payloadSize = snprintf(NULL, 0, PAYLOAD_FORMAT, temperature, humidity, light) + 1;
    char* payload = malloc(payloadSize);
    if (payload == NULL) {

        // Failed to allocate memory, handle error
        // ...
    }
    snprintf(payload, payloadSize, PAYLOAD_FORMAT, temperature, humidity, light);

    assert( pMqttContext != NULL );

    returnStatus = getNextFreeIndexForOutgoingPublishes( &publishIndex );

    if( returnStatus == EXIT_FAILURE )
    {
        LogError( ( "Unable to find a free spot for outgoing PUBLISH message.\n\n" ) );
    }
    else
    {
        outgoingPublishPackets[ publishIndex ].pubInfo.qos = MQTTQoS0;
        outgoingPublishPackets[ publishIndex ].pubInfo.pTopicName = TOPIC;
        outgoingPublishPackets[ publishIndex ].pubInfo.topicNameLength =  TOPIC_SIZE;
        outgoingPublishPackets[ publishIndex ].pubInfo.pPayload = payload;
        outgoingPublishPackets[ publishIndex ].pubInfo.payloadLength = ((uint16_t)(payloadSize ));

        outgoingPublishPackets[ publishIndex ].packetId = MQTT_GetPacketId( pMqttContext );

        mqttStatus = MQTT_Publish( pMqttContext,
                                   &outgoingPublishPackets[ publishIndex ].pubInfo,
                                   outgoingPublishPackets[ publishIndex ].packetId );

        if( mqttStatus != MQTTSuccess )
        {
            LogError( ( "Failed to send PUBLISH packet to broker with error = %s.",
                        MQTT_Status_strerror( mqttStatus ) ) );
            cleanupOutgoingPublishAt( publishIndex );
            returnStatus = EXIT_FAILURE;
        }
        else
        {
            LogInfo( ( "PUBLISH sent for topic %.*s to broker with packet ID %u.\n\n",
                       TOPIC_SIZE,
                       TOPIC,
                       outgoingPublishPackets[ publishIndex ].packetId ) );
        }

    }

    free(payload);
    return returnStatus;
}


void taskPublishAws(void *pvParameters)
{
    int returnStatus = EXIT_SUCCESS;
    MQTTContext_t mqttContext = { 0 };
    NetworkContext_t xNetworkContext = { 0 };
    bool clientSessionPresent = false, brokerSessionPresent = false;
    struct timespec tp;

    //char buffer[5]; 

    ( void ) clock_gettime( CLOCK_REALTIME, &tp );
    srand( tp.tv_nsec );

    returnStatus = initializeMqtt( &mqttContext, &xNetworkContext );
    returnStatus = connectToServerWithBackoffRetries( &xNetworkContext, &mqttContext, &clientSessionPresent, &brokerSessionPresent );

        if( returnStatus == EXIT_FAILURE )
        {
            LogError( ( "Failed to connect to MQTT broker %.*s.",
                        AWS_IOT_ENDPOINT_LENGTH,
                        AWS_IOT_ENDPOINT ) );
        }
        else
        {
            clientSessionPresent = true;
            if( brokerSessionPresent == true )
            {
                LogInfo( ( "An MQTT session with broker is re-established. "
                            "Resending unacked publishes." ) );

                returnStatus = handlePublishResend( &mqttContext );
            }
            else
            {
                LogInfo( ( "A clean MQTT connection is established."
                            " Cleaning up all the stored outgoing publishes.\n\n" ) );
                            
                
            }

    if( returnStatus == EXIT_SUCCESS )
    {
        while(1)
        {
            //Wait for semaphore to be available 
             xSemaphoreTake(taskSyncSemaphore, portMAX_DELAY);

            /* If TLS session is established, execute Publish . */

                returnStatus = publishToTopic( &mqttContext);
                if( returnStatus == EXIT_SUCCESS )
            {
                LogInfo( ( "Sensor Data Logged to Cloud Successfully.\n" ) );
            }
                else
            {
                LogInfo( ( "Error Logging Data.\n" ) );
            }

                 cleanupOutgoingPublishes();

                /* End TLS session, then close TCP connection. */
                //cleanupESPSecureMgrCerts( &xNetworkContext );
                //( void ) xTlsDisconnect( &xNetworkContext );
                 vTaskDelay(pdMS_TO_TICKS(1000)); 
            }

           
        }
        
    }

}

