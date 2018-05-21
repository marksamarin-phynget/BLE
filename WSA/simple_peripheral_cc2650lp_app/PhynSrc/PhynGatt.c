#include <string.h>

#include "bcomdef.h"
#include "osal.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include <string.h>
#include <stdio.h>


#include "PhynDefs.h"
#include "PhynSerial.h"
#include "PhynGatt.h"
#include  "PWD_Util.h"

// Simple Profile Service attribute
//uint8 PhynString1[512];

B32 bPartialMsgToAppInPrg;
U32 uPartialTxIdx = 0;

extern U32 uAppCRC, uBLEStackCRC,  uNVDCRC;

/*********************************************************************
 Phyn Profile Setup
*********************************************************************/
// PROFILE UUID: 0xFFFA
CONST uint8 PhynProfileUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(PHYNPROFILE_UUID), HI_UINT16(PHYNPROFILE_UUID)
};

static CONST gattAttrType_t PhynProfileService = { ATT_BT_UUID_SIZE, PhynProfileUUID };
static gattCharCfg_t *PhynProfileChar1Config;


/*********************************************************************
 Setup attribute Read/Write CALLBACKS
*********************************************************************/
CONST gattServiceCBs_t PhynRdWrCBs =
{
  PhynProfile_ReadAttrCB,  // Read callback function pointer
  PhynProfile_WriteAttrCB, // Write callback function pointer
  NULL                     // Authorization callback function pointer
};

/*********************************************************************
 Setup possible Callbacks for when a characteristic value has changed
*********************************************************************/
typedef void (*PhynProfileChange_t)( uint8 paramID );

typedef struct
{
  PhynProfileChange_t        pfnValueChange;  // Called when characteristic value changes
} PhynProfile_AppCBs_t;
    
static PhynProfile_AppCBs_t *PhynProfile_AppCBs = NULL;   // No change callback 


#define GATT_PERMIT_RW  ( GATT_PERMIT_READ | GATT_PERMIT_WRITE)


#if 0
/*********************************************************************
 Program CRC -  PWD_PROGRAM_CRC                (0x81)
                PHYNPROFILE_PROGRAM_CRC_UUID    (0xFFE2)
*********************************************************************/
static  U8  sProgramCRCDesc[] = "CRCs (App, Stack, NVD)";
        //U16 uProgramCRC;
        U8     sCRCs[15];

CONST U8 Phyn_Program_CRC_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_PROGRAM_CRC_UUID), HI_UINT16(PHYNPROFILE_PROGRAM_CRC_UUID)
};

U8 PhynProgramCRCProps = GATT_PROP_READ;


/*********************************************************************
 Validation Seed -  PWD_VALIDATION_SEED                (0x82)
                    PHYNPROFILE_VALIDATION_SEED_UUID    (0xFFE3)
*********************************************************************/
static  U8 sValidationSeedDesc[] = "Validation Seed";
        U8 sValidationSeed[PWD_VALIDATION_SEED_SIZE] = { '\xFC','\xFD','\xFE','\xFF' };

CONST U8 Phyn_Validation_Seed_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_VALIDATION_SEED_UUID), HI_UINT16(PHYNPROFILE_VALIDATION_SEED_UUID)
};

U8 PhynValidationSeedProps = GATT_PROP_READ;

/*********************************************************************
 Validation Key -   PWD_VALIDATION_KEY                 (0x83)
                    PHYNPROFILE_VALIDATION_KEY_UUID     (0xFFE4)
*********************************************************************/
static  U8 sValidationKeyDesc[] = "Validation Key";
        U8 sValidationKey[PWD_VALIDATION_KEY_SIZE];

CONST U8 Phyn_Validation_Key_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_VALIDATION_KEY_UUID), HI_UINT16(PHYNPROFILE_VALIDATION_KEY_UUID)
};

U8 PhynValidationKeyProps = GATT_PROP_READ | GATT_PROP_WRITE;


