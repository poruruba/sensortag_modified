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

#include "linethings.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

#define UUID_16BITS_L(x) ((x) & 0xFF),(((x) >> 8) & 0xFF)
#define UUID_128BITS_L(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16) (x16), (x15), (x14), (x13), (x12), (x11), (x10), (x9), (x8), (x7), (x6), (x5), (x4), (x3), (x2), (x1)

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 linethingsProfileServUUID[ATT_UUID_SIZE] =
{
 // cf4752b9-8525-4bb4-87f9-bf3be7af93ed
 UUID_128BITS_L(0xcf, 0x47, 0x52, 0xb9, 0x85, 0x25, 0x4b, 0xb4, 0x87, 0xf9, 0xbf, 0x3b, 0xe7, 0xaf, 0x93, 0xed)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 linethingsProfileServiceUuidCharUUID[ATT_UUID_SIZE] =
{
 // E9062E71-9E62-4BC6-B0D3-35CDCD9B027B
 UUID_128BITS_L(0xE9, 0x06, 0x2E, 0x71, 0x9E, 0x62, 0x4B, 0xC6, 0xB0, 0xD3, 0x35, 0xCD, 0xCD, 0x9B, 0x02, 0x7B)
};
// Characteristic 2 UUID: 0xFFF1
CONST uint8 linethingsProfileBeaconHwidCharUUID[ATT_UUID_SIZE] =
{
 // E9062E72-9E62-4BC6-B0D3-35CDCD9B027B
 UUID_128BITS_L(0xE9, 0x06, 0x2E, 0x72, 0x9E, 0x62, 0x4B, 0xC6, 0xB0, 0xD3, 0x35, 0xCD, 0xCD, 0x9B, 0x02, 0x7B)
};
// Characteristic 3 UUID: 0xFFF1
CONST uint8 linethingsProfileSwitchCharUUID[ATT_UUID_SIZE] =
{
 // E9062E73-9E62-4BC6-B0D3-35CDCD9B027B
 UUID_128BITS_L(0xE9, 0x06, 0x2E, 0x73, 0x9E, 0x62, 0x4B, 0xC6, 0xB0, 0xD3, 0x35, 0xCD, 0xCD, 0x9B, 0x02, 0x7B)
};
// Characteristic 4 UUID: 0xFFF1
CONST uint8 linethingsProfileNotifyCharUUID[ATT_UUID_SIZE] =
{
 // 62FBD229-6EDD-4D1A-B554-5C4E1BB29169
 UUID_128BITS_L(0x62, 0xFB, 0xD2, 0x29, 0x6E, 0xDD, 0x4D, 0x1A, 0xB5, 0x54, 0x5C, 0x4E, 0x1B, 0xB2, 0x91, 0x69)
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
static CONST gattAttrType_t linethingsProfileService = { ATT_UUID_SIZE, linethingsProfileServUUID };

// Simple Profile Characteristic 1 Properties
static uint8 linethingsProfileServiceUuidCharProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 1 Value
static uint8 linethingsProfileServiceUuidChars[LINETHINGS_PROFILE_SERVICEUUID_CHAR_LEN] = { 0 };

// Simple Profile Characteristic 2 Properties
static uint8 linethingsProfileBeaconHwidCharProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 2 Value
static uint8 linethingsProfileBeaconHwidChars[LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN] = { 0 };

// Simple Profile Characteristic 3 Properties
static uint8 linethingsProfileSwitchCharProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 3 Value
static uint8 linethingsProfileSwitchChars[LINETHINGS_PROFILE_SWITCH_CHAR_LEN] = { 0 };

// Simple Profile Characteristic 4 Properties
static uint8 linethingsProfileNotifyCharProps = GATT_PROP_NOTIFY;

// Characteristic 4 Value
static uint8 linethingsProfileNotifyChar = 0;

// Simple Profile Characteristic 4 Configuration Each client has its own
// instantiation of the Client Characteristic Configuration. Reads of the
// Client Characteristic Configuration only shows the configuration for
// that client and writes only affect the configuration of that client.
static gattCharCfg_t *linethingsProfileNotifyCharConfig;

uint8 linethingsChangeFlag = 0;

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t linethingsProfileAttrTbl[] = {
  // Simple Profile Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&linethingsProfileService            /* pValue */
  },

    // Characteristic 1 Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &linethingsProfileServiceUuidCharProps
    },

      // Characteristic Value 1
      {
        { ATT_UUID_SIZE, linethingsProfileServiceUuidCharUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        linethingsProfileServiceUuidChars
      },

      // Characteristic 2 Declaration
      {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &linethingsProfileBeaconHwidCharProps
      },

        // Characteristic Value 2
        {
          { ATT_UUID_SIZE, linethingsProfileBeaconHwidCharUUID },
          GATT_PERMIT_READ | GATT_PERMIT_WRITE,
          0,
          linethingsProfileBeaconHwidChars
        },

        // Characteristic 3 Declaration
        {
          { ATT_BT_UUID_SIZE, characterUUID },
          GATT_PERMIT_READ,
          0,
          &linethingsProfileSwitchCharProps
        },

          // Characteristic Value 3
          {
            { ATT_UUID_SIZE, linethingsProfileSwitchCharUUID },
            GATT_PERMIT_READ | GATT_PERMIT_WRITE,
            0,
            linethingsProfileSwitchChars
          },

    // Characteristic 4 Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },
        GATT_PERMIT_READ,
        0,
        &linethingsProfileNotifyCharProps
    },

    // Characteristic Value 4
    {
        { ATT_UUID_SIZE, linethingsProfileNotifyCharUUID },
        0,
        0,
        &linethingsProfileNotifyChar
    },

    // Characteristic 4 configuration
    {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8 *)&linethingsProfileNotifyCharConfig
    },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t linethingsProfile_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                          uint8_t *pValue, uint16_t *pLen, uint16_t offset, uint16_t maxLen, uint8_t method);
