#include "app_serverinf.h"
#include "app_timer.h"
#include <netinet/in.h>

#define _WIFI_TEST_
#define _LTE4G_TEST_
#define _ETHERNET_TEST_
#define _GPS_TEST_
#define _TIMER_TEST_

#define _TCP_DEBUG_
#define _CFG_DEBUG_

////////////////////////////////////////// VARIABLES ///////// //////////////////////////////////
#define MAX_BUFF_LEN 2048
#define STORE_LINE_SZ 2048
#define SBUF_SZ 64

static char json_str[STORE_LINE_SZ] = {0}; // Buff to send
static char buff[MAX_BUFF_LEN] = {0};   // Buff to receive
char g_imei[IMEI_MAX] = {0};
pthread_t recv_thr_id, send_thr_id;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int socketfd;

/* Timer ID of connect_timer */
timer_t g_connect_timer_id;

/* Flag of Configuration */
bool g_cfg_flg = 0;
uint8_t g_channel_wifi_flg = 0;

/* Flag of TCP Send all data */
bool g_busy_send_all_tcp_flg = 0;

/* Flag of TCP Connect */
bool g_connected_tcp_flg = 0;
bool g_connecting_tcp_flg = 0;

////////////////////////////////////////// LOCAL FUNCTION ////////////////////////////////////

