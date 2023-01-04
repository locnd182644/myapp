#ifndef _SERVER_INF_CONTROL_H_
#define _SERVER_INF_CONTROL_H_

#include "simcom_common.h"
#include "WiFiControl.h"
#include "ATControl.h"
#include "UIMControl.h"
#include "SMSControl.h"
#include "NASControl.h"
#include "LteControl.h"
#include "EthControl.h"
#include "GPSControl.h"
#include "mjson.h" // JSON
#include <limits.h>

////////////////////////////////////////CONSTANTS//////////////////////////////////
#define SERVER_IP_DEFAULT "192.168.1.34" //"192.168.1.34" //"192.168.1.49" //"192.168.1.23"    //"192.168.1.34"   //"192.168.1.55" //"192.168.137.43"    //"192.168.1.85"        //"103.229.41.235"      //"120.76.100.197"      // "103.199.79.73" //
#define SERVER_PORT_DEFAULT 6666         // 6666          //3022          // 3022               //6666             // 3022         // 6666                //6666                  // 3021                 //10002                 // 9081             //

#define DATA_LEN_MAX 512
#define IMEI_MAX 100

#define CONNECT_TIME 1000 * 5         // 5 second
#define SEND_DATA_TIME 1000 * 60 * 10 // 10 minute
#define RECEIVE_TIMEOUT 60            // 60 second

/* CONFIG STATUS*/
#define WIFI_FAILURE "00"
#define WIFI_SUCCESS "01"
#define LTE_FAILTURE "10"
#define LTE_SUCCESS "11"
#define ETH_FAILURE "20"
#define ETH_SUCCESS "21"
#define GPS_FAILURE "30"
#define GPS_SUCCESS "31"
#define STATUS_LEN_MAX 2

////////////////////////////////////////TYPES/////////////////////////////////////
typedef enum
{
    WIFI_INFOR = 0,
    LTE_INFOR,
    ETHERNET_INFOR,
    GPS_INFOR,
} INFOR_TYPE;

union app_serverinf
{
    struct check_information
    {
        char Imei[IMEI_MAX];
        uint8_t Index;
    };

    struct feedback_information
    {
        // char Imei[IMEI_MAX];
        // uint8_t FlagConfig;
        char status[STATUS_LEN_MAX];
    };

    /* Wifi Data */
    struct wifi_json_t
    {
        char Imei[IMEI_MAX];
        uint8_t Index;

        uint8_t WifiOpen;
        uint8_t WifiMode;
        uint8_t CurrentAp;
        uint8_t WifiNat;

        char SsidWifi1[WIFI_CWSSID_SSID_MAX];
        uint8_t AuthTypeWifi1;
        uint8_t EncryptModeWifi1;
        char AuthPwdWifi1[WIFI_CWAUTH_PASSWORD_MAX];
        uint8_t ClientCountWifi1;
        uint8_t BroadCastWifi1;
        uint8_t IsolationWifi1;
        char MacAddressWifi1[MAC_ADDR_MAX * MAC_ADDR_COUNT + MAC_ADDR_COUNT - 1];
        uint8_t ChannelModeWifi1;
        uint8_t ChannelWifi1;
        char DhcpHostIpWifi1[WIFI_CWDHCP_DHCP_MAX];
        char DhcpStartIpWifi1[WIFI_CWDHCP_DHCP_MAX];
        char DhcpEndIpWifi1[WIFI_CWDHCP_DHCP_MAX];
        char DhcpTimeWifi1[WIFI_CWDHCP_DHCP_MAX];

