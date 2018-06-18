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

#if 0
/*********************************************************************
 Setup possible Callbacks for when a characteristic value has changed
*********************************************************************/
typedef void (*PhynProfileChange_t)( uint8 paramID );


typedef struct
{
  PhynProfileChange_t        pfnValueChange;  // Called when characteristic value changes
} PhynProfile_AppCBs_t;
    
static PhynProfile_AppCBs_t *PhynProfile_AppCBs = NULL;   // No change callback 

#endif


#define GATT_PERMIT_RW  ( GATT_PERMIT_READ | GATT_PERMIT_WRITE)



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
        U8 cCCD[FIELD_COUNTRY_CODE_SIZE+1];
        U8 cCCD_Props = GATT_PROP_WRITE;

/*********************************************************************
 SSID
*********************************************************************/
const   U8 SSID_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 2), HI_UINT16(GRP_UUID + 2) };
        U8 cSSID[FIELD_SSID_SIZE+1];
        U8 cSSID_Props = GATT_PROP_WRITE;

/*********************************************************************
 Passphrase
*********************************************************************/
const   U8 PPH_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 3), HI_UINT16(GRP_UUID + 3) };
        U8 cPPH[FIELD_PASSPHRASE_SIZE+1];
        U8 cPPH_Props = GATT_PROP_WRITE;

/*********************************************************************
 Client ID
*********************************************************************/
const   U8 CLID_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 4), HI_UINT16(GRP_UUID + 4) };
        U8 cCLID[FIELD_CLIENT_ID_SIZE+1] = "Not Set";
        U8 cCLID_Props = GATT_PROP_WRITE;

/*********************************************************************
 Client Secret
*********************************************************************/
const   U8 CLS_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 5), HI_UINT16(GRP_UUID + 5) };
        U8 cCLS[FIELD_CLIENT_SECRET_SIZE+1] = "Not Set";
        U8 cCLS_Props = GATT_PROP_WRITE;

/*********************************************************************
 Cloud Environment ID
*********************************************************************/
const   U8 CLE_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 6), HI_UINT16(GRP_UUID + 6) };
        U8 cCLE[FIELD_CLOUD_ENV_SIZE+1] = "XXX";
        U8 cCLE_Props =  GATT_PROP_READ | GATT_PROP_WRITE;

/*********************************************************************
 Cloud API Key
*********************************************************************/
const   U8 CLA_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 7), HI_UINT16(GRP_UUID + 7) };
        U8 cCLA[FIELD_CLOUD_API_SIZE+1] = "Not Set";
        U8 cCLA_Props =  GATT_PROP_WRITE;

/*********************************************************************
 App Version
*********************************************************************/
const   U8 APV_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 8), HI_UINT16(GRP_UUID + 7) };
        U8 cAPV[FIELD_APP_VERSION_SIZE+1] = "XXXXXX";
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
 Firmware Version
*********************************************************************/
const   U8 FWV_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 11), HI_UINT16(GRP_UUID + 11) };
        U8 cFWV[FIELD_FIRMWARE_VERSION_SIZE+1];
        U8 cFWV_Props =  GATT_PROP_READ;