/////////////////////////// CONNECT TCP //////////////////////
int TCP_connect(char *serverIP, int serverPort)
{
	// Close socket:
	close(socketfd);

	// Create Socket:
	if ((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
#ifdef _TCP_DEBUG_
		printf("Create socket fail \n");
#endif
		return -1;
	}
	// Connect Server:
	struct sockaddr_in socketaddr;
	memset(&socketaddr, 0, sizeof(socketaddr));
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = htons(serverPort);
	socketaddr.sin_addr.s_addr = inet_addr(serverIP);
	if (connect(socketfd, (struct sockaddr *)&socketaddr, sizeof(socketaddr)) == -1)
	{
#ifdef _TCP_DEBUG_
		printf("Connect to server fail\n");
#endif
		// close(socketfd);
		return -1;
	}
	return 1;
}
void *timer_connect(union sigval v)
{
	if (TCP_connect(SERVER_IP_DEFAULT, SERVER_PORT_DEFAULT) == 1)
	{
		printf("\nConnect to server -> success\n");
		g_connected_tcp_flg = 1;
		g_connecting_tcp_flg = 0;
		TCP_ping();
		// Stop timer connect
		app_stopTimer(g_connect_timer_id);
	}
	else
		printf("\nConnect to server -> failure\n");
}

/////////////////////////// RECEIVE DATA /////////////////////
int TCP_receive(void)
{
#ifdef _TCP_DEBUG_
	printf("recv session new\n");
#endif
	if (g_connecting_tcp_flg == 1)
	{
		printf("\nConnecting to server ... \n");
		return -1;
	}

	int length = 0;
	// Receive data:
	struct timeval timeout = {RECEIVE_TIMEOUT, 0};
	setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	memset(buff, 0, sizeof(buff));
	length = recv(socketfd, buff, sizeof(buff) - 1, 0);
	if (length > 0)
	{
#ifdef _TCP_DEBUG_
		printf("\nrecv msg: %s\n", buff);
#endif
		return 1;
	}
	else if (length <= 0)
	{
#ifdef _TCP_DEBUG_
		printf("\nrecv fail data\n");
#endif
		g_connected_tcp_flg = 0;

#ifdef _TIMER_TEST_
// Re-Connect Server
#ifdef _TCP_DEBUG_
		printf("\nInit Timer_Connect Server -> success\n");
#endif
		g_connect_timer_id = app_startTimer(CONNECT_TIME, (timer_ind_cb_fcn)timer_connect);
		g_connecting_tcp_flg = 1;
#endif
		return 0;
	}
}
int TCP_handler(void)
{
	/* Handler messenger from Server
		- Check data
			1. Dữ liệu của Thiết bị -> nhận (Check Imei)
			2. Dữ liệu thuộc loại nào -> struct loại đó (Check Index)
		- Cấu hình thiết bị
	*/
#ifdef _TCP_DEBUG_
	printf("In TCP_handler()\n");
#endif
	json_read_object(buff, check_attrs, NULL);
#ifdef _TCP_DEBUG_
	printf("IMEI: %s\n", check_info.Imei);
	printf("Infor Type: %d\n", check_info.Index);
#endif

	// Check the device's data ?
	if (strncmp(check_info.Imei, g_imei, IMEI_MAX) == 0)
	{
		switch (check_info.Index)
		{
		case WIFI_INFOR:
			wifi_handler();
			break;

		case LTE_INFOR:
			lte4g_handler();
			break;

		case GPS_INFOR:
			gps_handler();
			break;

		case ETHERNET_INFOR:
			ethernet_handler();
			break;
		}
	}
	memset(buff, 0, sizeof(buff));
}

////////////////////////// SEND DATA //////////////////////////
int TCP_send(char *send_data, uint32_t send_len)
{
	if (g_connected_tcp_flg == 0)
		return -1;
	int length;
	/* Send */
	length = send(socketfd, send_data, send_len, 0);
	if (length > 0)
	{
		printf("Send -> success\n");
		return 1;
	}
	else if (length <= 0)
	{
#ifdef _TCP_DEBUG_
		printf("Send -> fail\n");
#endif
		g_connected_tcp_flg = 0;

		return 0;
	}
}
int TCP_ping()
{
	char sbuf[SBUF_SZ];
	memset(json_str, 0, sizeof(json_str));
#ifdef _TCP_DEBUG_
	printf("\nCreating json Ping data ...\n");
#endif
	snprintf(json_str, STORE_LINE_SZ, "{\"Imei\":\"%s\",", g_imei);
	snprintf(sbuf, SBUF_SZ, "\"FlagConfig\":2}");
	strcat(json_str, sbuf);
	int length;
	/* Send */
	length = send(socketfd, json_str, strlen(json_str), 0);
	if (length > 0)
	{
#ifdef _TCP_DEBUG_
		printf("Send Ping -> success\n");
#endif
		return 1;
	}
	else if (length <= 0)
	{
#ifdef _TCP_DEBUG_
		printf("Send Ping -> fail\n");
#endif
		g_connected_tcp_flg = 0;

		return 0;
	}
}
void *timer_send_data_12h(union sigval v)
{
#ifdef _TCP_DEBUG_
	printf("\nHandler TCP timer send data\n");
#endif
	g_busy_send_all_tcp_flg = 1;
}

////////////////////////////////////////// THREAD ////////////////////////////////////
void *TCP_send_Thr(void *data)
{
#ifdef _TIMER_TEST_

#ifdef _TCP_DEBUG_
	printf("Init Timer_send_data -> success\n");
#endif

	app_startTimer(SEND_DATA_TIME, (timer_ind_cb_fcn)timer_send_data_12h);

#endif

	while (1)
	{
		pthread_mutex_lock(&mutex);

		if (g_connected_tcp_flg == 1)
		{
			// sleep(2);
			if (g_cfg_flg == 1)
			{
				{
					feedback_data_send();
					g_cfg_flg = 0;
				}
			}
			if (g_busy_send_all_tcp_flg == 1)
			{

				{
					all_data_send();
					g_busy_send_all_tcp_flg = 0;
				}
			}
		}

		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}
void *TCP_receive_Thr(void *data)
{
	while (1)
	{
		pthread_mutex_lock(&mutex);
		if (TCP_receive() == 1)
		{
			TCP_handler();
		}
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	return NULL;
}

////////////////////////////// WIFI /////////////////////////////
void json_cfg_wifi_create(void)
{
#ifdef _WIFI_TEST_
	uint8_t type_index = WIFI_INFOR;
	char sbuf[SBUF_SZ];
	uint8_t i = 0;
	uint8_t index;
#ifdef _TCP_DEBUG_
	printf("\nCreating json Wifi data ...\n");
#endif
	// Start
	snprintf(json_str, STORE_LINE_SZ, "{\"Imei\":\"%s\",\"FlagConfig\":0,\"Index\":%u,",
			 g_imei, type_index);

	snprintf(sbuf, SBUF_SZ, "\"WifiOpen\":%u,\"WifiMode\":%u,\"CurrentAp\":%u,\"WifiNat\":%u,",
			 g_wifi_info.open, g_wifi_info.wifi_mode, g_wifi_info.current_ap, g_wifi_info.nat_type);

	strcat(json_str, sbuf);

	/*===============================================================================*/
	switch (g_wifi_info.wifi_mode)
	{
	case WIFI_CWCMAPCFG_VALUE_AP:
	case WIFI_CWCMAPCFG_VALUE_APAP:
		index = WIFI_CWCMAPCFG_SET_WIFI_0;
		break;

	case WIFI_CWCMAPCFG_VALUE_STAAP:
		index = WIFI_CWCMAPCFG_SET_WIFI_2;
		break;
	}
	snprintf(sbuf, SBUF_SZ, "\"SsidWifi1\":\"%s\",", g_wifi_info.wifi[index].ssid);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"AuthTypeWifi1\":%u,", g_wifi_info.wifi[index].auth_type);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"EncryptModeWifi1\":%u,", g_wifi_info.wifi[index].encrypt_mode);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"AuthPwdWifi1\":\"%s\",", g_wifi_info.wifi[index].auth_pwd);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"ClientCountWifi1\":%u,", g_wifi_info.wifi[index].client_count);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"BroadCastWifi1\":%u,\"IsolationWifi1\":%u,",
			 g_wifi_info.wifi[index].broadcast,
			 g_wifi_info.wifi[index].isolation);
	strcat(json_str, sbuf);

	snprintf(sbuf, SBUF_SZ, "\"MacAddressWifi1\":\"");
	strcat(json_str, sbuf);
	// MAC Address
	for (i = 0; i < g_wifi_info.wifi[index].client_count + 1; i++)
	{
		if (i == g_wifi_info.wifi[index].client_count)
		{
			snprintf(sbuf, SBUF_SZ, "%s\",", g_wifi_info.wifi[index].mac_info[i].mac_addr);
			strcat(json_str, sbuf);
		}
		else
		{
			snprintf(sbuf, SBUF_SZ, "%s,", g_wifi_info.wifi[index].mac_info[i].mac_addr);
			strcat(json_str, sbuf);
		}
	}
	snprintf(sbuf, SBUF_SZ, "\"ChannelModeWifi1\":%u,\"ChannelWifi1\":%u,",
			 g_wifi_info.wifi[index].channel_mode,
			 g_wifi_info.wifi[index].channel);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpHostIpWifi1\":\"%s\",", g_wifi_info.wifi[index].dhcp_host_ip);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpStartIpWifi1\":\"%s\",", g_wifi_info.wifi[index].dhcp_start_ip);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpEndIpWifi1\":\"%s\",", g_wifi_info.wifi[index].dhcp_end_ip);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpTimeWifi1\":\"%s\",", g_wifi_info.wifi[index].dhcp_time);
	strcat(json_str, sbuf);

	/****************************************/
	snprintf(sbuf, SBUF_SZ, "\"SsidWifi2\":\"%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].ssid);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"AuthTypeWifi2\":%u,", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].auth_type);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"EncryptModeWifi2\":%u,", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].encrypt_mode);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"AuthPwdWifi2\":\"%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].auth_pwd);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"ClientCountWifi2\":%u,", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].client_count);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"BroadCastWifi2\":%u,\"IsolationWifi2\":%u,",
			 g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].broadcast,
			 g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].isolation);
	strcat(json_str, sbuf);

	snprintf(sbuf, SBUF_SZ, "\"MacAddressWifi2\":\"");
	strcat(json_str, sbuf);
	// MAC Address
	for (i = 0; i < g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].client_count + 1; i++)
	{
		if (i == g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].client_count)
		{
			snprintf(sbuf, SBUF_SZ, "%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].mac_info[i].mac_addr);
			strcat(json_str, sbuf);
		}
		else
		{
			snprintf(sbuf, SBUF_SZ, "%s,", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].mac_info[i].mac_addr);
			strcat(json_str, sbuf);
		}
	}
	snprintf(sbuf, SBUF_SZ, "\"ChannelModeWifi2\":%u,\"ChannelWifi2\":%u,",
			 g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].channel_mode,
			 g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].channel);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpHostIpWifi2\":\"%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].dhcp_host_ip);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpStartIpWifi2\":\"%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].dhcp_start_ip);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpEndIpWifi2\":\"%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].dhcp_end_ip);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"DhcpTimeWifi2\":\"%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].dhcp_time);
	strcat(json_str, sbuf);

	/* STA */
	snprintf(sbuf, SBUF_SZ, "\"StaIp\":\"%s\",", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_ip);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"StaSsidExt\":%s,", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_ssid_ext);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"StaSecurity\":%u,", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_security);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"StaProtocol\":%u,", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_protocol);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"StaPassword\":%s", g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_2].sta_password);
	strcat(json_str, sbuf);

	snprintf(sbuf, SBUF_SZ, "}"); // End.
	strcat(json_str, sbuf);

	printf("\n\r ---> Wifi_JsonStr= %s\n", json_str);
