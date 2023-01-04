/**	@brief implement all functions of LED.
	@date
		- 2021_07_24: Create.
*/
#include "LedControl.h"

//define _DB_INIT_
//efine _DB_LED
/////////////////////////////////////////////////////CONSTANTS/////////////////////////////////
#if _USE_ATCOMMAND_
	// Config GPIO:
	#define LED_STATUS_GPIO_CFG		 	 	"AT+CGFUNC=40,0"
	#define LED_4G_GPIO_CFG		 		 	"AT+CGFUNC=40,0"  // Note
	#define LED_WIFI_1_GPIO_CFG       		"AT+CGFUNC=25,0"
	#define LED_WIFI_2_GPIO_CFG         	"AT+CGFUNC=59,0"

	// Config direction (output)
	#define LED_STATUS_DIR_OUTPUT_CFG	 	"AT+CGDRT=40,1"
	#define LED_4G_DIR_OUTPUT_CFG		 	"AT+CGDRT=40,1" // Note
	#define LED_WIFI_1_DIR_OUTPUT_CFG 		"AT+CGDRT=25,1"
	#define LED_WIFI_2_DIR_OUTPUT_CFG	 	"AT+CGDRT=59,1"

	// Config value:
	#define LED_STATUS_VALUE_HIGH_CFG		"AT+CGSETV=40,1"
	#define LED_4G_VALUE_HIGH_CFG	 	 	"AT+CGSETV=40,1"  // Note
	#define LED_WIFI_1_VALUE_HIGH_CFG		"AT+CGSETV=25,1"
	#define LED_WIFI_2_VALUE_HIGH_CFG	 	"AT+CGSETV=59,1"
	#define LED_STATUS_VALUE_LOW_CFG		"AT+CGSETV=40,0"
	#define LED_4G_VALUE_LOW_CFG	 	 	"AT+CGSETV=40,0" // Note
	#define LED_WIFI_1_VALUE_LOW_CFG		"AT+CGSETV=25,0"
	#define LED_WIFI_2_VALUE_LOW_CFG	 	"AT+CGSETV=59,0"
#else 
	#define SYSFS_LED_EXPORT         "/sys/class/gpio/export"
	#define SYSFS_LED_OUTPUT_VAL     "OUT"

	// Led Staus: (GPIO40):
	#define SYSFS_LED_STATUS_PIN         "40"
	#define SYSFS_LED_STATUS_DIR         "/sys/class/gpio/gpio40/direction"
	#define SYSFS_LED_STATUS_VAL         "/sys/class/gpio/gpio40/value"

	// Led 4G/LTE (GPIO40):
	#define SYSFS_LED_4G_PIN        	 "40"
	#define SYSFS_LED_4G_DIR        	 "/sys/class/gpio/gpio40/direction"
	#define SYSFS_LED_4G_VAL        	 "/sys/class/gpio/gpio40/value"

	// Led WiFi 2.4G (GPIO25):
	#define SYSFS_LED_WIFI_1_PIN      	 "25"
	#define SYSFS_LED_WIFI_1_DIR       	 "/sys/class/gpio/gpio41/direction"
	#define SYSFS_LED_WIFI_1_VAL      	 "/sys/class/gpio/gpio41/value"

	// Led Wifi 5G (GPIO59):
	#define SYSFS_LED_WIFI_2_PIN       "59"
	#define SYSFS_LED_WIFI_2_DIR       "/sys/class/gpio/gpio20/direction"
	#define SYSFS_LED_WIFI_2_VAL       "/sys/class/gpio/gpio20/value"

	// High/Low value:
	#define SYSFS_GPIO_VAL_H          "1"
	#define SYSFS_GPIO_VAL_L          "0"