        char SsidWifi2[WIFI_CWSSID_SSID_MAX];
        uint8_t AuthTypeWifi2;
        uint8_t EncryptModeWifi2;
        char AuthPwdWifi2[WIFI_CWAUTH_PASSWORD_MAX];
        uint8_t ClientCountWifi2;
        uint8_t BroadCastWifi2;
        uint8_t IsolationWifi2;
        char MacAddressWifi2[MAC_ADDR_MAX * MAC_ADDR_COUNT + MAC_ADDR_COUNT - 1];
        uint8_t ChannelModeWifi2;
        uint8_t ChannelWifi2;
        char DhcpHostIpWifi2[WIFI_CWDHCP_DHCP_MAX];
        char DhcpStartIpWifi2[WIFI_CWDHCP_DHCP_MAX];
        char DhcpEndIpWifi2[WIFI_CWDHCP_DHCP_MAX];
        char DhcpTimeWifi2[WIFI_CWDHCP_DHCP_MAX];

        // STA
        char StaIp[WIFI_STA_IP_MAX];
        char StaSsidExt[WIFI_CWSSID_SSID_MAX];
        uint8_t StaSecurity;
        uint8_t StaProtocol;
        char StaPassword[WIFI_CWAUTH_PASSWORD_MAX];
    };

    /* Lte4g Data */
    struct lte4g_json_t
    {
        char Imei[IMEI_MAX];
        uint8_t Index;

        uint8_t CardStatus;
        uint8_t AppType;
        uint8_t AppState;
        uint8_t Pin;

        char SimIccid[32];
        char SimImsi[32];

        char SystemMode[16];
        char OperationMode[16];
        char MobileCountryCode[8];
        char MobileNetworkCode[8];
        char LocationAreaCode[16];
        char ServiceCellId[16];
        char FreqBand[16];
        char Current4GData[16];
        char Afrcn[16];
        char PhoneNumber[16];
        uint8_t Rssi4G;
        uint8_t NetworkMode;
    };

    /* Ethernet Data */
    struct ethernet_json_t
    {
        char Imei[IMEI_MAX];
        uint8_t Index;

        uint8_t DriverType;
        uint8_t DriverEn;
        uint8_t BringUpdownEn;
        uint8_t IpStaticEn;
        char IpAddr[24];
        char Netmask[24];
    };

    /* GPS Data */
    struct gps_json_t
    {
        char Imei[IMEI_MAX];
        uint8_t Index;

        double Latitude;
        double Longitude;
        double Altitude;
        float Speed;
        float Bearing;
        float Accuracy;
        char Time[6];
    };
};

static const struct json_attr_t check_attrs[] = {
    {
        "Imei",
        t_string,
        .addr.string = check_info.Imei,
        .len = IMEI_MAX,
    },
    {
        "Index",
        t_uinteger,
        .addr.uinteger = &check_info.Index,
    },

    {NULL},
};