#endif
}
uint8_t wifi_set_cfg(void)
{
	uint8_t ret = 0;

#ifdef _CFG_DEBUG_
	printf("\n===========================\n");
	printf("Configuring wifi ...\n");
	fflush(stdout);
#endif

	uint8_t tmp_ret = 0;
	uint8_t wifi_mode;
	uint8_t val1;
	uint8_t val2;
	uint8_t index;

	/*
		Configuration here
	*/
	//=================================================================//

	/********************** Wifi General *************************/
	/* Wifi Open */
	if (strstr(buff, "WifiOpen") != NULL)
	{
		val1 = convert_char_to_int(wifi_config.WifiOpen);
#ifdef _CFG_DEBUG_
		printf("\nWifi Open = %d\n", val1);
#endif
		tmp_ret = wifi_open_write(val1);
		ret |= tmp_ret;
	}

	/* Wifi Mode */
	if (strstr(buff, "WifiMode") != NULL)
	{
		val1 = convert_char_to_int(wifi_config.WifiMode);
		wifi_mode = val1;
#ifdef _CFG_DEBUG_
		printf("\nWifi Mode = %d\n", val1);
#endif
		tmp_ret = wifi_config_mode_write(val1);
		ret |= tmp_ret;
	}

	/* Wifi Nat */
	if (strstr(buff, "WifiNat") != NULL)
	{
		val1 = convert_char_to_int(wifi_config.WifiNat);
#ifdef _CFG_DEBUG_
		printf("\nWifi Nat = %d\n", val1);
#endif
		tmp_ret = wifi_nat_write(val1);
		ret |= tmp_ret;
	}

	/********************** Wifi Specific *************************/
	switch (wifi_mode)
	{
	case WIFI_CWCMAPCFG_VALUE_AP:

	case WIFI_CWCMAPCFG_VALUE_APAP:
		index = WIFI_CWCMAPCFG_SET_WIFI_0;
		break;

	case WIFI_CWCMAPCFG_VALUE_STAAP:
		index = WIFI_CWCMAPCFG_SET_WIFI_2;
		break;
	}

#ifdef _CFG_DEBUG_
	switch (wifi_mode)
	{
	case WIFI_CWCMAPCFG_VALUE_AP:
		printf("\n... AP mode ...\n");
		break;
	case WIFI_CWCMAPCFG_VALUE_APAP:
		printf("\n... AP-AP mode ...\n");
		break;
	case WIFI_CWCMAPCFG_VALUE_STAAP:
		printf("\n... STA-AP mode ...\n");
		break;
	}
#endif

	if (strstr(buff, "SsidWifi1") != NULL)
	{
		tmp_ret = wifi_ssid_write(index, wifi_config.SsidWifi1);
		ret |= tmp_ret;
	}
	if (strstr(buff, "BroadCastWifi1") != NULL)
	{
		val1 = convert_char_to_int(wifi_config.BroadCastWifi1);
		tmp_ret = wifi_broadcast_write(index, val1);
		ret |= tmp_ret;
	}
	if ((strstr(buff, "AuthPwdWifi1") != NULL) ||
		(strstr(buff, "AuthTypeWifi1") != NULL) ||
		(strstr(buff, "EncryptModeWifi1") != NULL))
	{
		val1 = convert_char_to_int(wifi_config.AuthTypeWifi1);
		val2 = convert_char_to_int(wifi_config.EncryptModeWifi1);
		tmp_ret = wifi_auth_write(index,
								  wifi_config.AuthPwdWifi1,
								  val1,
								  val2);
		ret |= tmp_ret;
	}
	if ((strstr(buff, "ChannelModeWifi1") != NULL) ||
		(strstr(buff, "ChannelWifi1") != NULL))
	{
		val1 = convert_char_to_int(wifi_config.ChannelModeWifi1);
		val2 = convert_string_to_int(&wifi_config.ChannelWifi1);
#ifdef _CFG_DEBUG_
		printf("\nMode1:Channel1 = %d:%d\n", val1, val2);
#endif
		tmp_ret = wifi_mode_channel_write(index, val1, val2);
		if (tmp_ret == 1)
		{
			g_channel_wifi_flg = 1;
		}
		ret |= tmp_ret;
	}
	if (strstr(buff, "IsolationWifi1") != NULL)
	{
		val1 = convert_char_to_int(wifi_config.IsolationWifi1);
		tmp_ret = wifi_isolation_write(index, val1);
		ret |= tmp_ret;
	}
	/*************************************************/
	if (strstr(buff, "SsidWifi2") != NULL)
	{
		tmp_ret = wifi_ssid_write(WIFI_CWCMAPCFG_SET_WIFI_1,
								  wifi_config.SsidWifi2);
		ret |= tmp_ret;
	}
	if (strstr(buff, "BroadCastWifi2") != NULL)
	{
		val1 = convert_char_to_int(wifi_config.BroadCastWifi2);
		tmp_ret = wifi_broadcast_write(WIFI_CWCMAPCFG_SET_WIFI_1, val1);
		ret |= tmp_ret;
	}
	if ((strstr(buff, "AuthPwdWifi2") != NULL) ||
		(strstr(buff, "AuthTypeWifi2") != NULL) ||
		(strstr(buff, "EncryptModeWifi2") != NULL))
	{
		val1 = convert_char_to_int(wifi_config.AuthTypeWifi2);
		val2 = convert_char_to_int(wifi_config.EncryptModeWifi2);
		tmp_ret = wifi_auth_write(WIFI_CWCMAPCFG_SET_WIFI_1,
								  wifi_config.AuthPwdWifi2, val1, val2);
		ret |= tmp_ret;
	}
	if ((strstr(buff, "ChannelModeWifi2") != NULL) ||
		(strstr(buff, "ChannelWifi2") != NULL))
	{
		val1 = convert_char_to_int(wifi_config.ChannelModeWifi2);
		val2 = convert_string_to_int(&wifi_config.ChannelWifi2);
		tmp_ret = wifi_mode_channel_write(WIFI_CWCMAPCFG_SET_WIFI_1, val1, val2);
		if (tmp_ret == 1)
		{
			g_channel_wifi_flg = 2;
		}
		ret |= tmp_ret;
	}
	if (strstr(buff, "IsolationWifi2") != NULL)
	{
		val1 = convert_char_to_int(wifi_config.IsolationWifi2);
		tmp_ret = wifi_isolation_write(WIFI_CWCMAPCFG_SET_WIFI_1, val1);
		ret |= tmp_ret;
	}

	/* Config STA */
	if ((strstr(buff, "StaSsidExt") != NULL) ||
		(strstr(buff, "StaPassword") != NULL))
	{
		tmp_ret = wifi_sta_cfg_write(wifi_config.StaSsidExt,
									 wifi_config.StaPassword);
		ret |= tmp_ret;
	}

	//=================================================================//

#ifdef _CFG_DEBUG_
	if (ret == 1)
	{
		printf("\n\r-> User configure wifi SUCCESS\n");
		// close(socketfd);
		g_connected_tcp_flg = 0;
#ifdef _TIMER_TEST_
// Re-Connect Server
#ifdef _TCP_DEBUG_
		printf("\nInit Timer_Connect Server -> success\n");
#endif
		g_connect_timer_id = app_startTimer(CONNECT_TIME, (timer_ind_cb_fcn)timer_connect);
		g_connecting_tcp_flg = 1;
#endif
	}
	else
	{
		printf("\n\r-> User configure wifi FAILURE\n");
	}
	printf("===========================\n");
#endif

	return ret;
}
void wifi_handler(void)
{
#ifdef _WIFI_TEST_
#ifdef _TCP_DEBUG_
	printf("In wifi_handler()\n");
#endif
	uint8_t status;
	status = json_read_object(buff, wifi_json_attrs, NULL);
	if (status != 0)
	{
		puts(json_error_string(status));
		return;
	}
#ifdef _TCP_DEBUG_
	printf("=====================================================================================\n");
	printf("IMEI = %s, WifiOpen = %c, WifiMode = %c, CurrentAp = %c, WifiNat = %c\n",
		   wifi_config.Imei,
		   wifi_config.WifiOpen,
		   wifi_config.WifiMode,
		   wifi_config.CurrentAp,
		   wifi_config.WifiNat);
	printf("-------------------------------------------------------------------------------------\n");
	printf("+ Ssid1: %s\n", wifi_config.SsidWifi1);
	printf("+ AuthType1: %c\n", wifi_config.AuthTypeWifi1);
	printf("+ EncryptMode1: %c\n", wifi_config.EncryptModeWifi1);
	printf("+ AuthPwd1: %s\n", wifi_config.AuthPwdWifi1);
	printf("+ ClientCountWifi1: %c\n", wifi_config.ClientCountWifi1);
	printf("+ BroadCast1: %c\n", wifi_config.BroadCastWifi1);
	printf("+ Isolation1: %c\n", wifi_config.IsolationWifi1);
	printf("+ MacAddressWifi1: %s\n", wifi_config.MacAddressWifi1);
	printf("+ ChannelMode1: %c\n", wifi_config.ChannelModeWifi1);
	printf("+ Channel1: %s\n", wifi_config.ChannelWifi1);
	printf("+ DhcpHostIp1: %s\n", wifi_config.DhcpHostIpWifi1);
	printf("+ DhcpStartIp1: %s\n", wifi_config.DhcpStartIpWifi1);
	printf("+ DhcpEndIp1: %s\n", wifi_config.DhcpEndIpWifi1);
	printf("+ DhcpTime1: %s\n", wifi_config.DhcpTimeWifi1);
	printf("-------------------------------------------------------------------------------------\n");
	printf("+ Ssid2: %s\n", wifi_config.SsidWifi2);
	printf("+ AuthType2: %c\n", wifi_config.AuthTypeWifi2);
	printf("+ EncryptMode2: %c\n", wifi_config.EncryptModeWifi2);
	printf("+ AuthPwd2: %s\n", wifi_config.AuthPwdWifi2);
	printf("+ ClientCountWifi2: %c\n", wifi_config.ClientCountWifi2);
	printf("+ BroadCast2: %c\n", wifi_config.BroadCastWifi2);
	printf("+ Isolation2: %c\n", wifi_config.IsolationWifi2);
	printf("+ MacAddressWifi2: %s\n", wifi_config.MacAddressWifi2);
	printf("+ ChannelMode2: %c\n", wifi_config.ChannelModeWifi2);
	printf("+ Channel2: %s\n", wifi_config.ChannelWifi2);
	printf("+ DhcpHostIp2: %s\n", wifi_config.DhcpHostIpWifi2);
	printf("+ DhcpStartIp2: %s\n", wifi_config.DhcpStartIpWifi2);
	printf("+ DhcpEndIp2: %s\n", wifi_config.DhcpEndIpWifi2);
	printf("+ DhcpTime2: %s\n", wifi_config.DhcpTimeWifi2);
	printf("-------------------------------------------------------------------------------------\n");
	printf("+ StaIp: %s\n", wifi_config.StaIp);
	printf("+ StaSsidExt: %s\n", wifi_config.StaSsidExt);
	printf("+ StaSecurity: %c\n", wifi_config.StaSecurity);
	printf("+ StaProtocol: %c\n", wifi_config.StaProtocol);
	printf("+ StaPassword: %s\n", wifi_config.StaPassword);
	printf("=====================================================================================\n");
#endif

	/* Config Device */

	switch (wifi_set_cfg())
	{
	case 0:
		strncpy(feedback_info.status, WIFI_FAILURE, STATUS_LEN_MAX);
		break;

	case 1:
		strncpy(feedback_info.status, WIFI_SUCCESS, STATUS_LEN_MAX);
		break;
	}

	g_cfg_flg = 1;

#endif
}
uint8_t wifi_data_send(void)
{
	uint8_t ret = 0;
#ifdef _WIFI_TEST_
#ifdef _TCP_DEBUG_
	printf("\n===========wifi_data_send=============\n");
#endif

	wifi_read_all();

#ifdef _TCP_DEBUG_
	printf("\nWifi read all -> success\n");
#endif

	memset(json_str, 0, sizeof(json_str));
	json_cfg_wifi_create();
#ifdef _TCP_DEBUG_
	printf("\nCreate wifi Json -> success\n");
#endif

	ret = TCP_send(json_str, strlen(json_str));
#ifdef _TCP_DEBUG_
	printf("======================================\n");
#endif
#endif
	return ret;
}

