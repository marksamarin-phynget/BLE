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
U32 uCharsLeft = 0;

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
//static gattCharCfg_t *PhynProfileChar1Config;


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
 Message -  MESSAGE_FROM_APP           (0x87)
            MESSAGE_FROM_APP_UUID     (0xFFE8)
*********************************************************************/
static  U8 sMsgFromAppDesc[] = "Command";
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
static  U8 sMsgToAppDesc[] = "Response";
        U8 sMsgToApp[MSG_TO_APP_SIZE] = {"No Msgs Yet"};

CONST U8 Msg_To_App_UUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(MSG_TO_APP_UUID), HI_UINT16(MSG_TO_APP_UUID)
};

U8 cMsgToAppProps = GATT_PROP_READ;


/*********************************************************************
 Timezone
*********************************************************************/
const   U8 TZN_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 0), HI_UINT16(GRP_UUID + 0) };
        U8 sTZN[FIELD_TIMEZONE_SIZE+1];
        U8 cTZN_Props = GATT_PROP_WRITE;
        U8 sTZN_Desc[]="Timezone";

/*********************************************************************
 Country Code
*********************************************************************/
const   U8 CCD_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 1), HI_UINT16(GRP_UUID + 1) };
        U8 sCCD[FIELD_COUNTRY_CODE_SIZE+1];
        U8 cCCD_Props = GATT_PROP_WRITE;

/*********************************************************************
 SSID
*********************************************************************/
const   U8 SSID_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 2), HI_UINT16(GRP_UUID + 2) };
        U8 sSSID[FIELD_SSID_SIZE+1];
        U8 cSSID_Props = GATT_PROP_WRITE;

/*********************************************************************
 Passphrase
*********************************************************************/
const   U8 PPH_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 3), HI_UINT16(GRP_UUID + 3) };
        U8 sPPH[FIELD_PASSPHRASE_SIZE+1];
        U8 cPPH_Props = GATT_PROP_WRITE;

/*********************************************************************
 Client ID
*********************************************************************/
const   U8 CLID_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 4), HI_UINT16(GRP_UUID + 4) };
        U8 sCLID[FIELD_CLIENT_ID_SIZE+1] = "Not Set";
        U8 cCLID_Props = GATT_PROP_WRITE;

/*********************************************************************
 Client Secret
*********************************************************************/
const   U8 CLS_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 5), HI_UINT16(GRP_UUID + 5) };
        U8 sCLS[FIELD_CLIENT_SECRET_SIZE+1] = "Not Set";
        U8 cCLS_Props = GATT_PROP_WRITE;

/*********************************************************************
 Cloud Environment ID
*********************************************************************/
const   U8 CLE_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 6), HI_UINT16(GRP_UUID + 6) };
        U8 sCLE[FIELD_CLOUD_ENV_SIZE+1] = "XXX";
        U8 cCLE_Props =  GATT_PROP_READ | GATT_PROP_WRITE;

/*********************************************************************
 Cloud API Key
*********************************************************************/
const   U8 CLA_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 7), HI_UINT16(GRP_UUID + 7) };
        U8 sCLA[FIELD_CLOUD_API_SIZE+1] = "Not Set";
        U8 cCLA_Props =  GATT_PROP_WRITE;

/*********************************************************************
 App Version
*********************************************************************/
const   U8 APV_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 8), HI_UINT16(GRP_UUID + 7) };
        U8 sAPV[FIELD_APP_VERSION_SIZE+1] = "XXXXXX";
        U8 cAPV_Props =  GATT_PROP_WRITE;

/*********************************************************************
 Command
*********************************************************************/
const   U8 CMD_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 9), HI_UINT16(GRP_UUID + 9) };
        U8 cCMD;
        U8 cCMD_Props =  GATT_PROP_WRITE;


/*********************************************************************
 Status
*********************************************************************/
const   U8 STA_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 10),  HI_UINT16(GRP_UUID + 10) };
        U8 cSTA;
        U8 cSTA_Props =  GATT_PROP_READ;

/*********************************************************************
 WiFi Firmware Version
*********************************************************************/
const   U8 FWV_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 11), HI_UINT16(GRP_UUID + 11) };
        U8 sFWV[FIELD_WIFI_VERSION_SIZE+1];
        U8 cFWV_Props =  GATT_PROP_READ;