/*********************************************************************
 Serial Number
*********************************************************************/
const   U8 SSN_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(GRP_UUID + 12), HI_UINT16(GRP_UUID + 12) };
        U8 cSSN[FIELD_SERIAL_NUMBER_SIZE+1];
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
        U8 sSR_Count[FIELD_SR_COUNT_SIZE + 1] = "00";
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
      { { ATT_BT_UUID_SIZE, CCD_UUID },         GATT_PERMIT_WRITE, 0, cCCD },          // Value

      /*********************************************************************
       SSID
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSSID_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SSID_UUID },        GATT_PERMIT_WRITE, 0, cSSID },          // Value

      /*********************************************************************
       Passphrase
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cPPH_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, PPH_UUID },         GATT_PERMIT_WRITE, 0, cPPH },          // Value

      /*********************************************************************
       Client ID
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLID_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLID_UUID },        GATT_PERMIT_WRITE, 0, cCLID },          // Value

      /*********************************************************************
       Client Secret
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLS_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLS_UUID },         GATT_PERMIT_WRITE, 0, cCLS },          // Value

      /*********************************************************************
       Cloud Environment ID
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLE_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLE_UUID },         GATT_PERMIT_WRITE | GATT_PERMIT_READ, 0, cCLE },          // Value

      /*********************************************************************
       Cloud API Key
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cCLA_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, CLA_UUID },         GATT_PERMIT_WRITE, 0, cCLA },          // Value

      /*********************************************************************
       App Version
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cAPV_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, APV_UUID },         GATT_PERMIT_WRITE, 0, cAPV },          // Value

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
      { { ATT_BT_UUID_SIZE, FWV_UUID },         GATT_PERMIT_READ, 0, cFWV },          // Value

      /*********************************************************************
       Serial Number
      *********************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSSN_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SSN_UUID },         GATT_PERMIT_READ, 0, cSSN },          // Value

      /*************************************************************************************************
        Scan Result messages
      ************************************************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSRD_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SRD_UUID },         GATT_PERMIT_READ, 0, (U8 *)&sSR_Data }, // Value

      /*************************************************************************************************
        Scan Result Counter
      ************************************************************************************************/
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSRC_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SRC_UUID },         GATT_PERMIT_READ, 0, (U8 *)&sSR_Count }, // Value

};

