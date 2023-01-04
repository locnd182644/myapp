/**	@brief implement all functions of Wifi.
	@date
		- 2021_07_23: Create.
*/
#include "WiFiControl.h"
#include "LedControl.h"

// #define _DB_INIT_
// #define _DB_READ_
// #define _DB_WRITE_

////////////////////////////////////////////CONSTANT////////////////////////////////////
// Storage:
#define AT_LINE_SZ 128
/////////////////////////////////////////////VARIABLES///////////////////////////////////
static char str_temp[AT_LINE_SZ];
static char buff[100];
static uint8_t g_wifi_init_flg;
////////////////////////////////////////////LOCAL FUNCTIONS///////////////////////////////

/*****************************************************************************
 * Function Name : wifi_init
 * Description   : Init all functions of wifi.
 * Input         : None
 * Output        : None
 * Return        : None
 *****************************************************************************/
void wifi_init(void)
{

	// Enable Wifi:
	wifi_open_write(WIFI_CWMAP_FLAG_OPEN);
	wifi_nat_read();
	// wifi_config_mode_write(WIFI_CWCMAPCFG_VALUE_APAP);
	wifi_config_read();

	if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_AP)
	{
		wifi_ssid_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWSSID_SSID_DEFAULT);
		wifi_broadcast_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWBCAST_DEFAULT);
		wifi_auth_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWAUTH_PWD_DEFAULT, WIFI_00_CWAUTH_TYPE_DEFAULT, WIFI_00_CWAUTH_ENCRYPT_DEFAULT);
		wifi_isolation_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWISO_ISOLATION_DEFAULT);
		wifi_mode_channel_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWMOCH_MODE_DEFAULT, WIFI_00_CWMOCH_CHANNEL_DEFAULT);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_0);
	}
	else if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_APAP)
	{

		wifi_ssid_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWSSID_SSID_DEFAULT);
		wifi_broadcast_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWBCAST_DEFAULT);
		wifi_auth_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWAUTH_PWD_DEFAULT, WIFI_00_CWAUTH_TYPE_DEFAULT, WIFI_00_CWAUTH_ENCRYPT_DEFAULT);
		wifi_isolation_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWISO_ISOLATION_DEFAULT);
		wifi_mode_channel_write(WIFI_CWCMAPCFG_SET_WIFI_0, WIFI_00_CWMOCH_MODE_DEFAULT, WIFI_00_CWMOCH_CHANNEL_DEFAULT);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_0);

		wifi_ssid_write(WIFI_CWCMAPCFG_SET_WIFI_1, WIFI_01_CWSSID_SSID_DEFAULT);
		wifi_broadcast_write(WIFI_CWCMAPCFG_SET_WIFI_1, WIFI_01_CWBCAST_DEFAULT);
		wifi_auth_write(WIFI_CWCMAPCFG_SET_WIFI_1, WIFI_01_CWAUTH_PWD_DEFAULT, WIFI_01_CWAUTH_TYPE_DEFAULT, WIFI_01_CWAUTH_ENCRYPT_DEFAULT);
		wifi_isolation_write(WIFI_CWCMAPCFG_SET_WIFI_1, WIFI_01_CWISO_ISOLATION_DEFAULT);
		wifi_mode_channel_write(WIFI_CWCMAPCFG_SET_WIFI_1, WIFI_01_CWMOCH_MODE_DEFAULT, WIFI_01_CWMOCH_CHANNEL_DEFAULT);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_1);
	}
	else if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_STAAP)
	{
		wifi_ssid_write(WIFI_CWCMAPCFG_SET_WIFI_2, WIFI_02_CWSSID_SSID_DEFAULT);
		wifi_broadcast_write(WIFI_CWCMAPCFG_SET_WIFI_2, WIFI_02_CWBCAST_DEFAULT);
		wifi_auth_write(WIFI_CWCMAPCFG_SET_WIFI_2, WIFI_02_CWAUTH_PWD_DEFAULT, WIFI_02_CWAUTH_TYPE_DEFAULT, WIFI_02_CWAUTH_ENCRYPT_DEFAULT);
		wifi_isolation_write(WIFI_CWCMAPCFG_SET_WIFI_2, WIFI_02_CWISO_ISOLATION_DEFAULT);
		wifi_mode_channel_write(WIFI_CWCMAPCFG_SET_WIFI_2, WIFI_02_CWMOCH_MODE_DEFAULT, WIFI_02_CWMOCH_CHANNEL_DEFAULT);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_2);
	}
	g_wifi_init_flg = 1;
}