/*********************************************************************
 BLE Status -  PWD_STATUS              (0x84)
               PHYNPROFILE_STATUS_UUID  (0xFFE5)
*********************************************************************/
static  U8 sStatusDesc[] = "Status Code";
        U8 cStatusCode;

CONST U8 Phyn_Status_Key_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_STATUS_UUID), HI_UINT16(PHYNPROFILE_STATUS_UUID)
};

U8 PhynStatusKeyProps = GATT_PROP_READ;

/*********************************************************************
 BLE Command -  PWD_COMMAND                (0x85)
                PHYNPROFILE_COMMAND_UUID    (0xFFE6)
*********************************************************************/
static  U8 sCommandDesc[] = "Command Code";
        U8 cCommandCode;

CONST U8 Phyn_Command_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_COMMAND_UUID), HI_UINT16(PHYNPROFILE_COMMAND_UUID)
};

U8 PhynCommandProps =  GATT_PROP_READ | GATT_PROP_WRITE;

#endif

/*********************************************************************
 Message Counter -  PWD_MESSAGE_COUNTER                (0x86)
                    PHYNPROFILE_MESSAGE_COUNTER_UUID    (0xFFE7)

*********************************************************************
#define PWD_MESSAGE_COUNTER_SIZE 1

static  U8 cMessageFromAppDesc[] = "Tx Counter";
        U8 cMessageFromAppCounter;

CONST U8 Phyn_Message_Counter_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_MESSAGE_COUNTER_UUID), HI_UINT16(PHYNPROFILE_MESSAGE_COUNTER_UUID)
};

U8 PhynMessageCounterProps = GATT_PROP_READ;
*/

/*********************************************************************
 Message -  MESSAGE_FROM_APP           (0x87)
            MESSAGE_FROM_APP_UUID     (0xFFE8)
*********************************************************************/
static  U8 sMsgFromAppDesc[] = "JNAP Command";
        U8 sMsgFromApp[MSG_FROM_APP_SIZE] = {"Ready"};

CONST U8 Msg_From_App_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MSG_FROM_APP_UUID), HI_UINT16(MSG_FROM_APP_UUID)
};

U8 cMsgFromAppProps = GATT_PROP_WRITE;

/*********************************************************************
 Response Counter -  MSG_TO_APP_COUNTER                (0x88)
                     MSG_TO_APP_COUNTER_UUID           (0xFFE9)
*********************************************************************/
static  U8 sMsgToAppCtrDesc[] = "Chunk Counter";
        U8 cMsgToAppCtr;

CONST U8 Msg_To_App_Counter_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MSG_TO_APP_COUNTER_UUID), HI_UINT16(MSG_TO_APP_COUNTER_UUID)
};

U8 cMsgToAppCtrProps = GATT_PROP_READ;

/*********************************************************************
 Message -  MSG_TO_APP                       (0x89)
            MSG_TO_APP_UUID                 (0xFFEA)
*********************************************************************/

static  U8 sMsgToAppDesc[] = "JNAP Response";
        U8 sMsgToApp[MSG_TO_APP_SIZE] = {"No Msgs Yet"};

CONST U8 Msg_To_App_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MSG_TO_APP_UUID), HI_UINT16(MSG_TO_APP_UUID)
};

U8 cMsgToAppProps = GATT_PROP_READ;




