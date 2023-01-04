#ifndef _SD_CONTROL_H_
#define _SD_CONTROL_H_
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "WiFiControl.h"
#include "EthControl.h"
#include "LteControl.h"

////////////////////////////////////////CONSTANTS//////////////////////////////////

////////////////////////////////////////TYPES/////////////////////////////////////

///////////////////////////////////////PROTOTYPES/////////////////////////////////
void sdcard_init();
uint8_t sd_general_config_store(void);
uint8_t sd_general_config_load(void);
uint8_t sd_wifi_config_store(void);
uint8_t sd_wifi_config_load(void);
uint8_t sd_ethernet_config_store(void);
uint8_t sd_ethernet_config_load(void);
uint8_t sd_lte4g_config_store(void);
uint8_t sd_lte4g_config_load(void);
uint8_t sd_gps_config_store(void);
uint8_t sd_gps_config_load(void);
uint8_t sd_log_store(void);
uint8_t sd_log_load(void);
#endif

