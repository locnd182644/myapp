#ifndef _GPS_CONTROL_H_
#define _GPS_CONTROL_H_

#include "app_common.h"

////////////////////////////////////CONSTANTS///////////////////////////////////////
#define LOC_NMEA_MASK_GGA_V02 ((NmeaSentenceTypesMask)0x00000001)   /**<  Enable GGA type  */
#define LOC_NMEA_MASK_RMC_V02 ((NmeaSentenceTypesMask)0x00000002)   /**<  Enable RMC type  */
#define LOC_NMEA_MASK_GSV_V02 ((NmeaSentenceTypesMask)0x00000004)   /**<  Enable GSV type  */
#define LOC_NMEA_MASK_GSA_V02 ((NmeaSentenceTypesMask)0x00000008)   /**<  Enable GSA type  */
#define LOC_NMEA_MASK_VTG_V02 ((NmeaSentenceTypesMask)0x00000010)   /**<  Enable VTG type  */
#define LOC_NMEA_MASK_PQXFI_V02 ((NmeaSentenceTypesMask)0x00000020) /**<  Enable PQXFI type  */
#define LOC_NMEA_MASK_PSTIS_V02 ((NmeaSentenceTypesMask)0x00000040) /**<  Enable PSTIS type  */
#define LOC_NMEA_MASK_GLGSV_V02 ((NmeaSentenceTypesMask)0x00000080) /**<  Enable GLGSV type  */
#define LOC_NMEA_MASK_GNGSA_V02 ((NmeaSentenceTypesMask)0x00000100) /**<  Enable GNGSA type  */
#define LOC_NMEA_MASK_GNGNS_V02 ((NmeaSentenceTypesMask)0x00000200) /**<  Enable GNGNS type  */
#define LOC_NMEA_MASK_GARMC_V02 ((NmeaSentenceTypesMask)0x00000400) /**<  Enable GARMC type  */
#define LOC_NMEA_MASK_GAGSV_V02 ((NmeaSentenceTypesMask)0x00000800) /**<  Enable GAGSV type  */
#define LOC_NMEA_MASK_GAGSA_V02 ((NmeaSentenceTypesMask)0x00001000) /**<  Enable GAGSA type  */
#define LOC_NMEA_MASK_GAVTG_V02 ((NmeaSentenceTypesMask)0x00002000) /**<  Enable GAVTG type  */
#define LOC_NMEA_MASK_GAGGA_V02 ((NmeaSentenceTypesMask)0x00004000) /**<  Enable GAGGA type  */
#define LOC_NMEA_MASK_PQGSA_V02 ((NmeaSentenceTypesMask)0x00008000) /**<  Enable PQGSA type  */
#define LOC_NMEA_MASK_PQGSV_V02 ((NmeaSentenceTypesMask)0x00010000) /**<  Enable PQGSV type  */
#define LOC_NMEA_ALL_SUPPORTED_MASK (LOC_NMEA_MASK_GGA_V02 | LOC_NMEA_MASK_RMC_V02 |                               \
                                     LOC_NMEA_MASK_GSV_V02 | LOC_NMEA_MASK_GSA_V02 | LOC_NMEA_MASK_VTG_V02 |       \
                                     LOC_NMEA_MASK_PQXFI_V02 | LOC_NMEA_MASK_PSTIS_V02 | LOC_NMEA_MASK_GLGSV_V02 | \
                                     LOC_NMEA_MASK_GNGSA_V02 | LOC_NMEA_MASK_GNGNS_V02 | LOC_NMEA_MASK_GARMC_V02 | \
                                     LOC_NMEA_MASK_GAGSV_V02 | LOC_NMEA_MASK_GAGSA_V02 | LOC_NMEA_MASK_GAVTG_V02 | \
                                     LOC_NMEA_MASK_GAGGA_V02 | LOC_NMEA_MASK_PQGSA_V02 | LOC_NMEA_MASK_PQGSV_V02)

///////////////////////////////////TYPES//////////////////////////////////
typedef unsigned char boolean;

typedef struct
{
    double latitude;  /** Represents latitude in degrees. */
    double longitude; /** Represents longitude in degrees. */
    double altitude;  /** Represents altitude in meters above the WGS 84 reference ellipsoid. */
    float speed;      /** Represents speed in meters per second. */
    float bearing;    /** Represents heading in degrees. */
    float accuracy;   /** Represents expected accuracy in meters. */
    uint8_t time[6];  /** Y-M-D H-M-S*/
} GPS_FUNCTION_t;
GPS_FUNCTION_t g_gps_info;

typedef void (*gps_ind_cb_fcn)(uint8_t simcom_event, void *gps_info);
typedef uint32_t NmeaSentenceTypesMask;

////////////////////////////////////PROTOTYPES//////////////////////////////////
boolean gps_init(gps_ind_cb_fcn gps_ind_cb);
void gps_deinit();
boolean gps_coldstart();
boolean gps_hotstart();
boolean gps_stop();
int8_t gps_info_received(uint8_t *data);
int8_t gps_state_received(uint8_t *data);
#endif //_GPS_CONTROL_H_
