/**	@brief implement all functions of Button.
	@date
		- 2021_07_26: Create.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>  
#include "ButtonControl.h"
#include "ATControl.h"

#define _DB_INIT_
#define _DB_BUTTON_
/////////////////////////////////////////////////////CONSTANTS/////////////////////////////////
#if _USE_ATCOMMAND_
	// Config GPIO
	#define BUTTON_WPS_GPIO_CFG		 		 "AT+CGFUNC=24,0"

	// Config direction (input)
	#define BUTTON_WPS_DIR_INPUT_CFG		 "AT+CGDRT=24,0"

	// Get value:
	#define BUTTON_WPS_GET_VALUE_CFG		 "AT+CGGETV=24"

#else
	#define SYSFS_BUTTON_EXPORT         	 "/sys/class/gpio/export"
	#define SYSFS_BUTTON_INPUT_VAL      	 "IN"

	// WPS Button (GPIO24):
	#define SYSFS_BUTTON_WPS_PIN         "24"
	#define SYSFS_BUTTON_WPS_DIR         "/sys/class/gpio/gpio40/direction"
	#define SYSFS_BUTTON_WPS_VAL         "/sys/class/gpio/gpio40/value"
#endif

////////////////////////////////////////VARIABLES//////////////////////////////////
static int bt_wps, bt_reset;
static char buff[64] = {0};
////////////////////////////////////////LOCAL FUNCTION/////////////////////////////
/*****************************************************************************
	* Function Name : button_config
	* Description   : 
	* Input         : None
	* Output        : None
	* Return        : 
*****************************************************************************/ 
#if _USE_ATCOMMAND_ == 0
static int button_config(void)
{
    int fd;
	
    // Config pin:
    fd = open(SYSFS_BUTTON_EXPORT, O_WRONLY);
    if (fd == -1) {
#ifdef _DB_INIT_
	printf("ERR: export pin open error.\n");
#endif
        return -1;
    }
    write(fd, SYSFS_BUTTON_WPS_PIN, sizeof(SYSFS_BUTTON_WPS_PIN));
    close(fd);

    // Config input for WPS pin:
    fd = open(SYSFS_BUTTON_WPS_DIR, O_WRONLY);
    if (fd == -1) {
#ifdef _DB_INIT_
	printf("ERR: Config WPS pin direction open error.\n");
#endif
        return -1;
    }
    write(fd, SYSFS_BUTTON_INPUT_VAL, sizeof(SYSFS_BUTTON_INPUT_VAL));
    close(fd);

	return 0;
}
#endif
////////////////////////////////////////PUBLIC FUNCTION/////////////////////////////
/*****************************************************************************
	* Function Name : button_init
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
int button_init()
{
#if _USE_ATCOMMAND_
    // Config GPIO function:
    sendATCmd(BUTTON_WPS_GPIO_CFG, "OK", buff, AT_DATA_MAX, AT_TIMEOUT_MAX);

    // Set GPIO directiron (input)
    sendATCmd(BUTTON_WPS_DIR_INPUT_CFG, "OK", buff, AT_DATA_MAX, AT_TIMEOUT_MAX);
	return 0;
#else
    button_config();

    bt_wps = open(SYSFS_BUTTON_WPS_VAL, O_RDONLY);
    if (bt_wps == -1) {
#ifdef _DB_INIT_
 	printf("Error\n");
#endif
        return -1;
    }
    return 0;
#endif
}

/*****************************************************************************
	* Function Name : button_read
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
int button_read(int bt_number)
{
#ifdef _DB_BUTTON_
	printf("\n\rbutton: bt_num=%d", bt_number);
#endif	

#if _USE_ATCOMMAND_
    int value;
    char *gpio, *state;

    // Send AT to get value:
    sendATCmd(BUTTON_WPS_GET_VALUE_CFG, "OK", buff, AT_DATA_MAX, AT_TIMEOUT_MAX);
    strtok(buff, ":");
    gpio = strtok(buff, ",");  
    state = strtok(buff, " ");
    value = atoi(state);
    return value;
#else 
    int ret, value;
    char rbuff;
    // Read status from WPS button:
    if (bt_number == BUTTON_WPS) {
	ret = read(bt_wps, rbuff, sizeof(rbuff));
	if (ret) {
		value = atoi(rbuff);
	} else {
		value = 0xFF;
	}
	return value;
    }
#endif
}

/*****************************************************************************
	* Function Name : button_manager
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
void button_manager(void)
{
}