/*********************************************************************
 Setup Phyn Profile
*********************************************************************/
static gattAttribute_t PhynProfileAttrTbl[] = 
{
    // PHYN Profile Service
    { 
       { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type (=0xFFFA) */
         GATT_PERMIT_READ,                         /* permissions */
         0,                                        /* handle */
        (uint8 *)&PhynProfileService            /* pValue */
    },
#if 0
      /*************************************************************************************************
       Program CRC
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynProgramCRCProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Program_CRC_UUID },
             GATT_PERMIT_READ, 0,  sCRCs
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sProgramCRCDesc
      },
    
      /*************************************************************************************************
       Validation Seed
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynValidationSeedProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Validation_Seed_UUID },
             GATT_PERMIT_READ, 0,  sValidationSeed
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sValidationSeedDesc
      },
      /*************************************************************************************************
       Validation Key
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynValidationKeyProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Validation_Key_UUID },
             GATT_PERMIT_RW, 0,  sValidationKey
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sValidationKeyDesc
      },
    
      /*************************************************************************************************
       Status Code
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynStatusKeyProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Status_Key_UUID },
             GATT_PERMIT_READ, 0,  &cStatusCode
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sStatusDesc
      },
    
      /*************************************************************************************************
       Command Code
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynCommandProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Command_UUID },
             GATT_PERMIT_RW, 0,  &cStatusCode
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sCommandDesc
      },
    
      /*************************************************************************************************
       Message Counter
      *************************************************************************************************
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &PhynMessageCounterProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Phyn_Message_Counter_UUID },
           GATT_PERMIT_READ, 0,  &cMessageCounter
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sMessageCounterDesc
      },
    */
#endif

      /*************************************************************************************************
       Message From App Body
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0, &cMsgFromAppProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Msg_From_App_UUID },
             GATT_PERMIT_WRITE, 0,  sMsgFromApp
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sMsgFromAppDesc
      },
    
      /*************************************************************************************************
       Message to App Counter
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &cMsgToAppCtrProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Msg_To_App_Counter_UUID },
             GATT_PERMIT_READ, 0,  &cMsgToAppCtr
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sMsgToAppCtrDesc
      },
    
      /*************************************************************************************************
       Message (Data) to APP Body
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &cMsgToAppProps
      },
    
      // Value
      {
           { ATT_BT_UUID_SIZE, Msg_To_App_UUID },
             GATT_PERMIT_READ, 0,  sMsgToApp
       },
    
      // User Description
      {
          { ATT_BT_UUID_SIZE, charUserDescUUID },
            GATT_PERMIT_READ, 0, sMsgToAppDesc
      },
};

/***********************************************************************************************
 Phyn_AddServices
***********************************************************************************************/
bStatus_t Phyn_AddServices()
{
    uint8 status;
    
    // Allocate Client Characteristic Configuration table
    PhynProfileChar1Config = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) *
                                                              linkDBNumConns );
    if ( PhynProfileChar1Config == NULL )
    {
      return ( bleMemAllocError );
    }
    
    // Initialize Client Characteristic Configuration attributes
    GATTServApp_InitCharCfg( INVALID_CONNHANDLE, PhynProfileChar1Config );
    
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( PhynProfileAttrTbl,
                                          GATT_NUM_ATTRS( PhynProfileAttrTbl ),
                                          GATT_MAX_ENCRYPT_KEY_SIZE,
                                          &PhynRdWrCBs);
    
    return ( status );
}


 /**********************************************************************************
PhynProfile_ReadAttrCB
   Process read requests from CLE Client.

   Value to send is loaded here when an attribute request comes in over BLE

  connHandle    - connection message was received on  (NOT USED)
  pAttr         - pointer to attribute
  pValue        - pointer to data to be read
  len           - length of read data
  offset        - offset of the first octet to be written
  maxLen -      - maximum length of data to be read
  method        - type of write message (NOT USED)

  return        SUCCESS, ATT_ERR_ATTR_NOT_FOUND, ATT_ERR_ATTR_NOT_LONG,
                ATT_ERR_INVALID_VALUE
**********************************************************************************/
static bStatus_t PhynProfile_ReadAttrCB(uint16_t    connHandle, gattAttribute_t  *pAttr,
                                        uint8_t     *pValue,    uint16_t         *pLen,
                                        uint16_t    offset,     uint16_t         maxLen,
                                        uint8_t     method)
{
    bStatus_t status = ATT_ERR_UNSUPPORTED_REQ;
    
    // Make sure it's not a blob operation (no attributes in the profile are long)
    if ( offset > 0 )
        return ( ATT_ERR_ATTR_NOT_LONG );

    
    if ( pAttr->type.len == ATT_BT_UUID_SIZE )
    {
    
      uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]); // 16-bit UUID
    
    
      switch ( uuid )
      {
        // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
        // gattserverapp handles those reads
#if 0
        /***********************************************************************************************
           Program CRC
        ***********************************************************************************************/
        case PHYNPROFILE_PROGRAM_CRC_UUID:
            if (PWD_PROG_CRC_SIZE <= maxLen)
            {

                UnsHalfToAscii(uAppCRC,      (char *)sCRCs);         sCRCs[4] = ' ';
                UnsHalfToAscii(uBLEStackCRC, (char *)&sCRCs[5]);    sCRCs[9] = ' ';
                UnsHalfToAscii(uNVDCRC,      (char *)&sCRCs[10]);

                *pLen = 15;                                       //      Send only what is left
                memcpy(pValue, sCRCs, *pLen);

                //*pLen = PWD_PROG_CRC_SIZE;
                //VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_PROG_CRC_SIZE );

                status = SUCCESS;
             }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
        break;
    
        /***********************************************************************************************
          Validation Seed
        ***********************************************************************************************/
        case PHYNPROFILE_VALIDATION_SEED_UUID:
            if (PWD_VALIDATION_SEED_SIZE <= maxLen)
            {
                *pLen = PWD_VALIDATION_SEED_SIZE;
                VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_VALIDATION_SEED_SIZE );
                status = SUCCESS;
             }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
    
        break;
    
        /***********************************************************************************************
         Validation Key
        ***********************************************************************************************/
        case PHYNPROFILE_VALIDATION_KEY_UUID:
            if (PWD_VALIDATION_KEY_SIZE <= maxLen)
            {
                *pLen = PWD_VALIDATION_KEY_SIZE;
                VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_VALIDATION_KEY_SIZE );
                status = SUCCESS;
              }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
         break;


         /***********************************************************************************************
          Status
         ***********************************************************************************************/
         case PHYNPROFILE_STATUS_UUID:
             if (PWD_STATUS_SIZE <= maxLen)
             {
                 *pLen = PWD_STATUS_SIZE;
                 VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_STATUS_SIZE );
                 status = SUCCESS;
               }
              else
                 status = ATT_ERR_INVALID_VALUE_SIZE;
          break;

          /***********************************************************************************************
           Command
          ***********************************************************************************************/
          case PHYNPROFILE_COMMAND_UUID:
              if (PWD_COMMAND_SIZE <= maxLen)
              {
                  *pLen = PWD_COMMAND_SIZE;
                  VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_COMMAND_SIZE );
                  status = SUCCESS;
                }
               else
                  status = ATT_ERR_INVALID_VALUE_SIZE;
           break;


           /***********************************************************************************************
            Message Counter
           **********************************************************************************************
           case PHYNPROFILE_MESSAGE_COUNTER_UUID:
               if (PWD_MESSAGE_COUNTER_SIZE <= maxLen)
               {
                   *pLen = PWD_MESSAGE_COUNTER_SIZE;
                   VOID memcpy( pValue, (uint8 *)pAttr->pValue, PWD_MESSAGE_COUNTER_SIZE );
                   status = SUCCESS;
                 }
                else
                   status = ATT_ERR_INVALID_VALUE_SIZE;
            break;

         */


        /***********************************************************************************************
         Message From App to MT
        ***********************************************************************************************
        case MSG_FROM_APP:
            if (PWD_RESPONSE_SIZE >= maxLen)
            {
                *pLen = PWD_RESPONSE_SIZE;
                VOID memcpy( pValue, (uint8 *)pAttr->pValue, maxLen );
                status = SUCCESS;
            }
            else
                status = ATT_ERR_INVALID_VALUE_SIZE;
        break;
        */
