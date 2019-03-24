#ifndef LINETHINGS_PROFILE_H
#define LINETHINGS_PROFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */
#define LINEBEACON_SERV_UUID      0xFE6F

// Profile Parameters
#define LINETHINGS_PROFILE_SERVICEUUID_CHAR              1
#define LINETHINGS_PROFILE_BEACONHWID_CHAR               2
#define LINETHINGS_PROFILE_SWITCH_CHAR                   3
#define LINETHINGS_PROFILE_NOTIFY_CHAR                   4

// Length of Characteristic 5 in bytes
#define LINETHINGS_PROFILE_SERVICEUUID_CHAR_LEN   16
#define LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN    5
#define LINETHINGS_PROFILE_SWITCH_CHAR_LEN    1

/*********************************************************************
 * TYPEDEFS
 */
extern uint8 linethingsChangeFlag;
#define LINETHINGS_CHGFLG_SERVICEUUID   0x01
#define LINETHINGS_CHGFLG_BEACONHWID    0x02
#define LINETHINGS_CHGFLG_SWITCH        0x03


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */


/*********************************************************************
 * API FUNCTIONS
 */


/*
 * SimpleProfile_AddService- Initializes the Simple GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 */

extern bStatus_t LineThingsProfile_AddService( const uint8 *serviceuuid, const uint8 *hwid, const uint8* line_switch );

/*
 * SimpleProfile_SetParameter - Set a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t LineThingsProfile_SetParameter( uint8 param, uint8 len, void *value );

/*
 * SimpleProfile_GetParameter - Get a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t LineThingsProfile_GetParameter( uint8 param, void *value );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
