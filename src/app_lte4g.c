/**	@brief implement all functions of Wifi.
	@date
		- 2021_07_23: Create.
*/

#include "LteControl.h"
#include "LedControl.h"

// #define _DB_INIT_
// #define _DB_READ_
// #define _DB_WRITE_

extern void process_simcom_ind_message(simcom_event_e event,void *cb_usr_data);

////////////////////////////////////////////CONSTANT////////////////////////////////////
// Storage:
#define AT_LINE_SZ					128
/////////////////////////////////////////////VARIABLES///////////////////////////////////
char str_temp[128];
char buff[100];

////////////////////////////////////////////LOCAL FUNCTIONS///////////////////////////////

///////////////////////////////////S/////////PUBLIC FUNCTIONS/////////////////////////////
/*****************************************************************************
	* Function Name : lte4g_init
	* Description   : Init all functions of 4G/LTE.
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
void lte4g_init(void)
{
	// Init simcard:
	uim_init();
	
	// Init SMS:
    sms_init((sms_ind_cb_fcn)process_simcom_ind_message);
	
	// Get status of simcard:
	lte4g_simcard_status_get();
	lte4g_network_mode_get();
	lte4g_simcard_iccid_get();
	lte4g_simcard_imsi_get();
	lte4g_simcard_imei_get();
	lte4g_csq_get();	
	lte4g_UE_info_get();
	
#ifdef _DB_INIT_
	printf("\n\rLTE/4G_init.Success");
#endif 
}

/*****************************************************************************
	* Function Name : lte4g_simcard_status_get
	* Description   : 
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
int8_t lte4g_simcard_status_get(void)
{
	int8_t ret = -1;

	// Get all status of SIM Card:
	memset(&g_lte4g_info.sim_status, 0, sizeof(SimCard_Status_type));
	ret = getSimCardStatus(&g_lte4g_info.sim_status);
	
#ifdef _DB_READ_	
	printf("card_status:%d   (0:not exist 1: Exist: 2: UNKNOW)\n", g_lte4g_info.sim_status.card_status);
	printf("app_type:   %d   (0:UNKNOWN 1: SIM 2: USIM 3: RUIM 4: CSIM 5: ISIM)\n",g_lte4g_info.sim_status.app_type);

	printf("\napp_state define: (0:unknown  1: detected      2: pin block          3: puk block)\n");
	printf("app_state define: (4: person check  5:Permanently blocked 6: illgeal 7: Ready)\n");
	printf("app_state:  %d   \n\n", g_lte4g_info.sim_status.app_state);

	printf("\npin_state define: (0: unknown  1: Enabled and not verified 2: Enabled and verified)\n");
	printf("pin_state define: (3: Disabled 4: Blocked                  5:Permanently blocked)\n");
	printf("pin_state:  %d   \n\n", g_lte4g_info.sim_status.pin.pin_state);

	printf("pin_retries:%d\n", g_lte4g_info.sim_status.pin.pin_retries);
	printf("puk_retries:%d\n", g_lte4g_info.sim_status.pin.puk_retries);
#endif
		
	return ret;
}

/*****************************************************************************
	* Function Name : lte4g_simcard_iccid_get
	* Description   : Init all functions of 4G/LTE.
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
int8_t lte4g_simcard_iccid_get(void)
{
	int8_t ret = -1;
	ret = get_iccid(g_lte4g_info.sim_iccid);
#ifdef _DB_READ_
	printf("\n\rsim_iccid=%u", g_lte4g_info.sim_iccid);
#endif
	return ret;
}

/*****************************************************************************
	* Function Name : lte4g_simcard_imsi_get
	* Description   : .
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
int8_t lte4g_simcard_imsi_get(void)
{
	int8_t ret = -1;
	ret = get_imsi(g_lte4g_info.sim_imsi);
#ifdef _DB_READ_
	printf("\n\rsim_imsi=%s", g_lte4g_info.sim_imsi);
#endif
	return ret;
}

/*****************************************************************************
	* Function Name : lte4g_simcard_imei_get
	* Description   : .
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
int8_t lte4g_simcard_imei_get(void)
{
	int8_t ret = -1;
	ret = get_imei(buff);
#ifdef _DB_READ_
	printf("\n\rsim_imei=%s", g_lte4g_info.sim_imei);
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
uint8_t lte4g_csq_get(void) 
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, m = 0, k = 0;
	
	// Read CSQ:
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++) {
		len = sendATCmd((char*)"AT+CSQ", (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0) {
			for (j = 0; j < len; j++) {
				if ((buff[j] == ':') && (buff[j+1] == ' ')) {
					k = 0;
					for (m = j+2; m < len; m++) {
						if (buff[m] == ',') {
							break;
						} else {
							str_temp[k] = buff[m];
							k++;
						}
					}
					break;
				}
			}
			g_lte4g_info.rssi_4g = atoi(str_temp);
			memset(str_temp, 0, sizeof(str_temp));
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}
	
#ifdef _DB_READ_
	printf("\n\rRSSI: %u", g_lte4g_info.rssi_4g);
#endif
	return ret;
}

/*****************************************************************************
	* Function Name : lte4g_network_mode_get
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 1: Success, 0: Fail
*****************************************************************************/ 
uint8_t lte4g_network_mode_get(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, m = 0, k = 0;
	
	// Read network mode:
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++) {
		len = sendATCmd((char*)"AT+CNMP?", (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0) {
			for (j = 0; j < len; j++) {
				if ((buff[j] == ':') && (buff[j+1] == ' ')) {
					k = 0;
					for (m = j+2; m < len; m++) {
						if (buff[m] == ' ') {
							break;
						} else {
							str_temp[k] = buff[m];
							k++;
						}
					}
					break;
				}
			}
			g_lte4g_info.network_mode = atoi(str_temp);
			memset(str_temp, 0, sizeof(str_temp));
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_READ_
	printf("\n\rnetwork_mode=%u", g_lte4g_info.network_mode);
#endif
	
	return ret;
}

/*****************************************************************************
	* Function Name : lte4g_network_mode_set
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 1: Success, 0: Fail
*****************************************************************************/ 
uint8_t lte4g_network_mode_set(uint8_t network_mode)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
	
#ifdef _DB_WRITE_
	printf("\n\rNetwork mode=%u", network_mode);
#endif	
	
	// Check param:
	lte4g_network_mode_get();
	if (g_lte4g_info.network_mode == network_mode) {
		goto end_proc;
	}

	// Set open/close wifi
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++) {
		memset(str_temp, 0, sizeof(str_temp));
		snprintf(str_temp, AT_LINE_SZ, "%s%u","AT+CNMP=", network_mode);
		len = sendATCmd(str_temp, (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0) {
			g_lte4g_info.network_mode = network_mode;
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
	* Function Name : lte4g_network_mode_get
	* Description   : 
	* Input         : 
	* Output        : 
	* Return        : 1: Success, 0: Fail
*****************************************************************************/ 
uint8_t lte4g_UE_info_get(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0, j = 0, k = 0, m = 0;

	char *p;
    char temp[20];
	
	// Read network mode:
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++) {
		len = sendATCmd((char*)"AT+CPSI?", (char*)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		#ifdef _DB_READ_
			printf("\nbuff=%s, len = %u", buff, len);
		#endif
		if (len > 0) {
			p = strtok(&buff[9], ",");
			if (strcmp(p, "NO SERVICE") == 0)
			{
				strcpy(g_lte4g_info.system_mode, p);
				p = strtok(NULL, ",");
				strcpy(g_lte4g_info.operation_mode, p);
				g_lte4g_info.operation_mode[strlen(p)-2] = 0;
			}
			else
			{
				strcpy(g_lte4g_info.system_mode, p);
				for (uint8_t i = 0; i < 7; i++)
				{
					p = strtok(NULL, ",");
					switch (i)
					{
					case 0:
						strcpy(g_lte4g_info.operation_mode, p);
						break;
					case 1:
						strcpy(temp, p);
						for (uint8_t j = 0; j < strlen(temp); j++)
						{
							if (temp[j] == '-')
							{
								temp[j] = 0;
								strcpy(g_lte4g_info.mobile_country_code, temp);
								strcpy(g_lte4g_info.mobile_network_code, &temp[j + 1]);
							}
						}
						break;
					case 2:
						strcpy(g_lte4g_info.location_area_code, p);
						break;
					case 3:
						strcpy(g_lte4g_info.service_cell_id, p);
						break;
					case 4:
						strcpy(g_lte4g_info.physical_cell_id, p);
						break;
					case 5:
						strcpy(g_lte4g_info.freq_band, p);
						break;
					case 6:
						strcpy(g_lte4g_info.afrcn, p);
						break;
					}
				}
			}
		}
		memset(buff, 0, sizeof(buff));
	}
	
	return ret;
}

/*****************************************************************************
	* Function Name : lte4g_sms_send
	* Description   : .
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
int8_t lte4g_sms_send(char *phoneNumber, uint8_t *content)
{
	int8_t ret = -1;
	send_message(SMS_MSG_FORMAT_TEXT_ASCII, phoneNumber, content, sizeof(content));
	return ret;
}

/*****************************************************************************
	* Function Name : lte4g_sms_received
	* Description   : .
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
void lte4g_sms_received(uint8_t *data)
{
	sms_info_type sms_info;	
	memcpy((void *)&sms_info, data, sizeof(sms_info)); 
	
#ifdef _DB_READ_
	printf("\n-----------receive message ---------------------\n");
	printf("is_concate = %d\n", sms_info.is_concate);
	printf("msg_ref = %d\n", sms_info.msg_ref);
	printf("seq_num = %d\n", sms_info.seq_num);
	printf("total_sm = %d\n", sms_info.total_sm);
	printf("format = %d\n", sms_info.format);
	printf("message_len = %d\n", sms_info.message_len);
	printf("address=%s\n",sms_info.source_address);	
	if (sms_info.format == SMS_MSG_FORMAT_TEXT_UNICODE) {
		printf("content:");
		uint8_t i = 0;
		for (i=0; i < sms_info.message_len; i++)
			printf("%02X", sms_info.message_content[i]);
		printf("\n");
	} else {
		printf("content:%s\n", sms_info.message_content);
	}
#endif
}

/*****************************************************************************
	* Function Name : lte4g_sms_received
	* Description   : .
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/ 
void lte4g_network_received(uint8_t *data)
{
	network_info_type network_info;	
	memcpy((void *)&network_info, data, sizeof(network_info));
#ifdef _DB_READ_
	printf("network_info: register=%d, cs=%d, ps=%d,radio_if=%d\n",
			network_info.registration_state,
			network_info.cs_attach_state,
			network_info.ps_attach_state,
			network_info.radio_if_type);
#endif
}

/*****************************************************************************
	* Function Name : lte4g_read_all
	* Description   : 
	* Input         : None
	* Output        : None
	* Return        : None
*****************************************************************************/
void lte4g_read_all(void)
{
	lte4g_simcard_status_get();
	lte4g_network_mode_get();
	lte4g_simcard_iccid_get();
	lte4g_simcard_imsi_get();
	lte4g_csq_get();	
	lte4g_UE_info_get();
}