//////////////////////////// LTE4G //////////////////////////////
void json_cfg_lte4g_create(void)
{
#ifdef _LTE4G_TEST_
	uint8_t type_index = LTE_INFOR;
	char sbuf[SBUF_SZ];
#ifdef _TCP_DEBUG_
	printf("\nCreating json Lte/4G data ...\n");
#endif
	// Start
	snprintf(json_str, STORE_LINE_SZ, "{\"Imei\":\"%s\",\"FlagConfig\":0,\"Index\":%u,",
			 g_imei, type_index);
	snprintf(sbuf, SBUF_SZ, "\"CardStatus\":%u,\"AppType\":%u,", g_lte4g_info.sim_status.card_status, g_lte4g_info.sim_status.app_type);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"AppState\":%u,\"Pin\":%u,", g_lte4g_info.sim_status.app_state, g_lte4g_info.sim_status.pin);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"SimIccid\":\"%s\",", g_lte4g_info.sim_iccid);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"SimImsi\":\"%s\",", g_lte4g_info.sim_imsi);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"SystemMode\":\"%s\",", g_lte4g_info.system_mode);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"OperationMode\":\"%s\",", g_lte4g_info.operation_mode);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"MobileCountryCode\":\"%s\",", g_lte4g_info.mobile_country_code);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"MobileNetworkCode\":\"%s\",", g_lte4g_info.mobile_network_code);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"LocationAreaCode\":\"%s\",", g_lte4g_info.location_area_code);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"ServiceCellId\":\"%s\",", g_lte4g_info.service_cell_id);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"FreqBand\":\"%s\",", g_lte4g_info.freq_band);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Current4GData\":\"%s\",", g_lte4g_info.current_4g_data);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Afrcn\":\"%s\",", g_lte4g_info.afrcn);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"PhoneNumber\":\"%s\",", g_lte4g_info.phone_number);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Rssi4G\":%u,", g_lte4g_info.rssi_4g);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"NetworkMode\":%u", g_lte4g_info.network_mode);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "}"); // End.
	strcat(json_str, sbuf);

	printf("\n\r ---> Lte_JsonStr= %s\n", json_str);
