/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include "bcomdef.h"
#include "osal.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "peripheral.h"

#include "linethings_psdi.h"

/*********************************************************************
 * MACROS
 */
#define UUID_16BITS_L(x) ((x) & 0xFF),(((x) >> 8) & 0xFF)
#define UUID_128BITS_L(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) (x16), (x15), (x14), (x13), (x12), (x11), (x10), (x9), (x8), (x7), (x6), (x5), (x4), (x3), (x2), (x1)

/*********************************************************************
 * CONSTANTS
 */

#define LINETHINGS_PSDI_PROFILE_PSDI_CHAR_LEN    8

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 linethingsPsdiProfileServUUID[ATT_UUID_SIZE] =
{
 UUID_128BITS_L(0xe6, 0x25, 0x60, 0x1e, 0x9e, 0x55, 0x45, 0x97, 0xa5, 0x98, 0x76, 0x01, 0x8a, 0x0d, 0x29, 0x3d)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 linethingsPsdiProfileCharUUID[ATT_UUID_SIZE] =
{
 UUID_128BITS_L(0x26, 0xe2, 0xb1, 0x2b, 0x85, 0xf0, 0x4f, 0x3f, 0x9f, 0xdd, 0x91, 0xd1, 0x14, 0x27, 0x0e, 0x6e)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t linethingsPsdiProfileService = { ATT_UUID_SIZE, linethingsPsdiProfileServUUID };

// Simple Profile Characteristic 1 Properties
static uint8 linethingsPsdiProfileCharProps = GATT_PROP_READ;

// Characteristic 1 Value
static uint8 linethingsPsdiProfileChars[LINETHINGS_PSDI_PROFILE_CHAR_LEN] = { 0 };

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t linethingsPsdiProfileAttrTbl[] = {
  // Simple Profile Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&linethingsPsdiProfileService            /* pValue */
  },

    // Characteristic 1 Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &linethingsPsdiProfileCharProps
    },

      // Characteristic Value 1
      {
        { ATT_UUID_SIZE, linethingsPsdiProfileCharUUID },
        GATT_PERMIT_ENCRYPT_READ,
        0,
        linethingsPsdiProfileChars
      },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t linethingsPsdiProfile_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                          uint8_t *pValue, uint16_t *pLen, uint16_t offset, uint16_t maxLen, uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
CONST gattServiceCBs_t linethingsPsdiProfileCBs = {
  linethingsPsdiProfile_ReadAttrCB,  // Read callback function pointer
  NULL, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t LineThingsPsdiProfile_AddService(void){
    return GATTServApp_RegisterService( linethingsPsdiProfileAttrTbl, GATT_NUM_ATTRS( linethingsPsdiProfileAttrTbl ), GATT_MAX_ENCRYPT_KEY_SIZE, &linethingsPsdiProfileCBs );
}

/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
#if 0
bStatus_t LineThingsPsdiProfile_SetParameter( uint8 param, uint8 len, void *value ){
  bStatus_t ret = SUCCESS;
  switch ( param ){
    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return ( ret );
}

/*********************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t LineThingsPsdiProfile_GetParameter( uint8 param, void *value ){
  bStatus_t ret = SUCCESS;
  switch ( param ){
    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return ( ret );
}
#endif

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t linethingsPsdiProfile_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                          uint8_t *pValue, uint16_t *pLen,
                                          uint16_t offset, uint16_t maxLen,
                                          uint8_t method){
  bStatus_t status = SUCCESS;

  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 ){
    return ( ATT_ERR_ATTR_NOT_LONG );
  }

  if ( pAttr->type.len == ATT_UUID_SIZE ){
    // 128-bit UUID
      if( memcmp(pAttr->type.uuid, linethingsPsdiProfileCharUUID, ATT_UUID_SIZE) == 0 ){
          static uint8 isInit = 0x00;
          if( !isInit ){
              GAPRole_GetParameter(GAPROLE_BD_ADDR, linethingsPsdiProfileChars);
              isInit = 0x01;
          }
          *pLen = LINETHINGS_PSDI_PROFILE_CHAR_LEN;
          memcpy( pValue, pAttr->pValue, LINETHINGS_PSDI_PROFILE_CHAR_LEN );
      }else{
          *pLen = 0;
          status = ATT_ERR_ATTR_NOT_FOUND;
      }
  }
  else{
    // 16-bit UUID
    *pLen = 0;
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  return ( status );
}
