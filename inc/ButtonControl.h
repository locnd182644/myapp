/**	@brief define Constants & Prototypes for Button Functions.
	@history
		- 2021_07_24: Create.
*/
#ifndef _APP_BUTTON_H_
#define _APP_BUTTON_H_

#include "app_common.h"
///////////////////////////////////CONSTANTS///////////////////////////////////
#define BUTTON_WPS			0
#define BUTTON_RESET		1
////////////////////////////////////TYPES//////////////////////////////////////

///////////////////////////////PROTOTYPES//////////////////////////////////////
int button_init();
int button_read(int bt_number);
#endif
