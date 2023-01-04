#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_

#include <fcntl.h>  //define O_WRONLY and O_RDONLY
#include "app_common.h"
#include "ATControl.h"

////////////////////////////////CONSTANTS/////////////////////////////
#define LED_STATUS				0
#define LED_4G					1
#define LED_WIFI_1				2
#define LED_WIFI_2				3

#define LED_OFF					0
#define LED_ON					1
////////////////////////////////TYPES//////////////////////////////////////
typedef struct {
    uint8_t led_4G;
    uint8_t led_status;
    uint8_t led_wifi_1;
    uint8_t led_wifi_2;
} LED_FUNCTION_t;
///////////////////////////////PROTOTYPES//////////////////////////////////////
extern int tbox_gpio_init();
extern int led_on_off(int ledNum, int onAndOff);
extern int led_control_timer1ms();
void led_blink(uint8_t ledNum);
#endif // _LED_CONTROL_H_

