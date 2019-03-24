#ifndef LINETHINGS_PSDI_PROFILE_H
#define LINETHINGS_PSDI_PROFILE_H

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

// Profile Parameters
#define LINETHINGS_PSDI_PROFILE_CHAR                   0

// Simple Profile Service UUID
#define LINETHINGS_PSDI_PROFILE_SERV_UUID               "E625601E-9E55-4597-A598-76018A0D293D"

// Key Pressed UUID
#define LINETHINGS_PSDI_PROFILE_CHAR_UUID            "26E2B12B-85F0-4F3F-9FDD-91D114270E6E"

// Simple Keys Profile Services bit fields
#define LINETHINGS_PSDI_PROFILE_SERVICE               0x00000001

// Length of Characteristic 5 in bytes
#define LINETHINGS_PSDI_PROFILE_CHAR_LEN           6

/*********************************************************************
 * TYPEDEFS
 */


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

extern bStatus_t LineThingsPsdiProfile_AddService( void );

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
extern bStatus_t LineThingsPsdiProfile_SetParameter( uint8 param, uint8 len, void *value );

/*
 * SimpleProfile_GetParameter - Get a Simple GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 */
extern bStatus_t LineThingsPsdiProfile_GetParameter( uint8 param, void *value );


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEGATTPROFILE_H */