#endif
}
uint8_t lte4g_set_cfg(void)
{
	uint8_t ret = 0;

#ifdef _CFG_DEBUG_

	printf("\nConfiguring lte ...\n");
	fflush(stdout);
#endif

	uint8_t tmp_ret = 0;
	uint8_t val;

	/*
		Configuration here
	*/
	//===================================================//
	if (strstr(buff, "NetworkMode") != NULL)
	{
		val = atoi(&lte4g_config.NetworkMode);
		tmp_ret = lte4g_network_mode_set(val);
		ret |= tmp_ret;
	}
	//===================================================//

#ifdef _CFG_DEBUG_
	if (ret == 1)
	{
		printf("\n\r-> User configure lte SUCCESS\n");
	}
	else
	{
		printf("\n\r-> User configure lte FAILURE\n");
	}
#endif

	return ret;
}
void lte4g_handler(void)
{
#ifdef _LTE4G_TEST_
#ifdef _TCP_DEBUG_
	printf("In lte4g_handler()\n");
#endif
	uint8_t status;
	status = json_read_object(buff, lte4g_json_attrs, NULL);
	if (status != 0)
	{
		puts(json_error_string(status));
		return;
	}
#ifdef _TCP_DEBUG_
	printf("=====================================================================================\n");
	printf("IMEI = %s\n", lte4g_config.Imei);
	printf("+ CardStatus: %d\n", lte4g_config.CardStatus);
	printf("+ AppType: %d\n", lte4g_config.AppType);
	printf("+ AppState: %d\n", lte4g_config.AppState);
	printf("+ Pin: %d\n", lte4g_config.Pin);
	printf("+ SimIccid: %s\n", lte4g_config.SimIccid);
	printf("+ SimImsi: %s\n", lte4g_config.SimImsi);
	printf("+ SystemMode: %s\n", lte4g_config.SystemMode);
	printf("+ OperationMode: %s\n", lte4g_config.OperationMode);
	printf("+ MobileCountryCode: %s\n", lte4g_config.MobileCountryCode);
	printf("+ MobileNetworkCode: %s\n", lte4g_config.MobileNetworkCode);
	printf("+ LocationAreaCode: %s\n", lte4g_config.LocationAreaCode);
	printf("+ ServiceCellId: %s\n", lte4g_config.ServiceCellId);
	printf("+ FreqBand: %s\n", lte4g_config.FreqBand);
	printf("+ Current4GData: %s\n", lte4g_config.Current4GData);
	printf("+ Afrcn: %s\n", lte4g_config.Afrcn);
	printf("+ PhoneNumber: %s\n", lte4g_config.PhoneNumber);
	printf("+ FreqBand: %d\n", lte4g_config.Rssi4G);
	printf("+ SimImsi: %d\n", lte4g_config.NetworkMode);
	printf("=====================================================================================\n");
#endif

	/* Config Device */

	switch (lte4g_set_cfg())
	{
	case 0:
		strncpy(feedback_info.status, LTE_FAILURE, STATUS_LEN_MAX);
		break;

	case 1:
		strncpy(feedback_info.status, LTE_SUCCESS, STATUS_LEN_MAX);
		break;
	}

	g_cfg_flg = 1;

#endif
}
uint8_t lte4g_data_send(void)
{
	uint8_t ret = 0;
#ifdef _LTE4G_TEST_
#ifdef _TCP_DEBUG_
	printf("\n===========LTE_data_send=============\n");
#endif
	lte4g_read_all();

#ifdef _TCP_DEBUG_
	printf("\nLTE4G read all -> success\n");
#endif

	memset(json_str, 0, sizeof(json_str));
	json_cfg_lte4g_create();
#ifdef _TCP_DEBUG_
	printf("\nCreate Lte Json -> success\n");
#endif
	ret = TCP_send(json_str, strlen(json_str));
#ifdef _TCP_DEBUG_
	printf("======================================\n");
#endif
#endif
	return ret;
}

