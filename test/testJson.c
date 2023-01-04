#include <stdio.h>
#include <stdint.h>
#include <mjson.h>

// Wifi count:
#define WIFI_COUNT_MAX 2
#define WIFI_COUNT_MIN 1

// MAC:
#define MAC_ADDR_MAX 16
#define MAC_ADDR_COUNT 16

// WiFi 0
#define WIFI_00_CWSSID_SSID_DEFAULT "P.NCSP_NCPT_5Ghz"
#define WIFI_00_CWAUTH_PWD_DEFAULT "1234567890"
#define WIFI_00_CWAUTH_TYPE_DEFAULT 5
#define WIFI_00_CWAUTH_ENCRYPT_DEFAULT 4
#define WIFI_00_CWMOCH_MODE_DEFAULT 5      // 5GHz
#define WIFI_00_CWMOCH_CHANNEL_DEFAULT 149 // ch=149
#define WIFI_00_CWBCAST_DEFAULT 1
#define WIFI_00_CWISO_ISOLATION_DEFAULT 0
#define WIFI_00_CWISO_MACADDR_DEFAULT "00:00:00:00:00:00"

// WiFi 1
#define WIFI_01_CWSSID_SSID_DEFAULT "P.NCSP_NCPT_2.4Ghz"
#define WIFI_01_CWAUTH_PWD_DEFAULT "1234567890"
#define WIFI_01_CWAUTH_TYPE_DEFAULT 5
#define WIFI_01_CWAUTH_ENCRYPT_DEFAULT 4
#define WIFI_01_CWMOCH_MODE_DEFAULT 4    // 2.4GHz
#define WIFI_01_CWMOCH_CHANNEL_DEFAULT 0 // ch=auto
#define WIFI_01_CWBCAST_DEFAULT 1
#define WIFI_01_CWISO_ISOLATION_DEFAULT 0
#define WIFI_01_CWISO_MACADDR_DEFAULT "00:00:00:00:00:00"

// WiFi 2
#define WIFI_02_CWSSID_SSID_DEFAULT "P.NCSP_NCPT_STA"
#define WIFI_02_CWAUTH_PWD_DEFAULT "1234567890"
#define WIFI_02_CWAUTH_TYPE_DEFAULT 5
#define WIFI_02_CWAUTH_ENCRYPT_DEFAULT 4
#define WIFI_02_CWMOCH_MODE_DEFAULT 4    // 2.4GHz
#define WIFI_02_CWMOCH_CHANNEL_DEFAULT 0 // ch=auto
#define WIFI_02_CWBCAST_DEFAULT 1
#define WIFI_02_CWISO_ISOLATION_DEFAULT 0
#define WIFI_02_CWISO_MACADDR_DEFAULT "00:00:00:00:00:00"

// Open(close) wifi
#define WIFI_CWMAP_FLAG_OPEN 1
#define WIFI_CWMAP_FLAG_CLOSE 0

// SSID:
#define WIFI_CWSSID_SSID_MAX 32
#define WIFI_CWSSID_SSID_MIN 8

// Broadcast:
#define WIFI_CWBCAST_DISABLED 0
#define WIFI_CWBCAST_ENABLED 1

// Auth:
#define WIFI_CWAUTH_PASSWORD_MAX 32
#define WIFI_CWAUTH_PASSWORD_MIN 8
#define WIFI_CWAUTH_AUTH_OPENSHARE 0
#define WIFI_CWAUTH_AUTH_OPEN 1
#define WIFI_CWAUTH_AUTH_SHARE 2
#define WIFI_CWAUTH_AUTH_WPA 3
#define WIFI_CWAUTH_AUTH_WPA2 4
#define WIFI_CWAUTH_AUTH_WPA_WPA2 5
#define WIFI_CWAUTH_ENCRYPT_NULL 0
#define WIFI_CWAUTH_ENCRYPT_WEP 1
#define WIFI_CWAUTH_ENCRYPT_TKIP 2
#define WIFI_CWAUTH_ENCRYPT_AES 3
#define WIFI_CWAUTH_ENCRYPT_TKIP_AES 4

// Mode and channel:
#define WIFI_CWMOCH_MODE_80211AN_5G 1
#define WIFI_CWMOCH_MODE_80211B_2_4G 2
#define WIFI_CWMOCH_MODE_80211BG_2_4G 3
#define WIFI_CWMOCH_MODE_80211BGN_2_4G 4
#define WIFI_CWMOCH_MODE_80211ACN_5G 5
#define WIFI_CWMOCH_CHANNEL_AUTO 0

// Isolation:
#define WIFI_CWMISO_ISOLATION_OPEN 1
#define WIFI_CWMISO_ISOLATION_CLOSE 0