#endif
////////////////////////////////////////VARIABLES//////////////////////////////////
static int lteFd, wifi_1_Fd, wifi_2_Fd, statusFd;
static LED_FUNCTION_t led;
char buff[100];
////////////////////////////////////////LOCAL FUNCTION/////////////////////////////
/*****************************************************************************
	* Function Name : led_config
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 

////////////////////////////////////////PUBLIC FUNCTION/////////////////////////////
/*****************************************************************************
	* Function Name : tbox_gpio_init
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
int tbox_gpio_init(void)
{
    // Config GPIO function:
    sendATCmd((char*)LED_STATUS_GPIO_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
    sendATCmd((char*)LED_4G_GPIO_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
    sendATCmd((char*)LED_WIFI_1_GPIO_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
    sendATCmd((char*)LED_WIFI_2_GPIO_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);

    // Set GPIO directiron (output)
    sendATCmd((char*)LED_STATUS_DIR_OUTPUT_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
    sendATCmd((char*)LED_4G_DIR_OUTPUT_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
    sendATCmd((char*)LED_WIFI_1_DIR_OUTPUT_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
    sendATCmd((char*)LED_WIFI_2_DIR_OUTPUT_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
    
    // Set value = 0;
	led_on_off(LED_STATUS, LED_OFF);
	led_on_off(LED_4G, LED_OFF);
	led_on_off(LED_WIFI_1, LED_OFF);
	led_on_off(LED_WIFI_2, LED_OFF);

#ifdef _DB_INIT_
	printf("\n\rLed Init Success:");
#endif
    return 0;
}

/*****************************************************************************
	* Function Name : led_on_off
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
int led_on_off(int ledNum, int onAndOff)
{
    uint8_t ret;
	uint8_t i = 0;

#ifdef _DB_LED_
	printf("\n\rLed_on_off: led_num=%d, state=%d", ledNum, onAndOff);
#endif	

	for (i = 0; i < AT_RETRY_MAX; i++) {
		// Control led 4G/LTE:
		if (ledNum == LED_4G) {
			if (onAndOff == LED_OFF) {
				 ret = sendATCmd((char*)LED_4G_VALUE_LOW_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			} else {
				 ret = sendATCmd((char*)LED_4G_VALUE_HIGH_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			}
		}

		// Control Led status
		else if (ledNum == LED_STATUS) {
			if (onAndOff == LED_OFF) {
				 ret = sendATCmd((char*)LED_STATUS_VALUE_LOW_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			} else {
				 ret = sendATCmd((char*)LED_STATUS_VALUE_HIGH_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			}
		}
		
		// Control Led WiFi 1
		else if (ledNum == LED_WIFI_1) {
			if (onAndOff == LED_OFF) {
				 ret = sendATCmd((char*)LED_WIFI_1_VALUE_LOW_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			} else {
				 ret = sendATCmd((char*)LED_WIFI_1_VALUE_HIGH_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			}
		}
		
		// Control Led_Wifi 2:
		else if (ledNum == LED_WIFI_2) {
			if (onAndOff == LED_OFF) {
				 ret = sendATCmd((char*)LED_WIFI_2_VALUE_LOW_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			} else {
				ret = sendATCmd((char*)LED_WIFI_2_VALUE_HIGH_CFG, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
			}
		}
		if (ret > 0) {
			break;
		}
	}

    return ret ;
}

/*****************************************************************************
	* Function Name : led_blink
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 

*****************************************************************************/
void led_blink(uint8_t ledNum)
{
#ifndef _DB_LED_
	printf("\n\rLed_blink");
#endif	
	if (ledNum == LED_4G) {
		if (led.led_4G == LED_OFF) {
			led_on_off(LED_4G, LED_ON);
			led.led_4G = LED_ON;
		} else {
			led_on_off(LED_4G, LED_OFF);
			led.led_4G = LED_OFF;
		}
	} else if (ledNum == LED_STATUS) {
		if (led.led_status == LED_OFF) {
			led_on_off(LED_STATUS, LED_ON);
			led.led_status = LED_ON;
		} else {
			led_on_off(LED_STATUS, LED_OFF);
			led.led_status = LED_OFF;
		}
	} else if (ledNum == LED_WIFI_1) {
		if (led.led_wifi_1 == LED_OFF) {
			led_on_off(LED_WIFI_1, LED_ON);
			led.led_wifi_1 = LED_ON;
		} else {
			led_on_off(LED_WIFI_1, LED_OFF);
			led.led_wifi_1 = LED_OFF;
		}
	} else if (ledNum == LED_WIFI_2) {
		if (led.led_wifi_2 == LED_OFF) {
			led_on_off(LED_WIFI_2, LED_ON);
			led.led_wifi_2 = LED_ON;
		} else {
			led_on_off(LED_WIFI_2, LED_OFF);
			led.led_wifi_2 = LED_OFF;
		}
	}
		
}

