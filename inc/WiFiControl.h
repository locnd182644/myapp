#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#include "app_common.h"
#include "ATControl.h"
//#include "qcmap_client_def.h"

///////////////////////////////////////CONSTANTS///////////////////////////////////////
// Wifi count:
#define WIFI_COUNT_MAX							3
#define WIFI_COUNT_MIN							1

// MAC:	
#define MAC_ADDR_MAX							16
#define MAC_ADDR_COUNT							16

// WiFi 0 - 5GHz
#define WIFI_00_CWSSID_SSID_DEFAULT					"SIM8X00AP_5Ghz"
#define WIFI_00_CWAUTH_PWD_DEFAULT					"1234567890"
#define WIFI_00_CWAUTH_TYPE_DEFAULT					5
#define WIFI_00_CWAUTH_ENCRYPT_DEFAULT				4
#define WIFI_00_CWMOCH_MODE_DEFAULT					5    // 5GHz
#define WIFI_00_CWMOCH_CHANNEL_DEFAULT		    	149    // ch=149
#define WIFI_00_CWBCAST_DEFAULT		    			1  
#define WIFI_00_CWISO_ISOLATION_DEFAULT		    	0
#define WIFI_00_CWISO_MACADDR_DEFAULT		    	"00:00:00:00:00:00"

// WiFi 1 - 2.4GHz
#define WIFI_01_CWSSID_SSID_DEFAULT					"SIM8X00AP_2.4Ghz"
#define WIFI_01_CWAUTH_PWD_DEFAULT					"1234567890"
#define WIFI_01_CWAUTH_TYPE_DEFAULT					5
#define WIFI_01_CWAUTH_ENCRYPT_DEFAULT				4
#define WIFI_01_CWMOCH_MODE_DEFAULT					4    // 2.4GHz
#define WIFI_01_CWMOCH_CHANNEL_DEFAULT		    	0    // ch=auto
#define WIFI_01_CWBCAST_DEFAULT		    			1  
#define WIFI_01_CWISO_ISOLATION_DEFAULT		  	  	0
#define WIFI_01_CWISO_MACADDR_DEFAULT		    	"00:00:00:00:00:00"

// WiFi 2 - STA
#define WIFI_02_CWSSID_SSID_DEFAULT					"SIM8X00AP_STA"
#define WIFI_02_CWAUTH_PWD_DEFAULT					"1234567890"
#define WIFI_02_CWAUTH_TYPE_DEFAULT					5
#define WIFI_02_CWAUTH_ENCRYPT_DEFAULT				4
#define WIFI_02_CWMOCH_MODE_DEFAULT					4    // 2.4GHz
#define WIFI_02_CWMOCH_CHANNEL_DEFAULT		    	0    // ch=auto
#define WIFI_02_CWBCAST_DEFAULT		    			1  
#define WIFI_02_CWISO_ISOLATION_DEFAULT		  	  	0
#define WIFI_02_CWISO_MACADDR_DEFAULT		    	"00:00:00:00:00:00"


// Open(close) wifi
#define WIFI_CWMAP_FLAG_OPEN					1
#define WIFI_CWMAP_FLAG_CLOSE					0

// SSID:
#define WIFI_CWSSID_SSID_MAX					32
#define WIFI_CWSSID_SSID_MIN					8

// Broadcast:
#define WIFI_CWBCAST_DISABLED					0
#define WIFI_CWBCAST_ENABLED					1

// Auth:
#define WIFI_CWAUTH_PASSWORD_MAX				32
#define WIFI_CWAUTH_PASSWORD_MIN				8
#define WIFI_CWAUTH_AUTH_OPENSHARE				0
#define WIFI_CWAUTH_AUTH_OPEN					1
#define WIFI_CWAUTH_AUTH_SHARE					2
#define WIFI_CWAUTH_AUTH_WPA					3
#define WIFI_CWAUTH_AUTH_WPA2					4
#define WIFI_CWAUTH_AUTH_WPA_WPA2				5
#define WIFI_CWAUTH_ENCRYPT_NULL				0
#define WIFI_CWAUTH_ENCRYPT_WEP					1
#define WIFI_CWAUTH_ENCRYPT_TKIP				2
#define WIFI_CWAUTH_ENCRYPT_AES					3
#define WIFI_CWAUTH_ENCRYPT_TKIP_AES			4

// Mode and channel:
#define WIFI_CWMOCH_MODE_80211AN_5G				1
#define WIFI_CWMOCH_MODE_80211B_2_4G			2
#define WIFI_CWMOCH_MODE_80211BG_2_4G			3
#define WIFI_CWMOCH_MODE_80211BGN_2_4G			4
#define WIFI_CWMOCH_MODE_80211ACN_5G			5
#define WIFI_CWMOCH_CHANNEL_AUTO				0

// Isolation:
#define WIFI_CWMISO_ISOLATION_OPEN				1
#define WIFI_CWMISO_ISOLATION_CLOSE				0

// NAT:
#define WIFI_CWMISO_NAT_SYMETRIC				0
#define WIFI_CWMISO_NAT_CONE					1

#define WIFI_CWDHCP_DHCP_MAX					16

// MAP Configure:
#define WIFI_CWCMAPCFG_VALUE_AP					0
#define WIFI_CWCMAPCFG_VALUE_APAP				1
#define WIFI_CWCMAPCFG_VALUE_STAAP				2
#define WIFI_CWCMAPCFG_SET_WIFI_0				0
#define WIFI_CWCMAPCFG_SET_WIFI_1				1
#define WIFI_CWCMAPCFG_SET_WIFI_2				2


// STA
#define WIFI_STA_IP_MAX							16
///////////////////////////////////////VARIABLES///////////////////////////////////////
typedef enum{
	AP_MODE = 0,
	APAP_MODE,
	APSTA_MODE
} wifi_mode_type;

