#ifndef _ETH_CONTROL_H_
#define _ETH_CONTROL_H_

#include "app_common.h"

////////////////////////////////////////CONSTANTS//////////////////////////////////
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define DRIVER_DISABLE				0
#define DRIVER_ENABLE				1

#define MAC_DISABLE					0
#define MAC_ENABLE					1

#define BRINGDOWN_ENABLE			0
#define BRINGUP_ENABLE				1

#define DRIVER_TYPE_NONE			0
#define DRIVER_TYPE_BCM				1
#define DRIVER_TYPE_AT803X			2

#define ETHERNET_IP_ADDR_DEFAULT      "192.168.1.1"
#define ETHERNET_MAC_ADDR_DEFAULT     "00:24:1D:AF:3D:F2"
////////////////////////////////////////TYPES/////////////////////////////////////
typedef struct {
	uint8_t driver_en;
	uint8_t driver_type;
	uint8_t bring_updown_en;
	uint8_t ip_static_en;
	char ip_addr[24];
	char netmask[24];
} ETHERNET_FUNCTION_t;
ETHERNET_FUNCTION_t g_ethernet_info;
///////////////////////////////////////PROTOTYPES/////////////////////////////////
void ethernet_init(void);
uint8_t ethernet_mac_address_set(void);
uint8_t ethernet_ip_addr_set(char *ip_addr, char *netmask);
uint8_t ethernet_bringup_set(void);
uint8_t ethernet_bringdown_set(void); 
uint8_t ethernet_reset_factory(void);
void ethernet_manager(void);
#endif

