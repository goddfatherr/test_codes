#include "app_mqtt.h"
#include "common.h"
#include "mqtt_demo_mutual_auth.h"


int publishToTopic( MQTTContext_t * pMqttContext, char *topic, char *payload )
{
    int returnStatus = EXIT_SUCCESS;
    MQTTStatus_t mqttStatus = MQTTSuccess;
    uint8_t publishIndex = MAX_OUTGOING_PUBLISHES;

    uint16_t topicWidth =  ((uint16_t)(sizeof(TEMP_TOPIC) - 1 ));
    uint16_t payloadWidth =  ((uint16_t)(sizeof(payload) - 1 ));

    assert( pMqttContext != NULL );

    returnStatus = getNextFreeIndexForOutgoingPublishes( &publishIndex );

    if( returnStatus == EXIT_FAILURE )
    {
        LogError( ( "Unable to find a free spot for outgoing PUBLISH message.\n\n" ) );
    }
    else
    {
        outgoingPublishPackets[ publishIndex ].pubInfo.qos = MQTTQoS1;
        outgoingPublishPackets[ publishIndex ].pubInfo.pTopicName = TEMP_TOPIC;
        outgoingPublishPackets[ publishIndex ].pubInfo.topicNameLength =  topicWidth;
        outgoingPublishPackets[ publishIndex ].pubInfo.pPayload = payload;
        outgoingPublishPackets[ publishIndex ].pubInfo.payloadLength = payloadWidth;

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
                       topicWidth,
                       topic,
                       outgoingPublishPackets[ publishIndex ].packetId ) );
        }
    }

    return returnStatus;
}


void taskPublishAws(void *pvParameters)
{
    int returnStatus = EXIT_SUCCESS;
    MQTTContext_t mqttContext = { 0 };
    NetworkContext_t xNetworkContext = { 0 };
    bool clientSessionPresent = false, brokerSessionPresent = false;
    struct timespec tp;

    char buffer[5]; 

    ( void ) clock_gettime( CLOCK_REALTIME, &tp );
    srand( tp.tv_nsec );

    returnStatus = initializeMqtt( &mqttContext, &xNetworkContext );

    if( returnStatus == EXIT_SUCCESS )
    {
        while(1)
        {
            //Wait for semaphore to be available 
             xSemaphoreTake(taskSyncSemaphore, portMAX_DELAY);

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
                            
                cleanupOutgoingPublishes();
            }


            /* If TLS session is established, execute Publish . */

            /*Log temperature */
                itoa(temperature, buffer, 10);
                returnStatus = publishToTopic( &mqttContext, TEMP_TOPIC, buffer);
                if( returnStatus == EXIT_SUCCESS )
            {
                LogInfo( ( "Temperature Data Logged Successfully.\n" ) );
            }
                else
            {
                LogInfo( ( "Error Logging Temperature Data.\n" ) );
            }

            /*Log Humidity */
                itoa(humidity, buffer, 10);
                returnStatus = publishToTopic( &mqttContext, HUMD_TOPIC, buffer);
                if( returnStatus == EXIT_SUCCESS )
            {
                LogInfo( ( "Humidity Data Logged Successfully.\n" ) );
            }
                else
            {
                LogInfo( ( "Error Logging Humidity Data.\n" ) );
            }

            /*Log Light intensity */
                itoa(light, buffer, 10);
                returnStatus = publishToTopic( &mqttContext, LIGHT_TOPIC, buffer);
                if( returnStatus == EXIT_SUCCESS )
            {
                LogInfo( ( "Light Intensity Data Logged Successfully.\n" ) );
            }
                else
            {
                LogInfo( ( "Error Logging Light Intensity Data.\n" ) );
            }


                /* End TLS session, then close TCP connection. */
                cleanupESPSecureMgrCerts( &xNetworkContext );
                ( void ) xTlsDisconnect( &xNetworkContext );
            }

            vTaskDelay(pdMS_TO_TICKS(1000)); 
        }
        
    }

}