/***********************************************************************************************
 InitAppFields
***********************************************************************************************/
void InitAppFields()
{

    const I8 sInitField[4] = "xxx";

    strcpy(sTZN, sInitField);
    strcpy(cCCD, "xx");
    strcpy(cSSID, sInitField);
    strcpy(cPPH, sInitField);
    strcpy(cCLID, sInitField);
    strcpy(cCLS, sInitField);
    strcpy(cCLE, sInitField);
    strcpy(cCLA, sInitField);
    strcpy(cAPV, sInitField);
    cCMD = cSTA =  ' ';
    strcpy(cFWV, sInitField);
    strcpy(cSSN, sInitField);
    strcpy(sSR_Data, sInitField);
    strcpy(sSR_Count, "00");

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
    U8 Gatt_SR_Tx(U32 uIdx, U8 *pValue, U8 *sTxField, U8 uMaxLen);
    
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
#if 1
            // Send field in chunks
            *pLen = Gatt_SR_Tx(PARTIAL_FIELD_APP_COMMAND,  pValue,  pAttr->pValue, maxLen);
            status = SUCCESS;
#else
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
         break;

         /***********************************************************************************************
           Scan Results Data: GRP_UUID + 13
         ***********************************************************************************************/
         case GRP_UUID + 13:

         // Send field in chunks
            *pLen = Gatt_SR_Tx(PARTIAL_FIELD_SCAN_RESULT,  pValue,  pAttr->pValue, maxLen);
            status = SUCCESS;
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
**********************************************************************/
U8 Gatt_SR_Tx(U32 uIdx, U8 *pValue, U8 *sTxField, U8 uMaxLen)
{
   static U8 uIdxCtr[NUM_PARTIAL_FIELDS];

   U32 uCharsLeft;

   if (uIdx >= NUM_PARTIAL_FIELDS) return 0;

   uCharsLeft = strlen((const char *)&sTxField[uIdxCtr[uIdx]]);

   // Additional logic in case the messages coming into the serial port are larger than
   // the max client receive characteristic size
   if (uCharsLeft > uMaxLen)                                     //  IF remaining msg size > ATT_MTU
   {
       memcpy(pValue, &sTxField[uIdxCtr[uIdx]], uMaxLen);                //      Data transfer for BLE Tx
       uIdxCtr[uIdx] += uMaxLen;                                 //      Update string pointer for next chunk
       return uMaxLen;
   }
   else                                                         // ELSE - Last chunk
   {
       memcpy(pValue, &sTxField[uIdxCtr[uIdx]], uCharsLeft);    //      Data transfer for BLE Tx
       uIdxCtr[uIdx]=0;                                         //      Reset chunk index
       sTxField[0]= 0;                                          //      Clear message
       return uCharsLeft;
   }
}


#define APP_FIELD_TERMINATOR ('\x00')
#define DEBUG_APP_FIELD_TERMINATOR (';')


//U8 *NewAppFieldPtr;
B32 bNewAppField = bFALSE;

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
             Cloud Environment
          ***********************************************************************************************/
          case (GRP_UUID + 7):
              status = Update_Attribute_From_App(offset, pAttr, pValue, len, FIELD_CLOUD_API_SIZE,"CE:");
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
                //  uChangedValueID = PHYNPROFILE_CHAR1;
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

      //#ifndef NO_EXTRA_CHARACTERISTICS
      #if 0
      static CONST gattAttrType_t ExProfileService = { ATT_BT_UUID_SIZE, PhynProfileUUID };

      static gattAttribute_t ExProfileAttrTbl[] =
      {
          // PHYN Profile Service
          {
             { ATT_BT_UUID_SIZE, 0xFFD0 }, /* type (=0xFFFA) */
               GATT_PERMIT_READ,                         /* permissions */
               0,                                        /* handle */
              (uint8 *)&ExProfileService            /* pValue */
          },

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

      }




      gattAttribute_t ExtraProfileAttrTbl[2];

      I8 ExtraProfileServiceVal[2][64];


#endif


      /*********************************************************************
       Setup attribute Read/Write CALLBACKS
      *********************************************************************
      CONST gattServiceCBs_t ExtraRdWrCBs =
      {
        ExtraProfile_ReadAttrCB,  // Read callback function pointer
        ExtraProfile_WriteAttrCB, // Write callback function pointer
        NULL                     // Authorization callback function pointer
      };
      */

      /*
          Build_ExtraProfileAttrTbl();

          status = GATTServApp_RegisterService( ExtraProfileAttrTbl,
                                                GATT_NUM_ATTRS( ExtraProfileAttrTbl ),
                                                GATT_MAX_ENCRYPT_KEY_SIZE,
                                                &ExtraRdWrCBs);
        */
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
               SR1_UUID - SR20_UUID
           ***********************************************************************************************
          case SR_UUID+1:  case SR_UUID+2:  case SR_UUID+3:  case SR_UUID+4:  case SR_UUID+5:
          case SR_UUID+6:  case SR_UUID+7:  case SR_UUID+8:  case SR_UUID+9:  case SR_UUID+10:
          case SR_UUID+11:  case SR_UUID+12:  case SR_UUID+13:  case SR_UUID+14:  case SR_UUID+15:
          case SR_UUID+16:  case SR_UUID+17:  case SR_UUID+18:  case SR_UUID+19:  case SR_UUID+20:

             // Send a chunk of field
             *pLen = Gatt_SR_Tx(uuid-SR_UUID-1,  pValue,  pAttr->pValue, maxLen);
             status = SUCCESS;


          break;
  */


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
                           //uChangedValueID = PWD_VALIDATION_KEY;
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
                          // uChangedValueID = PWD_COMMAND;
                           status = SUCCESS;
                           Log_With_U8( "App COMMAND:", *pValue);
                       }
                 }
                 else
                   status = ATT_ERR_ATTR_NOT_LONG;

           break;
   #endif


           /*
           const U8 SR2_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 2), HI_UINT16(SR_UUID + 2) };
           const U8 SR3_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 3), HI_UINT16(SR_UUID + 3) };
           const U8 SR4_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 4), HI_UINT16(SR_UUID + 4) };
           const U8 SR5_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 5), HI_UINT16(SR_UUID + 5) };
           const U8 SR6_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 6), HI_UINT16(SR_UUID + 6) };
           const U8 SR7_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 7), HI_UINT16(SR_UUID + 7) };
           const U8 SR8_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 8), HI_UINT16(SR_UUID + 8) };
           const U8 SR9_UUID[ATT_BT_UUID_SIZE]   = { LO_UINT16(SR_UUID + 9), HI_UINT16(SR_UUID + 9) };
           const U8 SR10_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 10), HI_UINT16(SR_UUID + 10) };
           const U8 SR11_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 11), HI_UINT16(SR_UUID + 1) };
           const U8 SR12_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 12), HI_UINT16(SR_UUID + 2) };
           const U8 SR13_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 13), HI_UINT16(SR_UUID + 3) };
           const U8 SR14_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 14), HI_UINT16(SR_UUID + 4) };
           const U8 SR15_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 15), HI_UINT16(SR_UUID + 5) };
           const U8 SR16_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 16), HI_UINT16(SR_UUID + 6) };
           const U8 SR17_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 17), HI_UINT16(SR_UUID + 7) };
           const U8 SR18_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 18), HI_UINT16(SR_UUID + 8) };
           const U8 SR19_UUID[ATT_BT_UUID_SIZE]  = { LO_UINT16(SR_UUID + 19), HI_UINT16(SR_UUID + 9) };
           const U8 SR20_UUID[ATT_BT_UUID_SIZE] = { LO_UINT16(SR_UUID + 20), HI_UINT16(SR_UUID + 10) };

           //U8 cSR_Props = GATT_PROP_READ;
           */

           /*********************************************************************
            Message -  EXTRA1                       (0x89)
                       EXTRA_UUID1                 (0xFFD0)
           *********************************************************************
           #define EXTRA_UUID1 (0xFFD0)


             U8 sExtraMsg1Desc[] = "Extra #1";
             U8 sExtraMsg1[EXTRA1_SIZE+1] = {"12345678901234567890123456789012345678901234567890"};

           CONST U8 Extra1_UUID[ATT_BT_UUID_SIZE] =
           {
             LO_UINT16(EXTRA_UUID1), HI_UINT16(EXTRA_UUID1)
           };

           U8 cExtra1_Props = GATT_PROP_READ;
           */

           //const U8 sSR1_Desc[] = "Test Desc";