static const struct json_attr_t wifi_json_attrs[] = {
    /********************** Wifi General *************************/
    {
        "Imei",
        t_string,
        .addr.string = wifi_config.Imei,
        .len = sizeof(wifi_config.Imei),
    },
    {
        "Index",
        t_uinteger,
        .addr.uinteger = &wifi_config.Index,
    },
    {
        "WifiOpen",
        t_uinteger,
        .addr.uinteger = &wifi_config.WifiOpen,
    },
    {
        "WifiMode",
        t_uinteger,
        .addr.uinteger = &wifi_config.WifiMode,
    },
    {
        "CurrentAp",
        t_uinteger,
        .addr.uinteger = &wifi_config.CurrentAp,
    },
    {
        "WifiNat",
        t_uinteger,
        .addr.uinteger = &wifi_config.WifiNat,
    },
    /*********************** Wifi 1 ************************/
    {
        "SsidWifi1",
        t_string,
        .addr.string = wifi_config.SsidWifi1,
        .len = sizeof(wifi_config.SsidWifi1),
    },
    {
        "AuthTypeWifi1",
        t_uinteger,
        .addr.uinteger = &wifi_config.AuthTypeWifi1,
    },
    {
        "EncryptModeWifi1",
        t_uinteger,
        .addr.uinteger = &wifi_config.EncryptModeWifi1,
    },
    {
        "AuthPwdWifi1",
        t_string,
        .addr.string = wifi_config.AuthPwdWifi1,
        .len = sizeof(wifi_config.AuthPwdWifi1),
    },
    {
        "ClientCountWifi1",
        t_uinteger,
        .addr.uinteger = &wifi_config.ClientCountWifi1,
    },
    {
        "BroadCastWifi1",
        t_uinteger,
        .addr.uinteger = &wifi_config.BroadCastWifi1,
    },
    {
        "IsolationWifi1",
        t_uinteger,
        .addr.uinteger = &wifi_config.IsolationWifi1,
    },
    {
        "ChannelModeWifi1",
        t_uinteger,
        .addr.uinteger = &wifi_config.ChannelModeWifi1,
    },
    {
        "ChannelWifi1",
        t_uinteger,
        .addr.uinteger = &wifi_config.ChannelWifi1,
    },
    {
        "MacAddressWifi1",
        t_string,
        .addr.string = wifi_config.MacAddressWifi1,
        .len = sizeof(wifi_config.MacAddressWifi1),
    },
    {
        "DhcpHostIpWifi1",
        t_string,
        .addr.string = wifi_config.DhcpHostIpWifi1,
        .len = sizeof(wifi_config.DhcpHostIpWifi1),
    },
    {
        "DhcpStartIpWifi1",
        t_string,
        .addr.string = wifi_config.DhcpStartIpWifi1,
        .len = sizeof(wifi_config.DhcpStartIpWifi1),
    },
    {
        "DhcpEndIpWifi1",
        t_string,
        .addr.string = wifi_config.DhcpEndIpWifi1,
        .len = sizeof(wifi_config.DhcpEndIpWifi1),
    },
    {
        "DhcpTimeWifi1",
        t_string,
        .addr.string = wifi_config.DhcpTimeWifi1,
        .len = sizeof(wifi_config.DhcpTimeWifi1),
    },
    /*********************** Wifi 2 ************************/
    {
        "SsidWifi2",
        t_string,
        .addr.string = wifi_config.SsidWifi2,
        .len = sizeof(wifi_config.SsidWifi2),
    },
    {
        "AuthTypeWifi2",
        t_uinteger,
        .addr.uinteger = &wifi_config.AuthTypeWifi2,
    },
    {
        "EncryptModeWifi2",
        t_uinteger,
        .addr.uinteger = &wifi_config.EncryptModeWifi2,
    },
    {
        "AuthPwdWifi2",
        t_string,
        .addr.string = wifi_config.AuthPwdWifi2,
        .len = sizeof(wifi_config.AuthPwdWifi2),
    },
    {
        "ClientCountWifi2",
        t_uinteger,
        .addr.uinteger = &wifi_config.ClientCountWifi2,
    },
    {
        "BroadCastWifi2",
        t_uinteger,
        .addr.uinteger = &wifi_config.BroadCastWifi2,
    },
    {
        "IsolationWifi2",
        t_uinteger,
        .addr.uinteger = &wifi_config.IsolationWifi2,
    },
    {
        "ChannelModeWifi2",
        t_uinteger,
        .addr.uinteger = &wifi_config.ChannelModeWifi2,
    },
    {
        "ChannelWifi2",
        t_uinteger,
        .addr.uinteger = &wifi_config.ChannelWifi2,
    },
    {
        "MacAddressWifi2",
        t_string,
        .addr.string = wifi_config.MacAddressWifi2,
        .len = sizeof(wifi_config.MacAddressWifi2),
    },
    {
        "DhcpHostIpWifi2",
        t_string,
        .addr.string = wifi_config.DhcpHostIpWifi2,
        .len = sizeof(wifi_config.DhcpHostIpWifi2),
    },
    {
        "DhcpStartIpWifi2",
        t_string,
        .addr.string = wifi_config.DhcpStartIpWifi2,
        .len = sizeof(wifi_config.DhcpStartIpWifi2),
    },
    {
        "DhcpEndIpWifi2",
        t_string,
        .addr.string = wifi_config.DhcpEndIpWifi2,
        .len = sizeof(wifi_config.DhcpEndIpWifi2),
    },
    {
        "DhcpTimeWifi2",
        t_string,
        .addr.string = wifi_config.DhcpTimeWifi2,
        .len = sizeof(wifi_config.DhcpTimeWifi2),
    },

    /*********************** Wifi STA ************************/
    {
        "StaIp",
        t_string,
        .addr.string = wifi_config.StaIp,
        .len = sizeof(wifi_config.StaIp),
    },
    {
        "StaSsidExt",
        t_string,
        .addr.string = wifi_config.StaSsidExt,
        .len = sizeof(wifi_config.StaSsidExt),
    },
    {
        "StaSecurity",
        t_uinteger,
        .addr.uinteger = &wifi_config.StaSecurity,
    },
    {
        "StaProtocol",
        t_uinteger,
        .addr.uinteger = &wifi_config.StaProtocol,
    },
    {
        "StaPassword",
        t_string,
        .addr.string = wifi_config.StaPassword,
        .len = sizeof(wifi_config.StaPassword),
    },

    {NULL},
};