/*********************************************************************
 Serial Number
*********************************************************************/
const   U8 SSN_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 12), HI_UINT16(GRP_UUID + 12) };
        U8 sSSN[FIELD_SERIAL_NUMBER_SIZE+1];
        U8 cSSN_Props =  GATT_PROP_READ;



/*********************************************************************
Scan Result Data
*********************************************************************/
const   U8 SRD_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 13), HI_UINT16(GRP_UUID + 13) };
        U8 sSR_Data[FIELD_SR_DATA_SIZE+1] = "No Scan Results";
        U8 cSRD_Props = GATT_PROP_READ;

/*********************************************************************
Scan Result Counter
*********************************************************************/
const   U8 SRC_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 14), HI_UINT16(GRP_UUID + 14) };
        U8 cSR_Count=0;                        //[FIELD_SR_COUNT_SIZE + 1] = "00";
        U8 cSRC_Props = GATT_PROP_READ;



/*********************************************************************
 Setup Phyn Profile
*********************************************************************/
static gattAttribute_t PhynProfileAttrTbl[] = 
{
    // PHYN Profile Service
    { 
       { ATT_BT_UUID_SIZE, primaryServiceUUID },   /* type (=0xFFFA) */
         GATT_PERMIT_READ,                         /* permissions */
         0,                                        /* handle */
        (uint8 *)&PhynProfileService               /* pValue */
    },


      /*************************************************************************************************
       Message From App Body
      *************************************************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0, &cMsgFromAppProps },      // Declaration
      { { ATT_BT_UUID_SIZE, Msg_From_App_UUID }, GATT_PERMIT_WRITE, 0,  sMsgFromApp },      // Value
      { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sMsgFromAppDesc},      // User Description
    
      /*************************************************************************************************
       Message to App Counter
      *************************************************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },  GATT_PERMIT_READ, 0,   &cMsgToAppCtrProps    },  // Declaration
      { { ATT_BT_UUID_SIZE, Msg_To_App_Counter_UUID }, GATT_PERMIT_READ, 0,  &cMsgToAppCtr },  // Value
      { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sMsgToAppCtrDesc      },  // User Description

    
      /*************************************************************************************************
       Message (Data) to APP Body
      *************************************************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID }, GATT_PERMIT_READ, 0,   &cMsgToAppProps },      // Declaration
      { { ATT_BT_UUID_SIZE, Msg_To_App_UUID }, GATT_PERMIT_READ, 0,  sMsgToApp      },      // Value
      { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sMsgToAppDesc  },      // User Description

      /*********************************************************************
       Timezone
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cTZN_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, TZN_UUID },         GATT_PERMIT_WRITE, 0, sTZN },          // Value
      { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0,  sTZN_Desc  },      // User Description



      /*********************************************************************
       Country Code
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCCD_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CCD_UUID },         GATT_PERMIT_WRITE, 0, sCCD },          // Value

      /*********************************************************************
       SSID
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSSID_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SSID_UUID },        GATT_PERMIT_WRITE, 0, sSSID },          // Value

      /*********************************************************************
       Passphrase
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cPPH_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, PPH_UUID },         GATT_PERMIT_WRITE, 0, sPPH },          // Value

      /*********************************************************************
       Client ID
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLID_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLID_UUID },        GATT_PERMIT_WRITE, 0, sCLID },          // Value

      /*********************************************************************
       Client Secret
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLS_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLS_UUID },         GATT_PERMIT_WRITE, 0, sCLS },          // Value

      /*********************************************************************
       Cloud Environment ID
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLE_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLE_UUID },         GATT_PERMIT_WRITE | GATT_PERMIT_READ, 0, sCLE },          // Value

      /*********************************************************************
       Cloud API Key
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLA_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLA_UUID },         GATT_PERMIT_WRITE, 0, sCLA },          // Value

      /*********************************************************************
       App Version
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cAPV_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, APV_UUID },         GATT_PERMIT_WRITE, 0, sAPV },          // Value

      /*********************************************************************
       Command
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCMD_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CMD_UUID },         GATT_PERMIT_WRITE, 0, &cCMD },          // Value

      /*********************************************************************
       Status
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSTA_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, STA_UUID },         GATT_PERMIT_READ, 0, &cSTA },          // Value

      /*********************************************************************
       Firmware Version
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cFWV_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, FWV_UUID },         GATT_PERMIT_READ, 0, sFWV },          // Value

      /*********************************************************************
       Serial Number
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSSN_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SSN_UUID },         GATT_PERMIT_READ, 0, sSSN },          // Value

      /*************************************************************************************************
        Scan Result messages
      ************************************************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSRD_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SRD_UUID },         GATT_PERMIT_READ, 0, sSR_Data },      // Value

      /*************************************************************************************************
        Scan Result Counter
      ************************************************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSRC_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SRC_UUID },         GATT_PERMIT_READ, 0, (U8 *)&cSR_Count }, // Value

};

/***********************************************************************************************
 InitAppFields
***********************************************************************************************/
void InitAppFields()
{

    const I8 sInitField[4] = "xxx";
    extern B32 bAppMessageRead, bScanResultRead;


    strcpy(sTZN, sInitField);
    strcpy(sCCD, "xx");
    strcpy(sSSID, sInitField);
    strcpy(sPPH, sInitField);
    strcpy(sCLID, sInitField);
    strcpy(sCLS, sInitField);
    strcpy(sCLE, sInitField);
    strcpy(sCLA, sInitField);
    strcpy(sAPV, sInitField);
    cCMD = cSTA =  ' ';
    strcpy(sFWV, sInitField);
    strcpy(sSSN, sInitField);
    strcpy(sSR_Data, sInitField);
    cSR_Count = 0;

    bAppMessageRead = bTRUE;
    bScanResultRead = bTRUE;

}


/***********************************************************************************************
 Phyn_AddServices
***********************************************************************************************/
bStatus_t Phyn_AddServices()
{
    uint8 status;

    InitAppFields();
    
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( PhynProfileAttrTbl,
                                          GATT_NUM_ATTRS( PhynProfileAttrTbl ),
                                          GATT_MAX_ENCRYPT_KEY_SIZE,
                                          &PhynRdWrCBs);

    return ( status );
}


 /**********************************************************************************
PhynProfile_ReadAttrCB
   Process read requests from BLE Client.

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
    U8 Gatt_SR_Tx(U32 uIdx, U8 *pValue, U8 *sTxField, U8 uMaxLen, U16 *pChunkSize);
    
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

            extern B32 bAppMessageRead;


            // Send field in chunks
            uCharsLeft = Gatt_SR_Tx(PARTIAL_FIELD_APP_COMMAND,  pValue,  pAttr->pValue, maxLen, pLen);
            status = SUCCESS;

            // If last chunk - mark field as read
            if (uCharsLeft == 0)
            {
                bAppMessageRead = bTRUE;
                sMsgToApp[0] = 0;
             }

         break;

         /***********************************************************************************************
           WiFi Firmware Version
         ***********************************************************************************************/
         case GRP_UUID + 11:
             *((uint8 *)pAttr->pValue + FIELD_WIFI_VERSION_SIZE) = 0;
             *pLen = strlen((uint8 *)pAttr->pValue);
             VOID memcpy(pValue, (uint8 *)pAttr->pValue, FIELD_WIFI_VERSION_SIZE);
             status = SUCCESS;
         break;

         /***********************************************************************************************
           PWD Serial Number
         ***********************************************************************************************/
         case GRP_UUID + 12:
             *((uint8 *)pAttr->pValue + FIELD_SERIAL_NUMBER_SIZE) = 0;
             *pLen = strlen((uint8 *)pAttr->pValue);
             VOID memcpy(pValue, (uint8 *)pAttr->pValue, FIELD_SERIAL_NUMBER_SIZE);
             status = SUCCESS;
         break;

         /***********************************************************************************************
           Scan Results Data: GRP_UUID + 13
         ***********************************************************************************************/
         case GRP_UUID + 13:

             extern B32 bScanResultRead;

             // Send field to Client in chunks
            uCharsLeft = Gatt_SR_Tx(PARTIAL_FIELD_SCAN_RESULT,  pValue,  pAttr->pValue, maxLen, pLen);
            status = SUCCESS;

            // If last chunk - mark field as read
            if (uCharsLeft == 0)
            {
                bScanResultRead = bTRUE;     // Indicates when field is completely read by client
                sSR_Data[0] = 0;
            }

         break;

         /***********************************************************************************************
           Scan Results Count: GRP_UUID + 14
         ***********************************************************************************************/
         case GRP_UUID + 14:
             *pLen = FIELD_SR_COUNT_SIZE;
             VOID memcpy(pValue, (uint8 *)pAttr->pValue, FIELD_SR_COUNT_SIZE );
             status = SUCCESS;
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

/*********************************************************************
 Handle BLE Client reads of oversize characteristics
 Sends data in chunks until finished
 One chunk per call is sent, each tx will trigger another BLE callback
 that will get to here

 pValue         BLE transmit buffer
 sTxField:      Full message to send

 *pLen = Gatt_SR_Tx(PARTIAL_FIELD_APP_COMMAND,  pValue,  "01234567890123456789012345", maxLen);

**********************************************************************/
U8 Gatt_SR_Tx(U32 uIdx, U8 *pValue, U8 *sTxField, U8 uMaxLen, U16 *pChunkSize)
{
   static U8 uIdxCtr[NUM_PARTIAL_FIELDS];

   U8 uCharsLeft;

   if (uIdx >= NUM_PARTIAL_FIELDS) return 0;

   uCharsLeft = strlen((const char *)&sTxField[uIdxCtr[uIdx]]);  // Caller must ensure terminator in place!


   // Additional logic in case the messages coming into the serial port are larger than
   // the max client receive characteristic size
   if (uCharsLeft > uMaxLen)                                     //  IF remaining msg size > ATT_MTU
   {
       *pChunkSize = uMaxLen;
       memcpy(pValue, &sTxField[uIdxCtr[uIdx]], *pChunkSize);    //      Data transfer for BLE Tx
       uIdxCtr[uIdx] += uMaxLen;                                 //      Update string pointer for next chunk
       return uCharsLeft - uMaxLen;
   }
   else                                                         // ELSE - Last chunk
   {
       *pChunkSize = uCharsLeft;
       memcpy(pValue, &sTxField[uIdxCtr[uIdx]], *pChunkSize);   //      Data transfer for BLE Tx
       uIdxCtr[uIdx]=0;                                         //      Reset chunk index
       return 0;
   }
}


#define APP_FIELD_TERMINATOR ('\x00')
#define DEBUG_APP_FIELD_TERMINATOR (';')


B32 bNewAppField = bFALSE;

/***********************************************************************************************
 Update_Attribute_From_App

     Generic handling for App updated fields.

     Loads oversize data from BLE client into destination.

     Sets global flag bNewAppField to notify g=higher layers that the multiple-packet transfer
     is complete.   Loads and loads an alert message into sAppUpdateMsg for serial transmit.
 ***********************************************************************************************/
bStatus_t Update_Attribute_From_App(U16 uOffset, gattAttribute_t *GattAttrPtr,  U8 *vpValue, U16 uLen, U32 uMaxSize, I8 *sMsgPfx)
{
    static U32 uIdx = 0;   // chunk writes must be over same attribute - do not mix writes
    U8 *cP;
    extern I8 sAppUpdateMsg[];

    if ((uLen + uIdx) > uMaxSize + 1)          // add 1 for terminator
    {
        uIdx = 0;
        return ATT_ERR_INVALID_VALUE_SIZE;
    }


    // Setup current write pointer to account for chunk writes
    cP = (U8 *)GattAttrPtr->pValue + uIdx;
    uIdx += uLen;

    while (uLen)
    {
        #ifdef DEBUG_MODE
            if (*vpValue != APP_FIELD_TERMINATOR && *vpValue != DEBUG_APP_FIELD_TERMINATOR)
        #else
            if (*vpValue != APP_FIELD_TERMINATOR)
        #endif
        {
            *cP++ = *vpValue++;
            *cP=0;                      // terminator
            uLen--;
        }
        else
        {
            *cP = uLen = uIdx = 0;

            strcpy(sAppUpdateMsg, "Field Change - ");
            strcat(sAppUpdateMsg, sMsgPfx);

            if ((strlen(sAppUpdateMsg) + strlen((I8 *)GattAttrPtr->pValue)) < SIZEOF_APP_UPDATE_MSG)
                strcat(sAppUpdateMsg, (I8 *)GattAttrPtr->pValue);
            else
                strcat(sAppUpdateMsg, "** MESSAGE SIZE ERROR **");

            bNewAppField = bTRUE;
        }
    }

    return SUCCESS;



#if 0
     if (uOffset == 0 )
     {

        if ((uOffset + uIdx) > uMaxSize)
            return ATT_ERR_INVALID_VALUE_SIZE;
        else
        {


            memcpy((uint8 *)GattAttrPtr->pValue + uIdx, vpValue, uLen);
           // *(uint8 *)(GattAttrPtr->pValue + uLen + uOffset) = 0;
            return SUCCESS;
         }
      }
    else
        return ATT_ERR_ATTR_NOT_LONG;
#endif
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
    //U8 uChangedValueID = 0xFF;                     // Tells notify function what was changed
    static U32 uCallNo=0;
    
    if ( pAttr->type.len == ATT_BT_UUID_SIZE )
    {
      // 16-bit UUID
      uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
      switch ( uuid )
      {
    

          /***********************************************************************************************
             Timezone value from App
          ***********************************************************************************************/
          case (GRP_UUID + 0):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_TIMEZONE_SIZE,"TZ:");
              uCallNo++;
          break;

          /***********************************************************************************************
             Country Code
          ***********************************************************************************************/
          case (GRP_UUID + 1):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_COUNTRY_CODE_SIZE,"CC:");
          break;

          /***********************************************************************************************
             SSID
          ***********************************************************************************************/
          case (GRP_UUID + 2):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_SSID_SIZE,"SS:");
          break;

          /***********************************************************************************************
             Passphrase
          ***********************************************************************************************/
          case (GRP_UUID + 3):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_PASSPHRASE_SIZE,"PP:");
          break;

          /***********************************************************************************************
             Client ID Size
          ***********************************************************************************************/
          case (GRP_UUID + 4):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_CLIENT_ID_SIZE,"CI:");
          break;

          /***********************************************************************************************
             Client Secret
          ***********************************************************************************************/
          case (GRP_UUID + 5):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_CLIENT_SECRET_SIZE,"CS:");
          break;

          /***********************************************************************************************
             Cloud Environment
          ***********************************************************************************************/
          case (GRP_UUID + 6):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_CLOUD_ENV_SIZE,"CE:");
          break;

          /***********************************************************************************************
             Cloud API
          ***********************************************************************************************/
          case (GRP_UUID + 7):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_CLOUD_API_SIZE,"CA:");
          break;

          /***********************************************************************************************
             App Version
          ***********************************************************************************************/
          case (GRP_UUID + 8):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_APP_VERSION_SIZE,"AV:");
          break;

          /***********************************************************************************************
            Command
          ***********************************************************************************************/
          case (GRP_UUID + 9):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, 1,"CM:");
          break;


        /***********************************************************************************************
           Message from App
        ***********************************************************************************************/
        case MSG_FROM_APP_UUID:
    
              uCallNo++;

              if ( offset == 0 )
              {
                    if ( len > MSG_FROM_APP_SIZE-1 )
                        status = ATT_ERR_INVALID_VALUE_SIZE;
                    else
                    {
                        VOID memcpy( (uint8 *)pAttr->pValue, pValue, len );
                        *(uint8 *)(pAttr->pValue + len) = 0;
                       // uChangedValueID = MSG_FROM_APP;
                        status = SUCCESS;

                        // Signal
                        extern B32 bNewAppMsg;
                        bNewAppMsg = bTRUE;

                    }
              }
              else
                status = ATT_ERR_ATTR_NOT_LONG;
    
        break;
    

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
    
#if 0
    //  Callback functions are nulled out so nothing done
    // If a characteristic value changed then callback function to notify application of change
    if ( (uChangedValueID != 0xFF ) && PhynProfile_AppCBs && PhynProfile_AppCBs->pfnValueChange )
    {
        PhynProfile_AppCBs->pfnValueChange( uChangedValueID );
    }
#endif
    
    return ( status );
}





/***********************************************************************************************
 UNUSED/TEST
 ***********************************************************************************************/


#if 0
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



              }
             else
                status = ATT_ERR_INVALID_VALUE_SIZE;
#endif

