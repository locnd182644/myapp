/**	@brief implement all functions of MQTT protocol.
	@date
		- 2021_07_23: Create.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>
#include "MqttControl.h"
#include "ATControl.h"

///////////////////////////////////////CONSTANTS////////////////////////////////////////////
#define HOST "test.mosquitto.org"
#define PORT  1883
#define PORTS 8883
#define KEEP_ALIVE 60
#define MSG_MAX_SIZE  512

#define DEVICE_ID  			"device_1"
#define TOPIC_DEVICE_INFO		"device_info"
#define TOPIC_WARNING			"warning"
#define	TOPIC_NETWORK_INFO		"network_info"
#define TOPIC_CONFIG			"config"

#define CONNECTION_TIME			60   // 60 s

/////////////////////////////////////VARIABLES/////////////////////////////////////////////
static uint32_t g_connection_time = 0;

/////////////////////////////////////CALLBACK///////////////////////////////////////////////
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void my_connect_callback(struct mosquitto *mosq, void *userdata, int result);
/*****************************************************************************
	* Function Name : 
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if(message->payloadlen){
        printf("%s %s", message->topic, (char *)message->payload);
    } else {
        printf("%s (null)\n", message->topic);
    }
    fflush(stdout);
	
	// Reset param:
	g_connection_time = 0;
}

/*****************************************************************************
	* Function Name : 
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    if(!result){
        /* Subscribe to broker information topics on successful connect. */
        mosquitto_subscribe(mosq, NULL, "TOPIC_CONFIG", 2);

    }else{
        fprintf(stderr, "Connect failed\n");
    }
}

////////////////////////////////////PUBLIC FUNCTION////////////////////////////////////////////////
/*****************************************************************************
	* Function Name : mqtt_init
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
void mqtt_init(void)
{
	// Init mosquitto lib:
    //mosquitto_lib_init();
	
	// connect and subcribe:
	mqtt_sub();
}

/*****************************************************************************
	* Function Name : mqtt_sub
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
int mqtt_sub(void)
{
    struct mosquitto *mosq = NULL;
	int ret;
	
    // Create new session:
    //mosq = mosquitto_new(DEVICE_ID, FALSE, NULL);
   // if(!mosq) {
        //mosquitto_lib_cleanup();
   // }	

	// Set callback functions:
	//mosquitto_connect_callback_set(mosq, my_connect_callback);
   // mosquitto_message_callback_set(mosq, my_message_callback);
	
    // Connect to Server:
    //if(mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE)){
     //   fprintf(stderr, "Unable to connect.\n");
     //   return 1;
   // }

	// Free memory:
    //mosquitto_destroy(mosq);
    //mosquitto_lib_cleanup();
	//return 0;
}

/*****************************************************************************
	* Function Name : mqtt_pub
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
int mqtt_pub(char *buff, int len, char *topic)
{
    //struct mosquitto *mosq = NULL;
//	int ret = 0;
	
    // Create new session:
    //mosq = mosquitto_new(DEVICE_ID, FALSE, NULL);
    //if(!mosq) {
        //mosquitto_lib_cleanup();
   // }
	
    // Connect to Server:
    //if(mosquitto_connect(mosq, HOST, PORT, KEEP_ALIVE)){
    //}
	
	// Publish data:
	//ret = mosquitto_publish(mosq, NULL, topic, len + 1, buff, 0, 0);
	//memset(buff, 0, len); 
		
	// Free memory:
    //mosquitto_destroy(mosq);
    //mosquitto_lib_cleanup();
}

/*****************************************************************************
	* Function Name : mqtt_manager
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
void mqtt_manager(void) 
{
}