#endif

        /***********************************************************************************************
          Message to APP Counter
              This value increments when messages from Client for WiFi board are processed
        ***********************************************************************************************/
        case MSG_TO_APP_COUNTER_UUID:
            if (MSG_TO_APP_CTR_SIZE <= maxLen)
            {
                 *pLen = MSG_TO_APP_CTR_SIZE;
                 VOID memcpy( pValue, (uint8 *)pAttr->pValue, MSG_TO_APP_CTR_SIZE );
                 status = SUCCESS;
              }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
         break;


        /***********************************************************************************************
         Message to APP from MT
             Messages are broken up into ATT_MTU size chunks if too large.
             ATT_MTU starts off at 22 and can be increased by client only.
        ***********************************************************************************************/

        case MSG_TO_APP_UUID:
            if (MSG_TO_APP_SIZE >= maxLen)
            {
                U32 uLen;

                extern B32 bAppMessageRead;

                uLen = strlen((const char *)&sMsgToApp[uPartialTxIdx]);

                // Additional logic in case the messages coming into the serial port are larger than
                // the max client receive characteristic size
                if (uLen > maxLen)                                      //  IF remaining msg size > ATT_MTU
                {
                    bPartialMsgToAppInPrg = bTRUE;                      //      Set Partial message flag
                    *pLen = maxLen;                                     //      Send as much as we can (ATT_MTU)
                    memcpy(pValue, &sMsgToApp[uPartialTxIdx], *pLen);   // Data transfer for BLE Tx
                    uPartialTxIdx += maxLen;                            // Update string pointer for next chunk
                    cMsgToAppCtr++;                                     //      Increment message counter so app knows more is coming
                }


                else                                                    // ELSE - Last chunk
                {
                    *pLen = uLen;                                       //      Send only what is left
                    memcpy(pValue, &sMsgToApp[uPartialTxIdx], *pLen);   //      Data transfer for BLE Tx
                    bPartialMsgToAppInPrg = bFALSE;                     //      Clear Partial Message Flag
                    bAppMessageRead = bTRUE;                            //      Signal we are done, can accept another message from MT
                    uPartialTxIdx=0;                                    //      Reset chunk index
                    sMsgToApp[0]= 0;                                    //      Clear message
                }


                status = SUCCESS;

                /*
                 *pLen = (maxLen > uLen) ? uLen : maxLen;
                  VOID memcpy( pValue, (uint8 *)pAttr->pValue, *pLen );
                  status = SUCCESS;
                  extern B32 bAppMessageRead;
                  bAppMessageRead = bTRUE;
                */

              }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
         break;

          
         /***********************************************************************************************
           Should never get here
         ***********************************************************************************************/
        default:
          *pLen = 0;
          status = ATT_ERR_ATTR_NOT_FOUND;
        break;
      }
    }
    else
    {
      // 128-bit UUID
      *pLen = 0;
      status = ATT_ERR_INVALID_HANDLE;
    }
    
    return ( status );
}