//////////////////////////// ETHERNET ///////////////////////////
void json_cfg_ethernet_create(void)
{
#ifdef _ETHERNET_TEST_
	uint8_t type_index = ETHERNET_INFOR;
	char sbuf[SBUF_SZ];
#ifdef _TCP_DEBUG_
	printf("\nCreating json Ethernet data ...\n");
#endif
	// Start
	snprintf(json_str, STORE_LINE_SZ, "{\"Imei\":\"%s\",\"FlagConfig\":0,\"Index\":%u,",
			 g_imei, type_index);
	snprintf(sbuf, SBUF_SZ, "\"DriverEn\":%u,\"DriverType\":%u,",
			 g_ethernet_info.driver_en, g_ethernet_info.driver_type);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"BringUpdownEn\":%u,\"IpStaticEn\":%u,",
			 g_ethernet_info.bring_updown_en, g_ethernet_info.ip_static_en);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"IpAddr\":\"%s\",", g_ethernet_info.ip_addr);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Netmask\":\"%s\"", g_ethernet_info.netmask);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "}"); // End.
	strcat(json_str, sbuf);

	printf("\n\r ---> Ethernet_JsonStr= %s\n", json_str);
#endif
}
uint8_t ethernet_set_cfg(void)
{
	uint8_t ret = 0;

#ifdef _CFG_DEBUG_
	printf("\nConfiguring ethernet ...\n");
	fflush(stdout);
#endif

	uint8_t tmp_ret = 0;
	uint8_t val;

	/*
		Configuration here
	*/
	//===================================================//

	if (strstr(buff, "BringUpdownEn") != NULL)
	{
		val = atoi(&eth_config.BringUpdownEn);
		switch (val)
		{
		case 0:
			tmp_ret = ethernet_bringdown_set();
			break;

		case 1:
			tmp_ret = ethernet_bringup_set();
			break;
		}
		ret |= tmp_ret;
	}

	if ((strstr(buff, "IpAddr") != NULL) ||
		(strstr(buff, "Netmask") != NULL))
	{
		tmp_ret = ethernet_ip_addr_set(eth_config.IpAddr, eth_config.Netmask);
		ret |= tmp_ret;
	}

	//===================================================//

#ifdef _CFG_DEBUG_
	if (ret == 1)
	{
		printf("\n\r-> User configure ethernet SUCCESS\n");
	}
	else
	{
		printf("\n\r-> User configure ethernet FAILURE\n");
	}
#endif

	return ret;
}
void ethernet_handler(void)
{
#ifdef _ETHERNET_TEST_

#ifdef _TCP_DEBUG_
	printf("In ethernet_handler()\n");
#endif
	uint8_t status;
	status = json_read_object(buff, ether_json_attrs, NULL);
	if (status != 0)
	{
		puts(json_error_string(status));
		return;
	}
#ifdef _TCP_DEBUG_
	printf("====================================================================================\n");
	printf("IMEI = %s\n", eth_config.Imei);
	printf("+ DriverEn: %d\n", eth_config.DriverEn);
	printf("+ DriverType: %d\n", eth_config.DriverType);
	printf("+ BringUpdownEn: %d\n", eth_config.BringUpdownEn);
	printf("+ IpStaticEn: %d\n", eth_config.IpStaticEn);
	printf("+ IpAddr: %s\n", eth_config.IpAddr);
	printf("+ Netmask: %s\n", eth_config.Netmask);
	printf("=====================================================================================\n");
#endif

	/* Config Device */

	switch (ethernet_set_cfg())
	{
	case 0:
		strncpy(feedback_info.status, ETH_FAILURE, STATUS_LEN_MAX);
		break;

	case 1:
		strncpy(feedback_info.status, ETH_SUCCESS, STATUS_LEN_MAX);
		break;
	}

	g_cfg_flg = 1;

#endif
}
uint8_t ethernet_data_send(void)
{
	uint8_t ret;
#ifdef _ETHERNET_TEST_
#ifdef _TCP_DEBUG_
	printf("\n===========Ethernet_data_send=============\n");
#endif

	// ethernet_read_all();

#ifdef _TCP_DEBUG_
	printf("\nEthernet read all -> success\n");
#endif

	memset(json_str, 0, sizeof(json_str));
	json_cfg_ethernet_create();

#ifdef _TCP_DEBUG_
	printf("\nCreate Ethernet Json -> success\n");
#endif

	ret = TCP_send(json_str, strlen(json_str));

#ifdef _TCP_DEBUG_
	printf("============================================\n");
#endif

#endif
	return ret;
}

