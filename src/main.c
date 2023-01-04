/**	@brief: main function.
    @date
        - 2021_07_23: Create.
*/

#include "main.h"
#include "DataCall.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
///////////////////////////////////VARIABLES//////////////////////////////

//////////////////////////////////CALLBACK////////////////////////////////
void process_simcom_ind_message(simcom_event_e event, void *cb_usr_data)
{
    int i;
    switch (event)
    {
    case SIMCOM_EVENT_SMS_PP_IND:
        lte4g_sms_received(cb_usr_data);
        break;

    case SIMCOM_EVENT_NETWORK_IND:
        lte4g_network_received(cb_usr_data);
        break;
    case SIMCOM_EVENT_NMEA_IND:
        printf((char *)cb_usr_data);
        break;
    case SIMCOM_EVENT_LOC_IND:
        gps_state_received(cb_usr_data);
        break;
    // case SIMCOM_EVENT_TRYLTE_URC_IND:
    //     printf("%s\n", (char *)cb_usr_data);
    //     break;
    // case SIMCOM_EVENT_GPS_ENGINE_STATE_IND:
    //     gps_state_received(cb_usr_data);
    //     break;
    default:
        break;
    }
}

//////////////////////////////////MAIN FUNCTION///////////////////////////
int main(int argc, char *argv[])
{
    // Init SD Card:
    sdcard_init();
    printf("\n\rinit_sdcard success\n");

    // Init AT:
    atctrl_init();
    printf("\n\rinit_atcommand success\n");

    // Init 4G/LTE:
    lte4g_init();
    printf("\n\rinit_4g success\n");

    // Init Wifi:
    // wifi_init();
    // printf("\n\rinit_wifi success\n");

    // Init Ethernet:
    // ethernet_init();
    // printf("\n\rinit_ethernet success\n");

    // Init GPS:
    // gps_init((gps_ind_cb_fcn)process_simcom_ind_message);
    // // gps_coldstart();
    // printf("\n\rinit_gps success\n");

    // Init Led:
    tbox_gpio_init();
    printf("\n\rinit_gpio success\n");

    // Delay to stable system
    printf("\nWaiting for init");
    fflush(stdout);
    for (uint8_t i = 0; i < CONFIG_TIME; i++)
    {
        sleep(1);
        printf(".");
        fflush(stdout);
    }
    printf("\n");

    // Init TCP
    TCP_init();
    printf("\n\rinit_serverinf sucsess\n");

    // Loop:
    while (1)
    {
    }

    return 0;
}
