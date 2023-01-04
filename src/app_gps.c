/**	@brief implement all functions of Wifi.
	@date
		- 2021_07_23: Create.
*/

#include "GPSControl.h"

// #define _DB_INIT_
// #define _DB_GPS_

/////////////////////////////////////////////VARIABLES///////////////////////////////////

////////////////////////////////////////////LOCAL FUNCTIONS///////////////////////////////

////////////////////////////////////////////PUBLIC FUNCTIONS/////////////////////////////
/*****************************************************************************
 * Function Name : gps_info_received
 * Description   : .
 * Input         : None
 * Output        : None
 * Return        : None
 *****************************************************************************/
int8_t gps_info_received(uint8_t *data)
{
	int8_t ret = -1;
	uint8_t i = 0;
	GPS_FUNCTION_t *location = (GPS_FUNCTION_t *)data;
	printf("\nLATITUDE:  %f\nLONGITUDE: %f\nALTITUDE:  %f\nSPEED:     %f\nTIME:      %02d-%02d-%02d %02d:%02d:%02d\n",
		   location->latitude, location->longitude, location->altitude, location->speed,
		   location->time[0], location->time[1], location->time[2], location->time[3],
		   location->time[4], location->time[5]);
	return ret;
}

/*****************************************************************************
 * Function Name : gps_state_get
 * Description   : .
 * Input         : None
 * Output        : None
 * Return        : None
 *****************************************************************************/
int8_t gps_state_received(uint8_t *data)
{
	int8_t ret = -1;
	int *gps_engine_state = (int *)data;
	if ((*gps_engine_state) == 1)
	{
		printf("GPS engine on\n");
	}
	else if ((*gps_engine_state) == 2)
	{
		printf("GPS engine off\n");
	}
	else
	{
		printf("Unknow engine state\n");
	}
	return ret;
}