#if 0
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR2_UUID },         GATT_PERMIT_READ, 0, sSR_Data[1] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR3_UUID },         GATT_PERMIT_READ, 0, sSR_Data[2] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR4_UUID },         GATT_PERMIT_READ, 0, sSR_Data[3] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR5_UUID },         GATT_PERMIT_READ, 0, sSR_Data[4] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif


      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR6_UUID },         GATT_PERMIT_READ, 0, sSR_Data[5] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif


      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR7_UUID },         GATT_PERMIT_READ, 0, sSR_Data[6] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR8_UUID },         GATT_PERMIT_READ, 0, sSR_Data[7] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR9_UUID },         GATT_PERMIT_READ, 0, sSR_Data[8] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR10_UUID },        GATT_PERMIT_READ, 0, sSR_Data[9] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR11_UUID },        GATT_PERMIT_READ, 0, sSR_Data[10] }, // Value
        #ifdef USE_SR_DESC
            { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR12_UUID },        GATT_PERMIT_READ, 0, sSR_Data[11] }, // Value
        #ifdef USE_SR_DESC
          { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description
        #endif

/*  Cutoff #1 */
      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR13_UUID },        GATT_PERMIT_READ, 0, sSR_Data[12] }, // Value
      { { ATT_BT_UUID_SIZE, charUserDescUUID }, GATT_PERMIT_READ, 0, sSR1_Desc },    // User Description


      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR14_UUID },        GATT_PERMIT_READ, 0, sSR_Data[13] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR15_UUID },        GATT_PERMIT_READ, 0, sSR_Data[14] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR16_UUID },        GATT_PERMIT_READ, 0, sSR_Data[15] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR17_UUID },        GATT_PERMIT_READ, 0, sSR_Data[16] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR18_UUID },        GATT_PERMIT_READ, 0, sSR_Data[17] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR19_UUID },        GATT_PERMIT_READ, 0, sSR_Data[18] }, // Value

      { { ATT_BT_UUID_SIZE, characterUUID },    GATT_PERMIT_READ, 0, &cSR_Props },   // Declaration
      { { ATT_BT_UUID_SIZE, SR20_UUID },        GATT_PERMIT_READ, 0, sSR_Data[19] }, // Value
#endif

      #if 0
      /*************************************************************************************************
        Test messages
      *************************************************************************************************/
      //  Declaration
      {
          { ATT_BT_UUID_SIZE, characterUUID },
            GATT_PERMIT_READ, 0,   &cExtra1_Props
      },

      // Value
      {
           { ATT_BT_UUID_SIZE, Extra1_UUID },
             GATT_PERMIT_READ, 0,  sExtraMsg1
       },

       // User Description
       {
           { ATT_BT_UUID_SIZE, charUserDescUUID },
             GATT_PERMIT_READ, 0, sExtraMsg1Desc
       },
#endif
