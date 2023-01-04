/**	@brief implement all functions of SD Card.
	@date
		- 2021_07_23: Create.
*/
#include "SdcardControl.h"
#include "string.h"

//#define _DB_LOAD_
//#define _DB_STORE_
//#define _DB_ETH_LOAD_
//#define _DB_ETH_STORE_
///////////////////////////////////////LOCAL CONSTANTS/////////////////////////////////////////////
// Storage:
#define STORE_LINE_SZ					512
#define STORE_FORMAT_SIGN				"MOBIFONE_RD_FWA"

/////////////////////////////////VARIABLES///////////////////////////////////////////
// Use in Load/Store function:
char line[STORE_LINE_SZ] = {0};

/////////////////////////////////PUBLIC FUNCTION/////////////////////////////////////
/*****************************************************************************
	* Function Name : sd card init:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
/**	@brief SD card init:.
*/
void sdcard_init()
{	
	sd_general_config_load();
	
	// Load data from Wifi:
	//sd_wifi_config_load();
	
	// Load data from ethernet:
	sd_ethernet_config_load();
	
	// Load data from gps:
	sd_gps_config_load();
	
	// Load data from 4g:
	sd_lte4g_config_load();
}

/*****************************************************************************
	* Function Name : sd_general_config_load:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_general_config_load(void)
{
}

/*****************************************************************************
	* Function Name : sd_general_config_store:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_general_config_store(void)
{
}

/*****************************************************************************
	* Function Name : sd_wifi_config_load:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/ 
uint8_t sd_wifi_config_load(void)
{
	int fd;
	int16_t line_sz = -1;
	char temp_str[STORE_LINE_SZ] = {0};
	uint8_t i = 0, j = 0, k = 0, m = 0, err_flg = 0, cnt=0;
	uint8_t ret = 0;
#ifdef _DB_LOAD_
	printf("\r\nWifi config load:");
#endif
	// Start reading file:
	fd = open("/media/card/wifi_info.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);	
	if (fd != -1) {				
		// Read first line for Format Sign:
		memset(line, 0, sizeof(line));
		line_sz = read(fd, line, sizeof(line));
	#ifdef _DB_LOAD_
		printf("\n\rWifi=%s", line);
	#endif
		if (line_sz == -1) {
			err_flg = 1;
			goto end_proc;
		} 
		
		// Check wifi_open:
		for (i = 0; i < line_sz; i++) {
			if (line[i] == ':') {
				for (j = i+1; j < line_sz; j++) {
					if (line[j] = ',') {
						break;
					} else {
						temp_str[m++] = line[j];
					}
				}
				break;
			}
		}
		g_wifi_info.open = atoi(temp_str);
		
		// Check wifi_mode:
		m = 0;
		for (k = i; k < line_sz; k++) {
			if (line[k] == ':') {
				for (j = k+1; j < line_sz; j++) {
					if (line[j] = ',') {
						i = i + m;
						break;
					} else {
						temp_str[m++] = line[j];
					}
				}
				break;
			}
		}
		g_wifi_info.wifi_mode = atoi(temp_str);
		
		// Check nat_type:
		m = 0;
		for (k = i; k < line_sz; k++) {
			if (line[k] == ':') {
				for (j = k+1; j < line_sz; j++) {
					if (line[j] = ',') {
						i = i+m;
						break;
					} else {
						temp_str[m++] = line[j];
					}
				}
				break;
			}
		}
		g_wifi_info.nat_type = atoi(temp_str);
		
		// Update wifi
		if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_AP) {
			// Check SSID_1:
			m = 0;
			for (k = i; k < line_sz; k++) {
				if (line[k] == ':') {
					for (j = k+1; j < line_sz; j++) {
						if (line[j] = ',') {
							i = i+m;
							break;
						} else {
							temp_str[m++] = line[j];
						}
					}
					break;
				}
			}
			g_wifi_info.nat_type = atoi(temp_str);
		} else if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_AP) {
		}
		

	}

end_proc:
	printf("\n\rerr_flg=%u", err_flg);
	if (err_flg == 1) {
		g_wifi_info.wifi_mode = WIFI_CWCMAPCFG_VALUE_AP;
		ret = 0;
	} else {
		ret = 1;
	}
	// Close file:
	close(fd);
	return ret;
}

/*****************************************************************************
	* Function Name : sd_wifi_config_store:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_wifi_config_store(void)
{
	#define SBUF_SZ					64u
	uint8_t ret = 0U, i = 0;
	int fd;
	char sbuf[SBUF_SZ];
	uint16_t line_sz = 0, wr_sz = 0;

#ifdef _DB_STORE_
	printf("\r\nWifi config store:");
#endif
	
	// Open file to store:
	fd = open("/media/card/wifi_info.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		// Mark as Success:
		ret = 1U;
	
		// Store:
		snprintf(line, STORE_LINE_SZ, "{\"wifi_open\":%u,\"wifi_mode\":%u,\"wifi_nat\":%u,", g_wifi_info.open, g_wifi_info.wifi_mode, g_wifi_info.nat_type);

		// Check mode wifi:
		if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_AP) {
			// start wifi 1:
			snprintf(sbuf, SBUF_SZ, "\"wifi_01\":{\"ssid\":\"%s\",\"bcast\":%u,", g_wifi_info.wifi[0].ssid, g_wifi_info.wifi[0].broadcast); 
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"auth_type\":%u,\"auth_pwd\":\"%s\",\"encrypt_mode\":%u,", g_wifi_info.wifi[0].auth_type, g_wifi_info.wifi[0].auth_pwd, g_wifi_info.wifi[0].encrypt_mode);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"channel\":%u,\"channel_mode\":%u,\"isolation\":%u,", g_wifi_info.wifi[0].channel, g_wifi_info.wifi[0].channel_mode, g_wifi_info.wifi[0].isolation);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"dhcp_host_ip\":\"%s\",\"dhcp_start_ip\":\"%s\",", g_wifi_info.wifi[0].dhcp_host_ip, g_wifi_info.wifi[0].dhcp_start_ip);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"dhcp_end_ip\":\"%s\",\"dhcp_time\":\"%s\",", g_wifi_info.wifi[0].dhcp_end_ip, g_wifi_info.wifi[0].dhcp_time);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"client_count\":%u,", g_wifi_info.wifi[0].client_count);
			strcat(line, sbuf);
			for (i = 0; i < g_wifi_info.wifi[0].client_count + 1; i++) {
				if (i == g_wifi_info.wifi[0].client_count) {
					snprintf(sbuf, SBUF_SZ, "\"mac_addr_%u\":\"%s\"},", i, g_wifi_info.wifi[0].mac_info[i].mac_addr);
					strcat(line, sbuf);
				} else {
					snprintf(sbuf, SBUF_SZ, "\"mac_addr_%u\":\"%s\",", i, g_wifi_info.wifi[0].mac_info[i].mac_addr);
					strcat(line, sbuf);
				}
			}
		} else if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_APAP) {
			// start wifi 1:
			snprintf(sbuf, SBUF_SZ, "\"wifi_01\":{\"ssid\":\"%s\",\"bcast\":%u,", g_wifi_info.wifi[0].ssid, g_wifi_info.wifi[0].broadcast); 
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"auth_type\":%u,\"auth_pwd\":\"%s\",\"encrypt_mode\":%u,", g_wifi_info.wifi[0].auth_type, g_wifi_info.wifi[0].auth_pwd, g_wifi_info.wifi[0].encrypt_mode);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"channel\":%u,\"channel_mode\":%u,\"isolation\":%u,", g_wifi_info.wifi[0].channel, g_wifi_info.wifi[0].channel_mode, g_wifi_info.wifi[0].isolation);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"dhcp_host_ip\":\"%s\",\"dhcp_start_ip\":\"%s\",", g_wifi_info.wifi[0].dhcp_host_ip, g_wifi_info.wifi[0].dhcp_start_ip);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"dhcp_end_ip\":\"%s\",\"dhcp_time\":\"%s\",", g_wifi_info.wifi[0].dhcp_end_ip, g_wifi_info.wifi[0].dhcp_time);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"client_count\":%u,", g_wifi_info.wifi[0].client_count);
			strcat(line, sbuf);
			for (i = 0; i < g_wifi_info.wifi[0].client_count + 1; i++) {
				if (i == g_wifi_info.wifi[0].client_count) {
					snprintf(sbuf, SBUF_SZ, "\"mac_addr_%u\":\"%s\"},", i, g_wifi_info.wifi[0].mac_info[i].mac_addr);
					strcat(line, sbuf);
				} else {
					snprintf(sbuf, SBUF_SZ, "\"mac_addr_%u\":\"%s\",", i, g_wifi_info.wifi[0].mac_info[i].mac_addr);
					strcat(line, sbuf);
				}
			}
			// Start wifi2:
			snprintf(sbuf, SBUF_SZ, "\"wifi_02\":{\"ssid\":\"%s\",\"bcast\":%u,", g_wifi_info.wifi[1].ssid, g_wifi_info.wifi[1].broadcast); 
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"auth_type\":%u,\"auth_pwd\":\"%s\",\"encrypt_mode\":%u,", g_wifi_info.wifi[1].auth_type, g_wifi_info.wifi[1].auth_pwd, g_wifi_info.wifi[1].encrypt_mode);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"channel\":%u,\"channel_mode\":%u,\"isolation\":%u,", g_wifi_info.wifi[1].channel, g_wifi_info.wifi[1].channel_mode, g_wifi_info.wifi[1].isolation);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"dhcp_host_ip\":\"%s\",\"dhcp_start_ip\":\"%s\",", g_wifi_info.wifi[1].dhcp_host_ip, g_wifi_info.wifi[1].dhcp_start_ip);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"dhcp_end_ip\":\"%s\",\"dhcp_time\":\"%s\",", g_wifi_info.wifi[1].dhcp_end_ip, g_wifi_info.wifi[1].dhcp_time);
			strcat(line, sbuf);
			snprintf(sbuf, SBUF_SZ, "\"client_count\":%u,", g_wifi_info.wifi[1].client_count);
			strcat(line, sbuf);
			for (i = 0; i < g_wifi_info.wifi[1].client_count + 1; i++) {
				if (i == g_wifi_info.wifi[1].client_count) {
					snprintf(sbuf, SBUF_SZ, "\"mac_addr_%u\":\"%s\"}", i, g_wifi_info.wifi[1].mac_info[i].mac_addr);
					strcat(line, sbuf);
				} else {
					snprintf(sbuf, SBUF_SZ, "\"mac_addr_%u\":\"%s\",", i, g_wifi_info.wifi[1].mac_info[i].mac_addr);
					strcat(line, sbuf);
				}
			}

			snprintf(sbuf, SBUF_SZ, "}");  // End.
			strcat(line, sbuf); 
		}

		line_sz = strlen(line);	
		wr_sz = write(fd, (uint8_t *)line, line_sz);
		if (wr_sz != line_sz) {
			printf(" -> Store error");
			ret = 0U;
			goto end_proc;
		}
		
		// Close file:
		close(fd);
	}
end_proc:
#ifdef _DB_STORE_
	printf(" -> Store.Ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
	* Function Name : sd_ethernet_config_store:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_ethernet_config_load(void)
{

	int fd;
	int16_t line_sz = -1;
	char temp_str[STORE_LINE_SZ] = {0};
	uint8_t i = 0, j = 0, err_flg = 0, m = 0, cnt=0;
	uint8_t ret = 0;
#ifdef _DB_ETH_LOAD_
	printf("\r\nEthernet config load:");
#endif
	// Start reading file:
	fd = open("/media/card/ethernet_info.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);	
	if (fd != -1) {				
		// Read first line for Format Sign:
		memset(line, 0, sizeof(line));
		line_sz = read(fd, line, sizeof(line));
	#ifdef _DB_ETH_LOAD_
		printf("\n\rEthernet=%s", line);
	#endif
		if (line_sz == -1) {
			err_flg = 1;
			goto end_proc;
		} 
		strcpy(temp_str, line);
		for (i = 0; i < line_sz; i++) {
			if (temp_str[i] == ',') {
				break;
			}
		}
		temp_str[i] = '\0';
		if (strcmp(temp_str, STORE_FORMAT_SIGN) != 0) {
			err_flg = 1;
			goto end_proc;
		}
	#ifdef _DB_ETH_LOAD_
		printf("\n\rFormat_sign=%s,err=%u", temp_str, err_flg);
	#endif
		strcpy(temp_str, line);
		
		// Update driver_enable:
		if ((temp_str[i] == ',') && (temp_str[i+1] != ',')) {
			i++;
			g_ethernet_info.driver_en = atoi(&temp_str[i]);
			i++;
		} else {
			err_flg = 1;
			goto end_proc;
		}
	#ifdef _DB_ETH_LOAD_
		printf("\n\rdriver_enable=%u,err=%u", g_ethernet_info.driver_en, err_flg);
	#endif
		
		// Update driver_type:
		if ((temp_str[i] == ',') && (temp_str[i+1] != ',')) {
			i++;
			g_ethernet_info.driver_type = atoi(&temp_str[i]);
			i++;
		} else {
			err_flg = 1;
			goto end_proc;			
		}
	#ifdef _DB_ETH_LOAD_
		printf("\n\rdriver_type = %u,err=%u", g_ethernet_info.driver_type, err_flg);
	#endif
		
		// Update bring_updown_enable:
		if ((temp_str[i] == ',') && (temp_str[i+1] != ',')) {
			i++;
			g_ethernet_info.bring_updown_en = atoi(&temp_str[i]);
			i++;
		} else {
			err_flg = 1;
			goto end_proc;			
		}
	#ifdef _DB_ETH_LOAD_
		printf("\n\rbring_updown_en = %u,err=%u", g_ethernet_info.bring_updown_en,err_flg);
	#endif
		
		// Update ip_static_en:
		if ((temp_str[i] == ',') && (temp_str[i+1] != ',')) {
			i++;
			g_ethernet_info.ip_static_en = atoi(&temp_str[i]);
			i++;
		} else {
			g_ethernet_info.ip_static_en = 0;
			err_flg = 1;
			goto end_proc;			
		}
	#ifdef _DB_ETH_LOAD_
		printf("\n\rip_static_en = %u, err=%u", g_ethernet_info.ip_static_en, err_flg);
	#endif
		
		// Update ip_addr:
		if ((temp_str[i] == ',') && (temp_str[i+1] == ',')) {
			i++;
		} else if ((temp_str[i+1] != ',') && (temp_str[i] == ',')) {
			i++;
			m = 0;
			for (j = i; j < line_sz; j++) {
				if (temp_str[j] != ',') {
					g_ethernet_info.ip_addr[m] = temp_str[j];
					m++;
				} else {
					break;
				}
				cnt++;
			}
			i = i+cnt;
			cnt = 0;
		}
	#ifdef _DB_ETH_LOAD_
		printf("\n\ripaddr=%s, err=%u", g_ethernet_info.ip_addr, err_flg);
	#endif
		
		// Update netmask:
		if ((temp_str[i] == ',') && (temp_str[i+1] == ',')) {
			i++;
		} else if ((temp_str[i+1] != ',') && (temp_str[i] == ',')) {
			i++;
			m = 0;
			for (j = i; j < line_sz; j++) {
				if (temp_str[j] != ',') {
					g_ethernet_info.netmask[m] = temp_str[j];
					m++;
				} else {
					break;
				}
				cnt++;
			}
			i = i+cnt;
			cnt = 0;
		}
	#ifdef _DB_ETH_LOAD_
		printf("\n\rnetmask=%s, err=%u", g_ethernet_info.netmask, err_flg);
	#endif
	}
		

end_proc:
#ifdef _DB_ETH_LOAD_
	printf("\n\rerr_flg=%u", err_flg);
#endif

	if (err_flg == 1) {
		err_flg = 0;
		g_ethernet_info.driver_en = DRIVER_ENABLE;
		g_ethernet_info.driver_type = DRIVER_TYPE_AT803X;
		g_ethernet_info.bring_updown_en = BRINGUP_ENABLE;
		g_ethernet_info.ip_static_en = 0;
		sd_ethernet_config_store();
		ret = 0;
	} else {
		ret = 1;
	}
	
	// Close file:
	close(fd);
	return ret;
}

/*****************************************************************************
	* Function Name : sd_ethernet_config_store:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_ethernet_config_store(void)
{
	#define SBUF_SZ					64u
	uint8_t ret = 0U;
	int fd;
	char sbuf[SBUF_SZ];
	uint16_t line_sz = 0, wr_sz = 0;

#ifdef _DB_ETH_STORE_
	printf("\r\nEthernet config store:");
#endif	

	// Open file to store:
	fd = open("/media/card/ethernet_info.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		// Mark as Success:
		ret = 1U;
	
		// Store:
		snprintf(line, STORE_LINE_SZ, "{\"driver_en\":%u,\"driver_type\":%u,", g_ethernet_info.driver_en, g_ethernet_info.driver_type);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"bring_updown_en\":%u,\"ip_static_en\":%u,", g_ethernet_info.bring_updown_en, g_ethernet_info.ip_static_en);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"ip_addr\":\"%s\",\"netmask\":\"%s\"}", g_ethernet_info.ip_addr, g_ethernet_info.netmask);
		strcat(line, sbuf);
		line_sz = strlen(line);	
		wr_sz = write(fd, (uint8_t *)line, line_sz);
		if (wr_sz != line_sz) {
			printf(" -> Store error");
			ret = 0U;
			goto end_proc;
		}
		
		// Close file:
		close(fd);
	}
end_proc:
#ifdef _DB_ETH_STORE_
	printf(" -> Store.Ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
	* Function Name : sd_lte4G_config_load:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_lte4g_config_load(void)
{
}

/*****************************************************************************
	* Function Name : sd_lte4G_config_store:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_lte4g_config_store(void)
{
	#define SBUF_SZ					64u
	uint8_t ret = 0U;
	int fd;
	char sbuf[SBUF_SZ];
	uint16_t line_sz = 0, wr_sz = 0;

#ifdef _DB_LTE4G_STORE_
	printf("\r\nLTE4G config store:");
#endif	

	// Open file to store:
	fd = open("/media/card/lte4g_info.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd != -1) {
		// Mark as Success:
		ret = 1U;
	
		// Start
		snprintf(line, STORE_LINE_SZ, "{\"sim_status\":%u,\"sim_type\":%u,", g_lte4g_info.sim_status.card_status, g_lte4g_info.sim_status.app_type);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"sim_state\":%u,\"pin_state\":%u,", g_lte4g_info.sim_status.app_state, g_lte4g_info.sim_status.pin.pin_state);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"pin_retries\":%u,\"puk_retries\":%u,", g_lte4g_info.sim_status.pin.pin_retries, g_lte4g_info.sim_status.pin.puk_retries);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"sim_iccid\":\"%s\",", g_lte4g_info.sim_iccid);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"sim_imsi\":\"%s\",", g_lte4g_info.sim_imsi);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"phone_number\":\"%s\",", g_lte4g_info.phone_number);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"rssi_4g\":%u,\"system_mode\":\"%s\",", g_lte4g_info.rssi_4g, g_lte4g_info.system_mode);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"operation_mode\":\"%s\",\"mobile_country_code\":\"%s\",", g_lte4g_info.operation_mode, g_lte4g_info.mobile_country_code);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"mobile_network_code\":\"%s\",\"location_area_code\":\"%s\",", g_lte4g_info.mobile_network_code, g_lte4g_info.location_area_code);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"service_cell_id\":\"%s\",\"afrcn\":\"%s\",", g_lte4g_info.service_cell_id, g_lte4g_info.afrcn);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"freq_band\":\"%s\",\"current_4g_data\":\"%s\",", g_lte4g_info.freq_band, g_lte4g_info.current_4g_data);
		strcat(line, sbuf);
		snprintf(sbuf, SBUF_SZ, "\"sim_status\":%u", g_lte4g_info.network_mode);
		strcat(line, sbuf);
		line_sz = strlen(line);	
		wr_sz = write(fd, (uint8_t *)line, line_sz);
		if (wr_sz != line_sz) {
			printf(" -> Store error");
			ret = 0U;
			goto end_proc;
		}
		
		// Close file:
		close(fd);
	}
end_proc:
#ifdef _DB_LTE4G_STORE_
	printf(" -> Store.Ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
	* Function Name : sd_gps_config_load:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_gps_config_load(void)
{
}

/*****************************************************************************
	* Function Name : sd_gps_config_store:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_gps_config_store(void)
{
}

/*****************************************************************************
	* Function Name : sd_log_load:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_log_load(void)
{
}

/*****************************************************************************
	* Function Name : sd_log_store:
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 
*****************************************************************************/
uint8_t sd_log_store(void)
{
}