//////////////////////////// GPS /////////////////////////////
void json_cfg_gps_create(void)
{
#ifdef _GPS_TEST_
	uint8_t type_index = GPS_INFOR;
	char sbuf[SBUF_SZ];
#ifdef _TCP_DEBUG_
	printf("\nCreating json Gps data ...\n");
#endif
	snprintf(json_str, STORE_LINE_SZ, "{\"Imei\":\"%s\",\"FlagConfig\":0,\"Index\":%u,",
			 g_imei, type_index);
	snprintf(sbuf, SBUF_SZ, "\"Latitude\":%lf,", g_gps_info.latitude);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Longitude\":%lf,", g_gps_info.longitude);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Altitude\":%lf,", g_gps_info.altitude);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Speed\":%f,", g_gps_info.speed);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Bearing\":%f,", g_gps_info.bearing);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Accuracy\":%f,", g_gps_info.accuracy);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Time\":\"%s\"", g_gps_info.time);
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "}"); // End.
	strcat(json_str, sbuf);

	printf("\n\r ---> Gps_JsonStr= %s\n", json_str);
#endif
}
uint8_t gps_set_cfg(void)
{
	uint8_t ret = 0;

#ifdef _CFG_DEBUG_
	printf("\nConfiguring ethernet ...\n");
	fflush(stdout);
#endif

	uint8_t tmp_ret = 0;
	uint8_t val;

	/*
		Configuration here
	*/
	//===================================================//

	//===================================================//

	return ret;
}
void gps_handler(void)
{
#ifdef _GPS_TEST_
#ifdef _TCP_DEBUG_
	printf("In gps_handler()\n");
#endif
	uint8_t status;
	status = json_read_object(buff, gps_json_attrs, NULL);
	if (status != 0)
	{
		puts(json_error_string(status));
		return;
	}
#ifdef _TCP_DEBUG_
	printf("=====================================================================================\n");
	printf("IMEI = %s\n", gps_config.Imei);
	printf("+ Latitude: %lf\n", gps_config.Latitude);
	printf("+ Longitude: %lf\n", gps_config.Longitude);
	printf("+ Altitude: %lf\n", gps_config.Altitude);
	printf("+ Speed: %f\n", gps_config.Speed);
	printf("+ Bearing: %f\n", gps_config.Bearing);
	printf("+ Accuracy: %f\n", gps_config.Accuracy);
	printf("+ Time: %s\n", gps_config.Time);
	printf("=====================================================================================\n");
#endif

	/* Config Device */

	switch (gps_set_cfg())
	{
	case 0:
		strncpy(feedback_info.status, GPS_FAILURE, STATUS_LEN_MAX);
		break;

	case 1:
		strncpy(feedback_info.status, GPS_FAILURE, STATUS_LEN_MAX);
		break;
	}

	g_cfg_flg = 1;

#endif
}
uint8_t gps_data_send(void)
{
	uint8_t ret = 0;
#ifdef _GPS_TEST_
#ifdef _TCP_DEBUG_
	printf("\n===========GPS data_send=============\n");
#endif
	// gps_read_all();

#ifdef _TCP_DEBUG_
	printf("\nGPS read all -> success\n");
#endif

	memset(json_str, 0, sizeof(json_str));
	json_cfg_gps_create();
#ifdef _TCP_DEBUG_
	printf("\nCreate Gps Json -> success\n");
#endif

	ret = TCP_send(json_str, strlen(json_str));

#ifdef _TCP_DEBUG_
	printf("============================================\n");
#endif

#endif
	return ret;
}