static bStatus_t linethingsProfile_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t len, uint16_t offset, uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
CONST gattServiceCBs_t linethingsProfileCBs = {
  linethingsProfile_ReadAttrCB,  // Read callback function pointer
  linethingsProfile_WriteAttrCB, // Write callback function pointer
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
bStatus_t LineThingsProfile_AddService(const uint8 *serviceuuid, const uint8 *hwid, const uint8* line_switch){
    memcpy(linethingsProfileServiceUuidChars, serviceuuid, ATT_UUID_SIZE);
    memcpy(linethingsProfileBeaconHwidChars, hwid, LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN);
    memcpy(linethingsProfileSwitchChars, line_switch, LINETHINGS_PROFILE_SWITCH_CHAR_LEN);

    // Allocate Client Characteristic Configuration table
    linethingsProfileNotifyCharConfig = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) * linkDBNumConns );
    if ( linethingsProfileNotifyCharConfig == NULL ){
      return ( bleMemAllocError );
    }

    // Initialize Client Characteristic Configuration attributes
    GATTServApp_InitCharCfg( INVALID_CONNHANDLE, linethingsProfileNotifyCharConfig );

    return GATTServApp_RegisterService( linethingsProfileAttrTbl, GATT_NUM_ATTRS( linethingsProfileAttrTbl ), GATT_MAX_ENCRYPT_KEY_SIZE, &linethingsProfileCBs );
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
bStatus_t LineThingsProfile_SetParameter( uint8 param, uint8 len, void *value ){
  bStatus_t ret = SUCCESS;
  switch ( param ){
  case LINETHINGS_PROFILE_SERVICEUUID_CHAR:
      if ( len <= LINETHINGS_PROFILE_SERVICEUUID_CHAR_LEN ){
        memcpy( linethingsProfileServiceUuidChars, value, len );
      }
      else{
        ret = bleInvalidRange;
      }
      break;
  case LINETHINGS_PROFILE_BEACONHWID_CHAR:
      if ( len <= LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN ){
        memcpy( linethingsProfileBeaconHwidChars, value, len );
      }
      else{
        ret = bleInvalidRange;
      }
      break;
  case LINETHINGS_PROFILE_SWITCH_CHAR:
      if ( len <= LINETHINGS_PROFILE_SWITCH_CHAR_LEN ){
        memcpy( linethingsProfileSwitchChars, value, len );
      }
      else{
        ret = bleInvalidRange;
      }
      break;
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
bStatus_t LineThingsProfile_GetParameter( uint8 param, void *value ){
  bStatus_t ret = SUCCESS;
  switch ( param ){
  case LINETHINGS_PROFILE_SERVICEUUID_CHAR:
    memcpy( value, linethingsProfileServiceUuidChars, LINETHINGS_PROFILE_SERVICEUUID_CHAR_LEN );
    break;
  case LINETHINGS_PROFILE_BEACONHWID_CHAR:
    memcpy( value, linethingsProfileBeaconHwidChars, LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN );
    break;
  case LINETHINGS_PROFILE_SWITCH_CHAR:
    memcpy( value, linethingsProfileSwitchChars, LINETHINGS_PROFILE_SWITCH_CHAR_LEN );
    break;
  default:
    ret = INVALIDPARAMETER;
    break;
  }

  return ( ret );
}


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
static bStatus_t linethingsProfile_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
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
      if( memcmp(pAttr->type.uuid, linethingsProfileServiceUuidCharUUID, ATT_UUID_SIZE) == 0 ){
          *pLen = LINETHINGS_PROFILE_SERVICEUUID_CHAR_LEN;
          memcpy( pValue, pAttr->pValue, LINETHINGS_PROFILE_SERVICEUUID_CHAR_LEN );
      }
      else if( memcmp(pAttr->type.uuid, linethingsProfileBeaconHwidCharUUID, ATT_UUID_SIZE) == 0 ){
          *pLen = LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN;
          memcpy( pValue, pAttr->pValue, LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN );
      }
      else if( memcmp(pAttr->type.uuid, linethingsProfileSwitchCharUUID, ATT_UUID_SIZE) == 0 ){
          *pLen = LINETHINGS_PROFILE_SWITCH_CHAR_LEN;
          memcpy( pValue, pAttr->pValue, LINETHINGS_PROFILE_SWITCH_CHAR_LEN );
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

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t linethingsProfile_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t len, uint16_t offset, uint8_t method){
  bStatus_t status = SUCCESS;

  if ( pAttr->type.len == ATT_BT_UUID_SIZE ){
      // 16-bit UUID
      uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
      switch ( uuid ){
      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len, offset, GATT_CLIENT_CFG_NOTIFY );
        break;

      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
      }
  }else{
      if( memcmp(pAttr->type.uuid, linethingsProfileServiceUuidCharUUID, ATT_UUID_SIZE) == 0 ){
          if( len > LINETHINGS_PROFILE_SERVICEUUID_CHAR_LEN ){
              status = ATT_ERR_INVALID_VALUE_SIZE;
          }else{
              memcpy( pAttr->pValue, pValue, len );
              linethingsChangeFlag = LINETHINGS_CHGFLG_SERVICEUUID;
          }
      }
      else if( memcmp(pAttr->type.uuid, linethingsProfileBeaconHwidCharUUID, ATT_UUID_SIZE) == 0 ){
          if( len > LINETHINGS_PROFILE_BEACONHWID_CHAR_LEN ){
              status = ATT_ERR_INVALID_VALUE_SIZE;
          }else{
              memcpy( pAttr->pValue, pValue, len );
              linethingsChangeFlag = LINETHINGS_CHGFLG_BEACONHWID;
          }
      }
      else if( memcmp(pAttr->type.uuid, linethingsProfileSwitchCharUUID, ATT_UUID_SIZE) == 0 ){
          if( len > LINETHINGS_PROFILE_SWITCH_CHAR_LEN ){
              status = ATT_ERR_INVALID_VALUE_SIZE;
          }else{
              memcpy( pAttr->pValue, pValue, len );
              linethingsChangeFlag = LINETHINGS_CHGFLG_SWITCH;
          }
      }else{
          status = ATT_ERR_ATTR_NOT_FOUND;
      }
  }

  return ( status );
}