static const struct json_attr_t lte4g_json_attrs[] = {
    {
        "Imei",
        t_string,
        .addr.string = lte4g_config.Imei,
        .len = sizeof(lte4g_config.Imei),
    },
    {
        "Index",
        t_uinteger,
        .addr.uinteger = &lte4g_config.Index,
    },
    {
        "CardStatus",
        t_uinteger,
        .addr.uinteger = &lte4g_config.CardStatus,
    },
    {
        "AppType",
        t_uinteger,
        .addr.uinteger = &lte4g_config.AppType,
    },
    {
        "AppState",
        t_uinteger,
        .addr.uinteger = &lte4g_config.AppState,
    },
    {
        "Pin",
        t_uinteger,
        .addr.uinteger = &lte4g_config.Pin,
    },
    {
        "SimIccid",
        t_string,
        .addr.string = lte4g_config.SimIccid,
        .len = sizeof(lte4g_config.SimIccid),
    },
    {
        "SimImsi",
        t_string,
        .addr.string = lte4g_config.SimImsi,
        .len = sizeof(lte4g_config.SimImsi),
    },
    {
        "SystemMode",
        t_string,
        .addr.string = lte4g_config.SystemMode,
        .len = sizeof(lte4g_config.SystemMode),
    },
    {
        "OperationMode",
        t_string,
        .addr.string = lte4g_config.OperationMode,
        .len = sizeof(lte4g_config.OperationMode),
    },
    {
        "MobileCountryCode",
        t_string,
        .addr.string = lte4g_config.MobileCountryCode,
        .len = sizeof(lte4g_config.MobileCountryCode),
    },
    {
        "MobileNetworkCode",
        t_string,
        .addr.string = lte4g_config.MobileNetworkCode,
        .len = sizeof(lte4g_config.MobileNetworkCode),
    },
    {
        "LocationAreaCode",
        t_string,
        .addr.string = lte4g_config.LocationAreaCode,
        .len = sizeof(lte4g_config.LocationAreaCode),
    },
    {
        "ServiceCellId",
        t_string,
        .addr.string = lte4g_config.ServiceCellId,
        .len = sizeof(lte4g_config.ServiceCellId),
    },
    {
        "FreqBand",
        t_string,
        .addr.string = lte4g_config.FreqBand,
        .len = sizeof(lte4g_config.FreqBand),
    },
    {
        "Current4GData",
        t_string,
        .addr.string = lte4g_config.Current4GData,
        .len = sizeof(lte4g_config.Current4GData),
    },
    {
        "Afrcn",
        t_string,
        .addr.string = lte4g_config.Afrcn,
        .len = sizeof(lte4g_config.Afrcn),
    },
    {
        "PhoneNumber",
        t_string,
        .addr.string = lte4g_config.PhoneNumber,
        .len = sizeof(lte4g_config.PhoneNumber),
    },
    {
        "Rssi4G",
        t_uinteger,
        .addr.uinteger = &lte4g_config.Rssi4G,
    },
    {
        "NetworkMode",
        t_uinteger,
        .addr.uinteger = &lte4g_config.NetworkMode,
    },

    {NULL},
};