typedef enum{
	AP_INDEX_0 = 0,
	AP_INDEX_1,
	AP_INDEX_STA
} ap_index_type;

typedef struct {
	char mac_addr[MAC_ADDR_MAX];
} MAC_ADDR_INFO_t;

typedef struct {
	char ssid[WIFI_CWSSID_SSID_MAX];    			// SSID
	uint8_t broadcast;				 				// Broadcast
	uint8_t auth_type; 				 				// Auth type
	char auth_pwd[WIFI_CWAUTH_PASSWORD_MAX];		// Auth password
	uint8_t encrypt_mode;							// Encrypt mode
	uint8_t channel;								// channel: 
	uint8_t channel_mode;							// mode
	uint8_t isolation;								// isolation function
	char dhcp_host_ip[WIFI_CWDHCP_DHCP_MAX];
	char dhcp_start_ip[WIFI_CWDHCP_DHCP_MAX];
	char dhcp_end_ip[WIFI_CWDHCP_DHCP_MAX];
	char dhcp_time[WIFI_CWDHCP_DHCP_MAX];
	uint8_t client_count; 							// Client count
	MAC_ADDR_INFO_t mac_info[MAC_ADDR_COUNT];
	
	// STA:
	char sta_ip[WIFI_STA_IP_MAX];
	char sta_ssid_ext[WIFI_CWSSID_SSID_MAX];
	uint8_t sta_security;
	uint8_t sta_protocol;
	char sta_password[WIFI_CWAUTH_PASSWORD_MAX];
} WIFI_CONFIG_t;

typedef struct {
	uint8_t open;            		 			// open/close
	uint8_t wifi_mode;
	uint8_t current_ap;
	uint8_t nat_type;	
	WIFI_CONFIG_t wifi[WIFI_COUNT_MAX];
} WIFI_FUNCTION_t;
WIFI_FUNCTION_t g_wifi_info;
typedef unsigned char boolean;
typedef unsigned char uint8;

wifi_mode_type get_wifi_mode();
boolean set_wifi_mode(wifi_mode_type mode);
boolean wifi_power(int act);	
boolean get_wifi_status(uint8 *flag);
boolean set_auth(char *str_pwd, int auth_type, int  encrypt_mode, ap_index_type ap_index);	
boolean get_auth(int *auth_type_ptr, int *encrypt_mode_ptr, char *pwd_str, ap_index_type ap_index);	
boolean set_ssid(char *SSID, ap_index_type ap_index);		
boolean get_ssid(char *str_SSID, ap_index_type ap_index);		
boolean set_bcast(int broadcast, ap_index_type ap_index);
boolean get_bcast(int *broadcast,ap_index_type ap_index);	
boolean get_dhcp(char *host_ip_str, char *start_ip_str, char *end_ip_str, char *time_str);	
int get_client_count(ap_index_type ap_index);		
boolean get_mac_addr(char *mac_addr, ap_index_type ap_index);	    
boolean get_sta_ip(char *ip_str, int len);	
boolean set_sta_cfg(char *ssid_str, char *psk_value);	
boolean get_sta_cfg(char *ssid_str, char *psk_value);		
boolean sta_scan(char *list_str);
boolean sta_scan_with_sig(char *list_str);		
boolean set_user_name_pwd(char *sz_usrname, char *sz_usrpwd);		
boolean get_user_name_pwd(char *sz_usrname, int len_name, char *sz_usrpwd, int len_pwd);		
boolean get_net_status(char *net_enable_str, int *to_be_save);		
void restore_wifi();
boolean set_ssid_and_auth(char *SSID, char *str_pwd, int auth_type, int  encrypt_mode, ap_index_type ap_index);
boolean wifi_is_sta_mode();	
boolean sta_init(int sta_enable);	
boolean get_sta_status(uint8 *flag);	
boolean wifi_net_cnct(int act, int to_be_save);	
boolean wifi_get_net_status(int *act);	
uint8 wifi_get_err_code();

void wifi_init(void);
uint8_t wifi_open_read(void);
uint8_t wifi_open_write(uint8_t flag);
uint8_t wifi_ssid_read(uint8_t current_ap);
uint8_t wifi_ssid_write(uint8_t current_ap, char *ssid);
uint8_t wifi_broadcast_read(uint8_t current_ap);
uint8_t wifi_broadcast_write(uint8_t current_ap, uint8_t broadcast);
uint8_t wifi_auth_read(uint8_t current_ap);
uint8_t wifi_auth_write(uint8_t current_ap, char *auth_pwd, uint8_t auth_type, uint8_t  encrypt_mode);
uint8_t wifi_mode_channel_read(uint8_t current_ap);
uint8_t wifi_mode_channel_write(uint8_t current_ap, uint8_t mode, uint8_t channel);
uint8_t wifi_isolation_read(uint8_t current_ap);
uint8_t wifi_isolation_write(uint8_t current_ap, uint8_t isolation);
uint8_t wifi_dhcp_read(uint8_t current_ap);
uint8_t wifi_nat_read(void);
uint8_t wifi_nat_write(uint8_t nat_type);
uint8_t wifi_client_count_read(uint8_t current_ap);
uint8_t wifi_config_read(void);
uint8_t wifi_config_mode_write(uint8_t wifi_mode);
uint8_t wifi_config_current_ap_write(uint8_t current_ap);
uint8_t wifi_mac_address_read(uint8_t current_ap);
 
bool wifi_sta_ip_read(void);
bool wifi_sta_cfg_read(void);
bool wifi_sta_cfg_write(char *ssid_str, char *psk_value);

void wifi_manager(void);
void wifi_read_all(void);
#endif