/**********************************************************************************
PhynProfile_WriteAttrCB
   Validate and set attribute data after receiving write request from client.
    Called froam a list of function pointers declared for this profile.
    Function parameters defined in gattServiceCBs_t.

  connHandle    - connection message was received on  (NOT USED)
  pAttr         - pointer to attribute
  pValue        - pointer to data to be written
  len           - length of data
  offset        - offset of the first octet to be written
  method        - type of write message (NOT USED)

  return        SUCCESS, ATT_ERR_ATTR_NOT_FOUND, ATT_ERR_ATTR_NOT_LONG,
                ATT_ERR_INVALID_VALUE
**********************************************************************************/
static bStatus_t PhynProfile_WriteAttrCB(uint16_t connHandle,   gattAttribute_t  *pAttr,
                                         uint8_t  *pValue,      uint16_t         len,
                                         uint16_t offset,       uint8_t          method)
{
    bStatus_t status = ATT_ERR_UNSUPPORTED_REQ;
    U8 uChangedValueID = 0xFF;                     // Tells notify function what was changed

    
    if ( pAttr->type.len == ATT_BT_UUID_SIZE )
    {
      // 16-bit UUID
      uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
      switch ( uuid )
      {
    
          /***********************************************************************************************
             Program CRC
          ***********************************************************************************************/
          // READ ONLY
    
         /***********************************************************************************************
                Validation Seed
         ***********************************************************************************************/
          // READ ONLY
    
        /***********************************************************************************************
           Validation Key
        ***********************************************************************************************/
#if 0
        case PHYNPROFILE_VALIDATION_KEY_UUID:
    
              if ( offset == 0 )
              {
                    if ( len != PWD_VALIDATION_KEY_SIZE )
                    {
                        status = ATT_ERR_INVALID_VALUE_SIZE;
                    }
                    else
                    {
                        VOID memcpy( (uint8 *)pAttr->pValue, pValue, PWD_VALIDATION_KEY_SIZE );
                        uChangedValueID = PWD_VALIDATION_KEY;
                        status = SUCCESS;
                        Log_With_U32( "App CHANGED Validation Key to ", *(U32 *)pValue);
                    }
              }
              else
                status = ATT_ERR_ATTR_NOT_LONG;
    
        break;
    
        /***********************************************************************************************
           Command
        ***********************************************************************************************/
        case PHYNPROFILE_COMMAND_UUID:
    
              if ( offset == 0 )
              {
                    if ( len != PWD_COMMAND_SIZE )
                        status = ATT_ERR_INVALID_VALUE_SIZE;
                    else
                    {
                        VOID memcpy( (uint8 *)pAttr->pValue, pValue, PWD_COMMAND_SIZE );
                        uChangedValueID = PWD_COMMAND;
                        status = SUCCESS;
                        Log_With_U8( "App COMMAND:", *pValue);
                    }
              }
              else
                status = ATT_ERR_ATTR_NOT_LONG;
    
        break;
#endif
        /***********************************************************************************************
           Message from App
        ***********************************************************************************************/
        case MSG_FROM_APP_UUID:
    
              if ( offset == 0 )
              {
                    if ( len > MSG_FROM_APP_SIZE-1 )
                        status = ATT_ERR_INVALID_VALUE_SIZE;
                    else
                    {
                        VOID memcpy( (uint8 *)pAttr->pValue, pValue, len );
                        *(uint8 *)(pAttr->pValue + len) = 0;
                        uChangedValueID = MSG_FROM_APP;
                        status = SUCCESS;

                        // Signal
                        extern B32 bNewAppMsg;
                        bNewAppMsg = bTRUE;

                    }
              }
              else
                status = ATT_ERR_ATTR_NOT_LONG;
    
        break;
    
        /***********************************************************************************************
           Message Counter
        ***********************************************************************************************/
        // READ ONLY
    
        /***********************************************************************************************
           Response
        ***********************************************************************************************/
        // READ ONLY
    
    
    
    
        case GATT_CLIENT_CHAR_CFG_UUID:
          status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                   offset, GATT_CLIENT_CFG_NOTIFY );
          break;
          
        default:
          // Should never get here! (characteristics 2 and 4 do not have write permissions)
          status = ATT_ERR_ATTR_NOT_FOUND;
          break;
      }
    }
    else
    {
      // 128-bit UUID
      status = ATT_ERR_INVALID_HANDLE;
    }
    
    // If a characteristic value changed then callback function to notify application of change
    if ( (uChangedValueID != 0xFF ) && PhynProfile_AppCBs && PhynProfile_AppCBs->pfnValueChange )
    {
        PhynProfile_AppCBs->pfnValueChange( uChangedValueID );
    }
    
    return ( status );
}