static const struct json_attr_t ether_json_attrs[] = {
    {
        "Imei",
        t_string,
        .addr.string = ether_config.Imei,
        .len = sizeof(ether_config.Imei),
    },
    {
        "Index",
        t_uinteger,
        .addr.uinteger = &ether_config.Index,
    },
    {
        "DriverType",
        t_uinteger,
        .addr.uinteger = &ether_config.DriverType,
    },
    {
        "DriverEn",
        t_uinteger,
        .addr.uinteger = &ether_config.DriverEn,
    },
    {
        "BringUpdownEn",
        t_uinteger,
        .addr.uinteger = &ether_config.BringUpdownEn,
    },
    {
        "IpStaticEn",
        t_uinteger,
        .addr.uinteger = &ether_config.IpStaticEn,
    },
    {
        "IpAddr",
        t_string,
        .addr.string = ether_config.IpAddr,
        .len = sizeof(ether_config.IpAddr),
    },
    {
        "Netmask",
        t_string,
        .addr.string = ether_config.Netmask,
        .len = sizeof(ether_config.Netmask),
    },

    {NULL},
};

static const struct json_attr_t gps_json_attrs[] = {
    {
        "Imei",
        t_string,
        .addr.string = gps_config.Imei,
        .len = sizeof(gps_config.Imei),
    },
    {
        "Index",
        t_uinteger,
        .addr.uinteger = &gps_config.Index,
    },
    {
        "Latitude",
        t_real,
        .addr.real = &gps_config.Latitude,
    },
    {
        "Longitude",
        t_real,
        .addr.real = &gps_config.Longitude,
    },
    {
        "Altitude",
        t_real,
        .addr.real = &gps_config.Altitude,
    },
    {
        "Speed",
        t_real,
        .addr.real = &gps_config.Speed,
    },
    {
        "Bearing",
        t_real,
        .addr.real = &gps_config.Bearing,
    },
    {
        "Accuracy",
        t_real,
        .addr.real = &gps_config.Accuracy,
    },
    {
        "Time",
        t_string,
        .addr.string = gps_config.Time,
        .len = sizeof(gps_config.Time),
    },

    {NULL},
};

///////////////////////////////////////PROTOTYPES/////////////////////////////////
void TCP_init(void);
int TCP_connect(char *serverIP, int serverPort);
int TCP_receive(char *serverIP, int serverPort);
int TCP_send(char *serverIP, int serverPort, uint8_t *send_data, uint32_t send_len);

void server_inf_init(void);
int server_inf_connect(void);
int server_inf_send(char *serverIP, int serverPort, uint8_t *send_data, uint32_t send_len);
void server_inf_manager(void);
int app_udp_send_data(char *serverIP, int serverPort, uint8_t *send_data, uint32_t send_len);

void json_cfg_wifi_create(void);
void json_cfg_ethernet_create(void);
void json_cfg_lte4g_create(void);
void json_cfg_gps_create(void);
void json_cfg_feedback_create(void);

uint8_t wifi_data_send(void);
uint8_t lte4g_data_send(void);
uint8_t ethernet_data_send(void);
uint8_t gps_data_send(void);
uint8_t feedback_data_send(void);
void all_data_send(void);

void wifi_handler(void);
void lte4g_handler(void);
void gps_handler(void);
void ethernet_handler(void);

#endif