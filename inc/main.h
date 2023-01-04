#ifndef _SIMCOM_TEST_MAIN_H_
#define _SIMCOM_TEST_MAIN_H_
#include "simcom_common.h"
#include "OTAUpgrade.h"
#include "LedControl.h"
#include "EthControl.h"
#include "WiFiControl.h"
#include "WDSControl.h"
#include "GPSControl.h"
#include "NASControl.h"
#include "ButtonControl.h"
#include "SdcardControl.h"
#include "app_timer.h"
#include "app_serverinf.h"
#include "LteControl.h"

const char * get_simcom_sdk_version(void);
const char * get_simcom_demo_version(void);
int json_test1(void);
#endif