#if 0
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
bStatus_t SimpleProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case SIMPLEPROFILE_CHAR1:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar1 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR2:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar2 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR3:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar3 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR4:
      if ( len == sizeof ( uint8 ) )
      {
        simpleProfileChar4 = *((uint8*)value);

        // See if Notification has been enabled
        GATTServApp_ProcessCharCfg( simpleProfileChar4Config, &simpleProfileChar4, FALSE,
                                    simpleProfileAttrTbl, GATT_NUM_ATTRS( simpleProfileAttrTbl ),
                                    INVALID_TASK_ID, simpleProfile_ReadAttrCB );
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR5:
      if ( len == SIMPLEPROFILE_CHAR5_LEN )
      {
        VOID memcpy( simpleProfileChar5, value, SIMPLEPROFILE_CHAR5_LEN );
      }
      else
      {
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
  PWD_GetParameter
      Get a Simple Profile parameter.

     param:    Profile parameter ID

     ValuePrt:    Pointer to data to put.  This is dependent on
               the parameter ID and WILL be cast to the appropriate
               data type (example: data type of uint16 will be cast to
               uint16 pointer).

     return:
 **********************************************************************/
bStatus_t PWD_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case SIMPLEPROFILE_CHAR1:
      *((uint8*)value) = simpleProfileChar1;
      break;

    case SIMPLEPROFILE_CHAR2:
      *((uint8*)value) = simpleProfileChar2;
      break;

    case SIMPLEPROFILE_CHAR3:
      *((uint8*)value) = simpleProfileChar3;
      break;

    case SIMPLEPROFILE_CHAR4:
      *((uint8*)value) = simpleProfileChar4;
      break;

    case SIMPLEPROFILE_CHAR5:
      VOID memcpy( value, simpleProfileChar5, SIMPLEPROFILE_CHAR5_LEN );
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return ( ret );
}
#endif
/***********************************************************************************************
 UNUSED/TEST
 ***********************************************************************************************/

#if 0
/*********************************************************************
 Setup Phyn Characteristic #1
*********************************************************************/

uint8 PhynProfileChar1 = 'Z';                                        // Characteristic 1 Value
uint8 PhynProfileChar1UserDesc[] = "Phyn #1";           // Simple Profile Characteristic 1 User Description

// Characteristic 1 UUID: 0xFFFB
CONST uint8 PhynProfilechar1UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(PHYNPROFILE_CHAR1_UUID), HI_UINT16(PHYNPROFILE_CHAR1_UUID)
};