/*****************************************************************************
 * Function Name : wifi_open_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_open_read(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0;

	// Read Open/Close status:
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWMAP?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					break;
				}
			}
			if (buff[j + 2] != ' ')
			{
				g_wifi_info.open = atoi(&buff[j + 2]);
			}
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWifi.open=%u", g_wifi_info.open);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_open_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_open_write(uint8_t flag)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;

#ifdef _DB_WRITE_
	printf("\n\rWifiWrite.flag=%u", flag);
#endif

	// Check param:
	wifi_open_read();
	if (g_wifi_info.open == flag)
	{
		goto end_proc;
	}

	// Set open/close wifi
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%u", "AT+CWMAP=", flag);
		len = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_wifi_info.open = flag;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf(" ->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_ssid_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_ssid_read(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, m = 0, k = 0;

	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read SSID:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWSSID?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					k = 0;
					for (m = j + 3; m < len; m++)
					{
						if ((buff[m] == '"') && ((buff[m + 1] == ' ') || (buff[m + 1] == '\r')))
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					break;
				}
			}
			strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].ssid, str_temp);
			memset(str_temp, 0, sizeof(str_temp));
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.ssid:%s", g_wifi_info.wifi[g_wifi_info.current_ap].ssid);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_ssid_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_ssid_write(uint8_t current_ap, char *ssid)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifi.ssid=%s", ssid);
#endif

	// Check param:
	wifi_ssid_read(current_ap);
	if (strcmp(ssid, g_wifi_info.wifi[g_wifi_info.current_ap].ssid) == 0)
	{
		goto end_proc;
	}

	// Set SSID:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		if ((strcmp(ssid, "SIM7800MIFI") == 0) || (strcmp(ssid, "SIM7800MIFI_1") == 0))
		{
			snprintf(str_temp, AT_LINE_SZ, "%s%s%s%s", "AT+CWSSID=", "\"", WIFI_01_CWSSID_SSID_DEFAULT, "\"");
		}
		else
		{
			snprintf(str_temp, AT_LINE_SZ, "%s%s%s%s", "AT+CWSSID=", "\"", ssid, "\"");
		}
		len = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].ssid, ssid);
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_broadcast_read
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_broadcast_read(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0;

	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read broadcast status:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWBCAST?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					if (buff[j + 2] != ' ')
					{
						g_wifi_info.wifi[g_wifi_info.current_ap].broadcast = atoi(&buff[j + 2]);
					}
					break;
				}
			}
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.bcast=%u", g_wifi_info.wifi[g_wifi_info.current_ap].broadcast);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_broadcast_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_broadcast_write(uint8_t current_ap, uint8_t broadcast)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifiW.bcast=%u", broadcast);
#endif

	// Check param:
	wifi_broadcast_read(current_ap);
	if (g_wifi_info.wifi[g_wifi_info.current_ap].broadcast == broadcast)
	{
		goto end_proc;
	}

	// Set the broadcast function:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%u", "AT+CWBCAST=", broadcast);
		len = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_wifi_info.wifi[g_wifi_info.current_ap].broadcast = broadcast;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_auth_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_auth_read(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, k = 0, m = 0;

	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read Auth:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWAUTH?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					j = j + 2;
					if (buff[j] != ' ')
					{
						g_wifi_info.wifi[g_wifi_info.current_ap].auth_type = atoi(&buff[j]);
						j = j + 2;
					}
					if (buff[j - 1] == ',')
					{
						g_wifi_info.wifi[g_wifi_info.current_ap].encrypt_mode = atoi(&buff[j]);
						j = j + 3;
					}
					k = 0;
					for (m = j; m < len; m++)
					{
						if ((buff[m] == '"') && ((buff[m + 1] == ' ') || (buff[m + 1] == '\r')))
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].auth_pwd, str_temp);
					memset(str_temp, 0, sizeof(str_temp));
					break;
				}
			}
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.pwd=%s,type=%u,encrypt=%u", g_wifi_info.wifi[g_wifi_info.current_ap].auth_pwd,
		   g_wifi_info.wifi[g_wifi_info.current_ap].auth_type, g_wifi_info.wifi[g_wifi_info.current_ap].encrypt_mode);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_auth_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_auth_write(uint8_t current_ap, char *auth_pwd, uint8_t auth_type, uint8_t encrypt_mode)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifiWrite.auth_pwd=%s, type=%u,mode=%u", auth_pwd, auth_type, encrypt_mode);
#endif

	// Check param:
	wifi_auth_read(current_ap);
	if ((strcmp(auth_pwd, g_wifi_info.wifi[g_wifi_info.current_ap].auth_pwd) == 0) &&
		(g_wifi_info.wifi[g_wifi_info.current_ap].auth_type == auth_type) &&
		(g_wifi_info.wifi[g_wifi_info.current_ap].encrypt_mode == encrypt_mode))
	{
		goto end_proc;
	}

	// Set auth:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%u%c%u%c%s%s%s", "AT+CWAUTH=", auth_type, ',', encrypt_mode, ',', "\"", auth_pwd, "\"");
		len = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_wifi_info.wifi[g_wifi_info.current_ap].auth_type = auth_type;
			strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].auth_pwd, auth_pwd);
			g_wifi_info.wifi[g_wifi_info.current_ap].encrypt_mode = encrypt_mode;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_mode_channel_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_mode_channel_read(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, m = 0, k = 0;

	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read mode/channel status:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWMOCH?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					j = j + 2;
					if (buff[j] != ' ')
					{
						g_wifi_info.wifi[g_wifi_info.current_ap].channel_mode = atoi(&buff[j]);
						j = j + 2;
					}
					k = 0;
					for (m = j; m < len; m++)
					{
						if ((buff[m] == ' ') || (buff[m] == '\r'))
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					g_wifi_info.wifi[g_wifi_info.current_ap].channel = atoi(str_temp);
					memset(str_temp, 0, sizeof(str_temp));
					break;
				}
			}
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.channel=%u, mode=%u", g_wifi_info.wifi[g_wifi_info.current_ap].channel,
		   g_wifi_info.wifi[g_wifi_info.current_ap].channel_mode);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_mode_channel_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_mode_channel_write(uint8_t current_ap, uint8_t mode, uint8_t channel)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifiW.mode=%u, channel=%u", mode, channel);
#endif

	// Check param:
	wifi_mode_channel_read(current_ap);
	if ((g_wifi_info.wifi[g_wifi_info.current_ap].channel == channel) &&
		(g_wifi_info.wifi[g_wifi_info.current_ap].channel_mode == mode))
	{
		goto end_proc;
	}

	// Set channel and mode:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%u%c%u", "AT+CWMOCH=", mode, ',', channel);
		len = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_wifi_info.wifi[g_wifi_info.current_ap].channel = channel;
			g_wifi_info.wifi[g_wifi_info.current_ap].channel_mode = mode;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_isolation_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_isolation_read(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0;

	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read Isolation status:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWISO?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					if (buff[j + 2] != ' ')
					{
						g_wifi_info.wifi[g_wifi_info.current_ap].isolation = atoi(&buff[j + 2]);
					}
					break;
				}
			}
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.iso=%u", g_wifi_info.wifi[g_wifi_info.current_ap].isolation);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_isolation_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_isolation_write(uint8_t current_ap, uint8_t isolation)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifiWrite.isolation=%u", isolation);
#endif

	// Check param:
	wifi_isolation_read(current_ap);
	if (g_wifi_info.wifi[g_wifi_info.current_ap].isolation == isolation)
	{
		goto end_proc;
	}

	// Set isolation:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%u", "AT+CWISO=", isolation);
		len = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_wifi_info.wifi[g_wifi_info.current_ap].isolation = isolation;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_dhcp_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_dhcp_read(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, m = 0, k = 0;

	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read Auth:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWDHCP?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					// DHCP_host_ip:
					k = 0;
					j = j + 3;
					for (m = j; m < len; m++)
					{
						if ((buff[m] == '"') && (buff[m + 1] == ','))
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					j = j + k;
					strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_host_ip, str_temp);
					memset(str_temp, 0, sizeof(str_temp));

					// DHCP_start_ip:
					k = 0;
					j = j + 3;
					for (m = j; m < len; m++)
					{
						if ((buff[m] == '"') && (buff[m + 1] == ','))
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					j = j + k;
					strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_start_ip, str_temp);
					memset(str_temp, 0, sizeof(str_temp));

					// DHCP_end_ip:
					k = 0;
					j = j + 3;
					for (m = j; m < len; m++)
					{
						if ((buff[m] == '"') && (buff[m + 1] == ','))
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					j = j + k;
					strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_end_ip, str_temp);
					memset(str_temp, 0, sizeof(str_temp));

					// Lease time:
					k = 0;
					j = j + 2;
					for (m = j; m < len; m++)
					{
						if ((buff[m] == ' ') || (buff[m] == '\r'))
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					strcpy(g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_time, str_temp);
					memset(str_temp, 0, sizeof(str_temp));
				}
			}
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFiead:");
	printf("hostip=%s,startip=%s,endip=%s,dhcp_time=%s", g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_host_ip, g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_start_ip,
		   g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_end_ip, g_wifi_info.wifi[g_wifi_info.current_ap].dhcp_time);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_nat_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_nat_read(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0;

	// Read NAT status:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWNAT?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					if (buff[j + 2] != ' ')
					{
						g_wifi_info.nat_type = atoi(&buff[j + 2]);
					}
					break;
				}
			}
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.NAT_type=%u", g_wifi_info.nat_type);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_nat_write
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_nat_write(uint8_t nat_type)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifiW.nat_type=%u", nat_type);
#endif

	// Check param:
	wifi_nat_read();
	if (g_wifi_info.nat_type == nat_type)
	{
		goto end_proc;
	}

	// Set the nat function:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%u", "AT+CWNAT=", nat_type);
		ret = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (ret > 0)
		{
			g_wifi_info.nat_type = nat_type;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_client_count_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
uint8_t wifi_client_count_read(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, k = 0, m = 0;

	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read Isolation status:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWCLICNT?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					k = 0;
					for (m = j + 2; m < len; m++)
					{
						if (buff[m] == ' ')
						{
							break;
						}
						else
						{
							str_temp[k] = buff[m];
							k++;
						}
					}
					g_wifi_info.wifi[g_wifi_info.current_ap].client_count = atoi(str_temp);
					break;
				}
			}
			memset(str_temp, 0, sizeof(str_temp));
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.Client_cnt=%u", g_wifi_info.wifi[g_wifi_info.current_ap].client_count);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_config_read
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_config_read(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, m = 0;

	// Read WIfi configuration:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWMAPCFG?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			for (j = 0; j < len; j++)
			{
				if ((buff[j] == ':') && (buff[j + 1] == ' '))
				{
					j = j + 2;
					if (buff[j] != ' ')
					{
						g_wifi_info.wifi_mode = atoi(&buff[j]);
						j = j + 2;
					}
					if (buff[j] != ' ')
					{
						g_wifi_info.current_ap = atoi(&buff[j]);
					}
					break;
				}
			}
			memset(str_temp, 0, sizeof(str_temp));
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rWiFi.wifi_mode=%u, current_ap=%u", g_wifi_info.wifi_mode,
		   g_wifi_info.current_ap);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_config_mode_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_config_mode_write(uint8_t wifi_mode)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifiW.wifi_mode=%u", wifi_mode);
#endif

	// Check param:
	wifi_config_read();
	if (g_wifi_info.wifi_mode == wifi_mode)
	{
		goto end_proc;
	}

	// Set wifi mode:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%c%u", "AT+CWMAPCFG=\"enablessid2\"", ',', wifi_mode);
		len = sendATCmd(str_temp, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_wifi_info.wifi_mode = wifi_mode;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_config_current_ap_write()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_config_current_ap_write(uint8_t current_ap)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_WRITE_
	printf("\n\rWifiW.ap=%u", current_ap);
#endif

	// Check param:
	wifi_config_read();
	if (g_wifi_info.current_ap == current_ap)
	{
		goto end_proc;
	}

	// Set current_ap:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		snprintf(str_temp, AT_LINE_SZ, "%s%c%u", "AT+CWMAPCFG=\"configselect\"", ',', current_ap);
		len = sendATCmd(str_temp, (char *)"OK", buff, AT_DATA_MAX, AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_wifi_info.current_ap = current_ap;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_WRITE_
	printf("->ret=%u", ret);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : wifi_mac_address_read()
 * Description   :
 * Input         :
 * Output        :
 * Return        : 1: Success, 0: Fail
 *****************************************************************************/
