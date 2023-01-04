#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "simcom_common.h"
#include "qmi_client.h"
#include "wireless_messaging_service_v01.h"

//////////////////////////////////////CONSTANTS///////////////////////////////////// 
#define TRUE   1
#define FALSE  0

#define SMS_MAX_ADDRESS_LENGTH  48
#define SMS_MAX_CONTENT_LENGTH  255
//////////////////////////////////////TYPES/////////////////////////////////////////
typedef unsigned char uint8;
typedef unsigned char boolean;
typedef enum {
	SMS_MSG_FORMAT_TEXT_NULL,
	SMS_MSG_FORMAT_TEXT_ASCII = 1,
	SMS_MSG_FORMAT_TEXT_UTF8 = 2,
	SMS_MSG_FORMAT_TEXT_UNICODE = 3,
	SMS_MSG_FORMAT_TEXT_MAX
} sms_format;

typedef struct {
	boolean     	is_concate;
	uint8         	msg_ref;
	uint8        	seq_num;
	uint8        	total_sm;
	sms_format     	format;
	int            	message_len;
	char         	message_content[SMS_MAX_CONTENT_LENGTH];
	char         	source_address[SMS_MAX_ADDRESS_LENGTH];
} sms_info_type;

typedef void (*sms_ind_cb_fcn)(simcom_event_e simcom_event, sms_info_type *sms_info);

////////////////////////////////////PROTOTYPES///////////////////////////////////////
int sms_init(sms_ind_cb_fcn sms_ind_cb);
void sms_deinit();
void sms_set_format(sms_format format);
int send_message(int smsMode, char *phoneNumber, unsigned char *content, int content_len);
#endif
