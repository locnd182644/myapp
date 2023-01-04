/**	@brief implement all functions of ethernet.
	@date
		- 2021_07_23: Create.
*/
#include "EthControl.h"
#include "LedControl.h"
#include "DMSControl.h"
#include "device_management_service_v01.h"

// #define _DB_ETHERNET_
// #define _DB_INIT_
/////////////////////////////////VARIABLES/////////////////////////////////////
char buff[100];
////////////////////////////////CONSTANTS/////////////////////////////////////
// install driver;
#define ETH_INSTALL_DRIVER_BCM898XX_CFG "AT+CLANCTRL=1,1"
#define ETH_INSTALL_DRIVER_AT803X_CFG "AT+CLANCTRL=1,2"

// unintall driver:
#define ETH_UNINSTALL_DRIVER_BCM898XX_CFG "AT+CLANCTRL=0,1"
#define ETH_UNINSTALL_DRIVER_AT803X_CFG "AT+CLANCTRL=0,2"

// Set Static Ip mode
#define ETH_STATIC_IP_CFG "AT+CLANMODE=2"

// Set Mac address:
#define ETH_MAC_ADDRESS_CFG "AT+CLANCTRL=2"
#define ETH_BRING_UP_CFG "AT+CLANCTRL=4"
#define ETH_BRING_DOWN_CFG "AT+CLANCTRL=5"

#define ETH_IP_DEFAULT "192.168.1.1"
#define ETH_NETMASK_DEFAULT "255.255.255.0"
////////////////////////////////LOCAL FUNCTION////////////////////////////////
/*****************************************************************************
 * Function Name : Install_driver
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
static void Install_driver()
{
	int8_t ret = -1;
	uint8_t i = 0;

#ifdef _DB_ETHERNET_
	printf("\n\rInstall Driver: ");
#endif
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		ret = sendATCmd((char *)ETH_INSTALL_DRIVER_AT803X_CFG, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (ret > 0)
		{
			g_ethernet_info.driver_en = 1;
			g_ethernet_info.driver_type = 2;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}
#ifdef _DB_ETHERNET_
	printf(" ->ret = %u", ret);
#endif
}

/*****************************************************************************
 * Function Name : Uninstall_driver
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
static void Uninstall_driver()
{
	int8_t ret = -1;
	uint8_t i = 0;

#ifdef _DB_ETHERNET_
	printf("\n\rUninstall Driver: ");
#endif
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		ret = sendATCmd((char *)ETH_UNINSTALL_DRIVER_AT803X_CFG, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (ret > 0)
		{
			g_ethernet_info.driver_en = 0;
			g_ethernet_info.driver_type = 2;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}
#ifdef _DB_ETHERNET_
	printf(" ->ret = %u", ret);
#endif
}

/*****************************************************************************
 * Function Name : ethernet_static_ip_mode_set
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
uint8_t ethernet_static_ip_mode_set(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;

#ifdef _DB_ETHERNET_
	printf("\nstatic_ip_mode_set: ");
#endif

	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)ETH_STATIC_IP_CFG, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_ethernet_info.ip_static_en = 1;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}
#ifdef _DB_ETHERNET_
	printf(" ->ret = %u", ret);
#endif

	return ret;
}

/////////////////////////////////PUBLIC FUNCTION/////////////////////////////////////
/*****************************************************************************
 * Function Name : ethernet_init
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
void ethernet_init(void)
{
	// Install driver:
	Install_driver();

	// set Mac address:
	ethernet_mac_address_set();

	// set Bring up ethernet:
	ethernet_bringup_set();

	// Check g_ethernet_info.ip_static_en == 1
	// set ip:
	// ethernet_ip_addr_set(ETH_IP_DEFAULT, ETH_NETMASK_DEFAULT);

#ifdef _DB_INIT_
	printf("\n\rEthernet Init Success");
#endif
}

/*****************************************************************************
 * Function Name : ethernet_mac_address_set
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
uint8_t ethernet_mac_address_set(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
#ifdef _DB_ETHERNET_
	printf("\n\rmac_address_set:");
#endif

	// set Mac address:
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)ETH_MAC_ADDRESS_CFG, (char *)"OK", buff, sizeof(buff), AT_TIMEOUT_MAX);
		if (len > 0)
		{
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

#ifdef _DB_ETHERNET_
	printf(" ->ret=%u", ret);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : ethernet_ip_addr_set
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
uint8_t ethernet_ip_addr_set(char *ip_addr, char *netmask)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;
	char str[100];

#ifdef _DB_ETHERNET_
	printf("\n\rip_addr_set: ");
#endif
	if ((strcmp(ip_addr, g_ethernet_info.ip_addr) == 0) &&
		(strcmp(netmask, g_ethernet_info.netmask) == 0))
	{
		goto end_proc;
	}
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		sprintf(str, "%s%s%s%s%c%s%s%s", "AT+CLANCTRL=3,", "\"", ip_addr, "\"", ',', "\"", netmask, "\"");
		len = sendATCmd(str, (char *)"OK", buff, AT_DATA_MAX, AT_TIMEOUT_MAX);
		if (len > 0)
		{
			strcpy(g_ethernet_info.ip_addr, ip_addr);
			strcpy(g_ethernet_info.netmask, netmask);
			g_ethernet_info.ip_static_en = 1;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_ETHERNET_
	printf("->ret = %u", ret);
#endif

	return ret;
}

/*****************************************************************************
 * Function Name : ethernet_bringup_set
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
uint8_t ethernet_bringup_set(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;

#ifdef _DB_ETHERNET_
	printf("\n\rethernet_bringup_set: ");
#endif
	if (g_ethernet_info.bring_updown_en == 1)
	{
		goto end_proc;
	}
	// Send AT
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)ETH_BRING_UP_CFG, (char *)"OK", buff, AT_DATA_MAX, AT_TIMEOUT_MAX);
		if (len > 0)
		{
			g_ethernet_info.bring_updown_en = 1;
			ret = 1;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_ETHERNET_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : ethernet_bringup_set
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
uint8_t ethernet_bringdown_set(void)
{
	int8_t len = -1;
	uint8_t ret = 0;
	uint8_t i = 0;

#ifdef _DB_ETHERNET_
	printf("\n\rethernet_bringdown_set: ");
#endif
	if (g_ethernet_info.bring_updown_en == 0)
	{
		goto end_proc;
	}
	// Send AT
	memset(buff, 0, sizeof(buff));
	for (i = 0; i < AT_RETRY_MAX; i++)
	{
		len = sendATCmd((char *)ETH_BRING_DOWN_CFG, (char *)"OK", buff, AT_DATA_MAX, AT_TIMEOUT_MAX);
		if (len > 0)
		{
			ret = 1;
			g_ethernet_info.bring_updown_en = 0;
			break;
		}
		memset(buff, 0, sizeof(buff));
	}

end_proc:
#ifdef _DB_ETHERNET_
	printf("->ret=%u", ret);
#endif
	return ret;
}

/*****************************************************************************
 * Function Name : ethernet_reset_factory();
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
uint8_t ethernet_reset_factory(void)
{

#ifdef _DB_ETHERNET_
	printf("\n\rethernet_reset_factory: ");
#endif

	Install_driver();
	ethernet_mac_address_set();
	ethernet_bringup_set();
	return 1;
}

/*****************************************************************************
 * Function Name : ethernet_manager
 * Description   :
 * Input         :
 * Output        :
 * Return        :
 *****************************************************************************/
void ethernet_manager(void)
{
}
