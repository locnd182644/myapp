#ifndef _USIM_CONTROL_H_
#define _USIM_CONTROL_H_
#include "qmi_client.h"
#include "user_identity_module_v01.h"
#include "simcom_common.h"
//////////////////////////////////CONSTANTS//////////////////////////////
#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/////////////////////////////////TYPES/////////////////////////////////////
typedef unsigned char boolean;
typedef unsigned char uint8;

typedef struct{
	uim_card_state_enum_v01  card_status;
	uim_app_type_enum_v01    app_type;
	uim_app_state_enum_v01   app_state;
	uim_pin_info_type_v01    pin;
}SimCard_Status_type;

///////////////////////////////////PROTOTYPES////////////////////////////////
int uim_init();
void uim_deinit();
int getSimCardStatus(SimCard_Status_type *simStatus);	
int get_iccid(char *piccid);	
int get_imsi(char *imsi);
int simcom_uim_power_down();
int simcom_uim_power_up();

#endif