U8 PhynProfileChar1Props = GATT_PROP_READ | GATT_PROP_WRITE;
#endif


#if 0
    /*************************************************************************************************
      Characteristic #1 Declaration - R/W Character
    *************************************************************************************************/

    //  Declaration
    {
        { ATT_BT_UUID_SIZE, characterUUID },                 // type=0x2803 (Standard Characteristic UUID)
          GATT_PERMIT_READ,  0,  &PhynProfileChar1Props
    },

    // Value
    {
        { ATT_BT_UUID_SIZE, PhynProfilechar1UUID },
          GATT_PERMIT_RW,  0, &PhynProfileChar1
    },

    // User Description
    {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
          GATT_PERMIT_READ,  0,  PhynProfileChar1UserDesc
    },
#endif

#if 0
        /***********************************************************************************************
           CHAR1
        ***********************************************************************************************/
        case PHYNPROFILE_CHAR1_UUID:

            // Validate Size
            if ( offset == 0 )
            {
                  if ( len != 1 )
                  {
                    status = ATT_ERR_INVALID_VALUE_SIZE;
                  }
            }
            else
            {
                status = ATT_ERR_ATTR_NOT_LONG;
            }

            // Write the value
            if ( status == SUCCESS )
            {
                  uint8 *pCurValue = (uint8 *)pAttr->pValue;
                  *pCurValue = pValue[0];
                  uChangedValueID = PHYNPROFILE_CHAR1;
            }

        break;
#endif
#if 0
      /***********************************************************************************************
         CHAR 1
      ***********************************************************************************************/
      case PHYNPROFILE_CHAR1_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
      break;
#endif