uint8_t wifi_mac_address_read(uint8_t current_ap)
{
	int8_t len = -1;
	char temp[12];
	uint8_t i = 0, j = 0, k = 0;
#ifdef _DB_WRITE_
	printf("\n\rwifi_mac_address_read");
#endif
	// Set current ap:
	wifi_config_current_ap_write(current_ap);

	// Read:
	memset(buff, 0, sizeof(buff));
	memset(str_temp, 0, sizeof(str_temp));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)"AT+CWMACADDR?", (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			k = 0;
			for (j = 0; j < len; j++)
			{
				if (buff[j] == ',')
				{
					temp[0] = buff[j + 1];
					temp[1] = buff[j + 2];
					temp[2] = buff[j + 4];
					temp[3] = buff[j + 5];
					temp[4] = buff[j + 7];
					temp[5] = buff[j + 8];
					temp[6] = buff[j + 10];
					temp[7] = buff[j + 11];
					temp[8] = buff[j + 13];
					temp[9] = buff[j + 14];
					temp[10] = buff[j + 16];
					temp[11] = buff[j + 17];
					memcpy(g_wifi_info.wifi[current_ap].mac_info[k].mac_addr, temp, sizeof(temp));
					k++;
				}
			}
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	for (i = 0; i < g_wifi_info.wifi[current_ap].client_count + 1; i++)
	{
		printf("\n\rmac_addr [%u]=%s", i, g_wifi_info.wifi[current_ap].mac_info[i].mac_addr);
	}
#endif

	return 0;
}

