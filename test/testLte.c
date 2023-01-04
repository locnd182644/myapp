#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    // Simcard:
    char sim_iccid[32];
    char sim_imsi[32];
    char sim_imei[32];
    char phone_number[16];
    uint8_t rssi_4g;

    char system_mode[16];
    char operation_mode[16];
    char mobile_country_code[8];
    char mobile_network_code[8];
    char location_area_code[16];
    char service_cell_id[16];
    char physical_cell_id[16];
    char freq_band[16];
    char afrcn[16];
    char current_4g_data[16];B

    uint8_t network_mode;
} LTE4G_FUNCTION_t;
LTE4G_FUNCTION_t g_lte4g_info;

char buff1[100] = "+CPSI: LTE,Online,452-01,0x4202,26134029,449,EUTRAN-AND3,1501,5,5,-122,-901,-580,9";
char buff2[100] = "+CPSI: NO SERVICE,Online";

/* +CPSI: <System Mode>,<Operation Mode>[,<MCC>-<MNC>,
<TAC>,<SCellID>,<PCellID>,<Frequency Band>,<earfcn>,<dlbw>,
<ulbw>,<RSRQ>,<RSRP>,<RSSI>,<RSSNR>] */

int main()
{
    char *p;
    char temp[20];
    printf("Length of buff1: %ld\n", strlen(buff1));
    p = strtok(&buff1[7], ",");
    if (strcmp(p, "NO SERVICE") == 0)
    {
        strcpy(g_lte4g_info.system_mode, p);
        p = strtok(NULL, ",");
        strcpy(g_lte4g_info.operation_mode, p);
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
    printf("system_mode: %s\n", g_lte4g_info.system_mode);
    printf("operation_mode: %s\n", g_lte4g_info.operation_mode);
    printf("mobile_country_code: %s\n", g_lte4g_info.mobile_country_code);
    printf("mobile_network_code: %s\n", g_lte4g_info.mobile_network_code);
    printf("location_area_code: %s\n", g_lte4g_info.location_area_code);
    printf("service_cell_id: %s\n", g_lte4g_info.service_cell_id);
    printf("freq_band: %s\n", g_lte4g_info.freq_band);
    printf("afrcn: %s\n", g_lte4g_info.afrcn);

    return 0;
}