//////////////////////////// Feedback data /////////////////////////////
void json_cfg_feedback_create(void)
{
	char sbuf[SBUF_SZ];
	char index;
#ifdef _TCP_DEBUG_
	printf("\nCreating json feedback data ...\n");
#endif
	snprintf(json_str, STORE_LINE_SZ, "{\"Imei\":\"%s\",", g_imei);
	snprintf(sbuf, SBUF_SZ, "\"FlagConfig\":1,");
	strcat(json_str, sbuf);
	snprintf(sbuf, SBUF_SZ, "\"Status\":\"%s\"", feedback_info.status);
	strcat(json_str, sbuf);
	switch (g_channel_wifi_flg)
	{
	case 0:
		break;
	case 1:
		index = g_wifi_info.wifi_mode;
		wifi_mode_channel_read(index);
		snprintf(sbuf, SBUF_SZ, ",\"ChannelModeWifi1\":%u,\"ChannelWifi1\":%u",
				 g_wifi_info.wifi[index].channel_mode,
				 g_wifi_info.wifi[index].channel);
		strcat(json_str, sbuf);
		break;
	case 2:
		wifi_mode_channel_read(WIFI_CWCMAPCFG_SET_WIFI_1);
		snprintf(sbuf, SBUF_SZ, ",\"ChannelModeWifi2\":%u,\"ChannelWifi2\":%u",
				 g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].channel_mode,
				 g_wifi_info.wifi[WIFI_CWCMAPCFG_SET_WIFI_1].channel);
		strcat(json_str, sbuf);
		break;
	}
	g_channel_wifi_flg = 0;
	snprintf(sbuf, SBUF_SZ, "}"); // End.
	strcat(json_str, sbuf);

	printf("\n\r ---> Feedback_JsonStr= %s\n", json_str);
}
uint8_t feedback_data_send(void)
{
	uint8_t ret = 0;
#ifdef _TCP_DEBUG_
	printf("\n===========Feedback_data_send=============\n");
#endif

	memset(json_str, 0, sizeof(json_str));
	json_cfg_feedback_create();

#ifdef _TCP_DEBUG_
	printf("\nCreate feedback Json -> success\n");
#endif

	ret = TCP_send(json_str, strlen(json_str));

#ifdef _TCP_DEBUG_
	printf("======================================\n");
#endif

	return ret;
}

/////////////////////////////////////////// PUBLIC FUNCTION ///////////////////////////////////
void TCP_init(void)
{
	// Get Imei:
	memset(g_imei, 0, sizeof(g_imei));
	getIMEI(g_imei, sizeof(g_imei));
	printf("IMEI: %s\n", g_imei);

	// Connect to Server:
	if (TCP_connect(SERVER_IP_DEFAULT, SERVER_PORT_DEFAULT) == 1)
	{
		g_connected_tcp_flg = 1;
		all_data_send();
	}
	else
	{
		printf("\nCan't connect to Server\n");
		g_busy_send_all_tcp_flg = 1;
	}

	// Create thread:
	pthread_create(&recv_thr_id, NULL, TCP_receive_Thr, NULL); // Creat thread to receive data
	pthread_create(&send_thr_id, NULL, TCP_send_Thr, NULL);	   // Creat thread to send data
}
uint8_t all_data_send(void)
{
	uint8_t ret = 0;
	uint8_t tmp_ret = 0;

	// Send data to Server:
	tmp_ret = wifi_data_send();
	ret |= tmp_ret;
	sleep(1);
	tmp_ret = lte4g_data_send();
	ret |= tmp_ret;
	sleep(1);
	tmp_ret = ethernet_data_send();
	ret |= tmp_ret;
	sleep(1);
	tmp_ret = gps_data_send();
	ret |= tmp_ret;
	sleep(1);

	fflush(stdout);
	return ret;
}

int convert_char_to_int(char c)
{
	int val = 0;
	val += (uint8_t)c - 48;
	return val;
}
int convert_string_to_int(char *str)
{
	int val = 0;
	for (uint8_t i = 0; i < strlen(str); i++)
	{
		val *= 10;
		val += (uint8_t)str[i] - 48;
	}
	return val;
}