/*****************************************************************************
 * Function Name : wifi_read_all
 * Description   :
 * Input         : None
 * Output        : None
 * Return        : None
 *****************************************************************************/
void wifi_read_all(void)
{
	// clean g_wifi_info struct
	memset(&g_wifi_info, 0, sizeof(g_wifi_info));

	wifi_open_read();
	wifi_config_read();
	wifi_nat_read();

	if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_AP)
	{
		wifi_ssid_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_client_count_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_isolation_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_mode_channel_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_auth_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_broadcast_read(WIFI_CWCMAPCFG_SET_WIFI_0);
	}
	else if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_APAP)
	{
		wifi_ssid_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_client_count_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_isolation_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_mode_channel_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_auth_read(WIFI_CWCMAPCFG_SET_WIFI_0);
		wifi_broadcast_read(WIFI_CWCMAPCFG_SET_WIFI_0);

		wifi_ssid_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_client_count_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_isolation_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_mode_channel_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_auth_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		wifi_broadcast_read(WIFI_CWCMAPCFG_SET_WIFI_1);
	}
	else if (g_wifi_info.wifi_mode == WIFI_CWCMAPCFG_VALUE_STAAP)
	{
		wifi_ssid_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_mac_address_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_client_count_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_dhcp_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_isolation_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_mode_channel_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_auth_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		wifi_broadcast_read(WIFI_CWCMAPCFG_SET_WIFI_2);
		// STA
		wifi_sta_ip_read();
		wifi_sta_cfg_read();
	}
}

/*****************************************************************************
 * Function Name : wifi_sta_ip_read
 * Description   :
 * Input         : None
 * Output        : None
 * Return        : None
 *****************************************************************************/
bool wifi_sta_ip_read(void)
{
	bool ret;
	ret = get_sta_ip(g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_ip, WIFI_STA_IP_MAX);
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_sta_cfg_read
 * Description   :
 * Input         : None
 * Output        : None
 * Return        : None
 *****************************************************************************/
bool wifi_sta_cfg_read(void)
{
	bool ret;
	ret = get_sta_cfg(g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_ssid_ext,
					  g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_password);
	return ret;
}

/*****************************************************************************
 * Function Name : wifi_sta_cfg_write
 * Description   :
 * Input         : None
 * Output        : None
 * Return        : None
 *****************************************************************************/
bool wifi_sta_cfg_write(char *ssid_str, char *psk_value)
{
	bool ret;
	ret = set_sta_cfg(ssid_str, psk_value);
	return ret;
}
