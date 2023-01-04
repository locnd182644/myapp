#ifndef _LTE4G_CONTROL_H_
#define _LTE4G_CONTROL_H_

#include "simcom_common.h"
#include "ATControl.h"
#include "UIMControl.h"
#include "SMSControl.h"
#include "NASControl.h"

///////////////////////////////////////CONSTANTS///////////////////////////////////////

///////////////////////////////////////TYPES///////////////////////////////////////
typedef struct {
	// Simcard:
	SimCard_Status_type sim_status;
	char sim_iccid[32];
	char sim_imsi[32];
	char sim_imei[32];
	char phone_number[16];
	uint8_t rssi_4g;

	char system_mode[16];
	char operation_mode[16];
	char mobile_country_code[8];
	char mobile_network_code[8];
	char location_area_code[16];
	char service_cell_id[16];
	char physical_cell_id[16];
	char afrcn[16];	
	char freq_band[16];

	char current_4g_data[16];	
	uint8_t network_mode;
	
} LTE4G_FUNCTION_t;
LTE4G_FUNCTION_t g_lte4g_info;
/////////////////////////////////////PROTOTYPES///////////////////////////////////////
void lte4g_init(void);
int8_t lte4g_simcard_status_get(void);
int8_t lte4g_simcard_iccid_get(void);
int8_t lte4g_simcard_imsi_get(void);
int8_t lte4g_simcard_imei_get(void);
int8_t lte4g_sms_send(char *phoneNumber, uint8_t *content);
void lte4g_sms_received(uint8_t *data);
void lte4g_network_received(uint8_t *data);
uint8_t lte4g_csq_get(void);
uint8_t lte4g_network_mode_get(void);
uint8_t lte4g_network_mode_set(uint8_t network_mode);
uint8_t lte4g_UE_info_get(void);
void lte4g_read_all(void);
void lte4g_manager(void);
#endif