// NAT:
#define WIFI_CWMISO_NAT_SYMETRIC 0
#define WIFI_CWMISO_NAT_CONE 1

#define WIFI_CWDHCP_DHCP_MAX 16

// MAP Configure:
#define WIFI_CWCMAPCFG_VALUE_AP 0
#define WIFI_CWCMAPCFG_VALUE_APAP 1
#define WIFI_CWCMAPCFG_VALUE_STAAP 2
#define WIFI_CWCMAPCFG_SET_WIFI_0 0
#define WIFI_CWCMAPCFG_SET_WIFI_1 1
#define WIFI_CWCMAPCFG_SET_WIFI_2 2

// STA
#define WIFI_STA_IP_MAX 16

/* Wifi Data */
struct wifi_json_t
{
    char Imei[100];

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
static struct wifi_json_t wifi_config;
static const struct json_attr_t wifi_json_attrs[] = {
    {
        "Imei",
        t_string,
        .addr.string = wifi_config.Imei,
        .len = sizeof(wifi_config.Imei),
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
        .addr.uinteger = &wifi_config.ClientCountWifi1,
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

    // STA
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
        .addr.uinteger = &wifi_config.StaSecurity,
    },
    {
        "StaPassword",
        t_string,
        .addr.string = wifi_config.StaPassword,
        .len = sizeof(wifi_config.StaPassword),
    },

    {NULL},
};

static uint8_t buff[2048];

int main()
{
    printf("Nhap Buff: ");
    scanf("%s", buff);

    uint8_t status;
    status = json_read_object(&buff[1], wifi_json_attrs, NULL);
    if (status != 0)
        puts(json_error_string(status));
    printf("=====================================================================================\n");
    printf("IMEI = %s, WifiOpen = %d, WifiMode = %d, CurrentAp = %d, WifiNat = %d\n",
           wifi_config.Imei,
           wifi_config.WifiOpen,
           wifi_config.WifiMode,
           wifi_config.CurrentAp,
           wifi_config.WifiNat);
    printf("-------------------------------------------------------------------------------------\n");
    printf("+ Ssid1: %s\n", wifi_config.SsidWifi1);
    printf("+ AuthType1: %d\n", wifi_config.AuthTypeWifi1);
    printf("+ EncryptMode1: %d\n", wifi_config.EncryptModeWifi1);
    printf("+ AuthPwd1: %s\n", wifi_config.AuthPwdWifi1);
    printf("+ BroadCast1: %d\n", wifi_config.BroadCastWifi1);
    printf("+ Isolation1: %d\n", wifi_config.IsolationWifi1);
    printf("+ ChannelMode1: %d\n", wifi_config.ChannelModeWifi1);
    printf("+ Channel1: %d\n", wifi_config.ChannelWifi1);
    printf("+ DhcpHostIp1: %s\n", wifi_config.DhcpHostIpWifi1);
    printf("+ DhcpStartIp1: %s\n", wifi_config.DhcpStartIpWifi1);
    printf("+ DhcpEndIp1: %s\n", wifi_config.DhcpEndIpWifi1);
    printf("+ DhcpTime1: %s\n", wifi_config.DhcpTimeWifi1);
    printf("-------------------------------------------------------------------------------------\n");
    printf("+ Ssid2: %s\n", wifi_config.SsidWifi2);
    printf("+ AuthType2: %d\n", wifi_config.AuthTypeWifi2);
    printf("+ EncryptMode2: %d\n", wifi_config.EncryptModeWifi2);
    printf("+ AuthPwd2: %s\n", wifi_config.AuthPwdWifi2);
    printf("+ BroadCast2: %d\n", wifi_config.BroadCastWifi2);
    printf("+ Isolation2: %d\n", wifi_config.IsolationWifi2);
    printf("+ ChannelMode2: %d\n", wifi_config.ChannelModeWifi2);
    printf("+ Channel2: %d\n", wifi_config.ChannelWifi2);
    printf("+ DhcpHostIp2: %s\n", wifi_config.DhcpHostIpWifi2);
    printf("+ DhcpStartIp2: %s\n", wifi_config.DhcpStartIpWifi2);
    printf("+ DhcpEndIp2: %s\n", wifi_config.DhcpEndIpWifi2);
    printf("+ DhcpTime2: %s\n", wifi_config.DhcpTimeWifi2);
    printf("-------------------------------------------------------------------------------------\n");
    printf("+ StaSsidExt: %s\n", wifi_config.StaSsidExt);
    printf("+ StaSecurity: %d\n", wifi_config.StaSecurity);
    printf("+ StaProtocol: %d\n", wifi_config.StaProtocol);
    printf("+ StaPassword: %s\n", wifi_config.StaPassword);
    printf("=====================================================================================\n");
    return 0;
}