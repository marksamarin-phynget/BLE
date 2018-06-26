#include <string.h>
#include <stdio.h>

#include "bcomdef.h"
#include "osal.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "peripheral.h"

#include "PWD_Board.h"
#include "PWD_Util.h"

#include <ti/sysbios/knl/Task.h>

#include "PhynSerial.h"
#include "PhynUart.h"
#include "PhynGatt.h"
#include "PWD_Asserts.h"
#include "simple_peripheral.h"

#include "devinfoservice.h"

#include <ti\drivers\uart\UARTCC26XX.h>
#include "util.h"

extern I8 sCommandLn[];

B32 bCommandProcessed  = bTRUE;
extern B32 bUnsolicitedMsgPend;

B32    bAppMessageRead = bTRUE;
B32    bScanResultRead = bTRUE;

I8     sScanName[DEVICENAME_PRESERVE_BYTES+1];
U8 bIDSet = FALSE;

void Send_Serial_Response_Msg(U32 uIdx, BLE_MESSAGE_CODES_ENUM eResp, char *sAltText);

extern U32 uGapTaskStackUsedBytes;
extern F32 fGapStackUsedPct;

extern U32 uAppTaskStackUsedBytes;
extern F32 fAppStackUsedPct;

extern U32 uUartStackUsedBytes;
extern F32 fUartStackUsedPct;

const I8 sNotConnected[] = "Not Connected";

/*********************************************************************
Read Tree:

Start a Read sequence:
    int UART_read(UART_Handle handle, void *buffer, size_t size)
        int UARTCC26XX_read(UART_Handle handle, void *buffer, size_t size)

Pulls from UART:
    UARTCC26XX_hwiIntFxn()
        int32_t readData(UART_Handle handle, int32_t size)  // returns chars read      
             UARTCharGetNonBlocking(hwAttrs->baseAddr)    // returns -1 if empty
           

int UARTCC26XX_write(UART_Handle handle, const void *buffer, size_t size);           
                         
*********************************************************************/


void Reset_Serial_Rx(UArg arg)
{
    extern B32 bPartialMsgToAppInPrg, bAppMessageRead;
    extern U32 uPartialTxIdx;
    extern   Clock_Struct            zMsgTimeoutClock;

    Util_stopClock(&zMsgTimeoutClock);

    bPartialMsgToAppInPrg = bFALSE;                     //      Clear Partial Message Flag
    bAppMessageRead = bTRUE;                            //      Signal we are done, can accept another message from Serial
    uPartialTxIdx=0;                                    //      Reset chunk index

    Start_UART_Read(PWD_SERIAL_RXBUF_SIZE);            //       Resets UART RX buffers

    UART_Naked_Tx("00, 99, Rx Timeout - Reset UART Rx");
}

/*********************************************************************
Get_Simple_Text_Field
    Gets a simple Write UUID string for Tx over serial
*********************************************************************/
BLE_MESSAGE_CODES_ENUM Get_Simple_Text_Field(U8 *sRespText, U8 *sSrc, U32 uMaxSize)
{
    extern gaprole_States_t eGapState;


       if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
       {
           strcpy(sRespText, sNotConnected);
           return  BLE_MESSAGE_CODE_NOCONN;
       }
       else
       {
           memcpy(sRespText, sSrc, uMaxSize);
           sRespText[uMaxSize]=0;
           return BLE_MESSAGE_CODE_SUCCESS;
       }
}

/*********************************************************************
Process_Serial_Command
    Process the incoming command in sCommandLn
*********************************************************************/
void Process_Serial_Command()
{
    U32 uIdx, uCmd;
    B32 bERROR = bFALSE;
    I8 sText[ALT_TEXT_MAX_SIZE];
    I8 sVal[8];
    BLE_MESSAGE_CODES_ENUM eResponse;

    extern U32 uAppCRC, uBLEStackCRC, uNVDCRC, uCodeCRC;
    extern U32 uAppCRC1;

    extern I8 sBLE_FW_Version[BLE_VER_STR_LEN + 1];
    extern U8 sTZN[FIELD_TIMEZONE_SIZE+1];
    extern U8 sCCD[FIELD_COUNTRY_CODE_SIZE+1];
    extern U8 sSSID[FIELD_SSID_SIZE+1];
    extern U8 sPPH[FIELD_PASSPHRASE_SIZE+1];
    extern U8 sCLID[FIELD_CLIENT_ID_SIZE+1];
    extern U8 sCLS[FIELD_CLIENT_SECRET_SIZE+1];
    extern U8 sCLE[FIELD_CLOUD_ENV_SIZE+1];
    extern U8 sCLA[FIELD_CLOUD_API_SIZE+1];
    extern U8 sAPV[FIELD_APP_VERSION_SIZE+1];
    extern U8 cCMD;
    extern U8 cSTA;
    extern U8 sFWV[FIELD_WIFI_VERSION_SIZE+1];
    extern U8 sSSN[FIELD_SERIAL_NUMBER_SIZE+1];
    extern U8 sSR_Data[FIELD_SR_DATA_SIZE+1];
    extern U8 cSR_Count;

    extern gaprole_States_t eGapState;
    extern U32 uLowestWDValue;
    extern U32 uGapEventCounter, uAppEventCounter, uUartEventCounter;

    extern U32 uStackSizeBytes;
    extern U32 uStackUsedBytes, uGapTaskStackUsedBytes, uAppTaskStackUsedBytes, uUartTaskStackUsedBytes;
    extern F32 fStackUsedPct, fGapStackUsedPct, fAppStackUsedPct, fUartStackUsedPct;

    I32 i;
    volatile F32 f=1.01;
    U32 uEvent;

    uIdx = ((sCommandLn[0]-'\x30') * 10) + sCommandLn[1]-'\x30';
    uCmd = ((sCommandLn[3]-'\x30') * 10) + sCommandLn[4]-'\x30';

    if (sCommandLn[2] != ',' || sCommandLn[5] != ',' || uIdx > 99 || uCmd > 99)
        bERROR = bTRUE;
    else
    {

        //eResponse = (WIFI_MESSAGE_CODES_ENUM)uCmd;

        switch ((WIFI_MESSAGE_CODES_ENUM)uCmd)
        {


            /***********************************************************************************************
             DEBUG Function - Test Assertions
            ***********************************************************************************************/
            #ifdef DEBUG_MODE

                case WIFI_MESSAGE_CODE_FORCE_ASSERT:
                    PWD_FORCE_ASSERT(PWD_ASSERT_CAUSE_TEST_ASSERT);
                    eResponse = BLE_MESSAGE_CODE_SUCCESS;
                break;

            #endif

            /***********************************************************************************************
              Increment an event
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_INC_EVENT:
                uEvent = (sCommandLn[6] - 0x30)*10 + sCommandLn[7] - 0x30;
                sprintf(sText, "Increment Event %02d", uEvent);

                if (uEvent < MAX_COUNT_PER_EVENT)
                {
                    BLE_IncrementEventCount(uEvent);
                    eResponse = BLE_MESSAGE_CODE_SUCCESS;

                }
                else
                {
                    sprintf(sText, "Cannot Increment Event %02d: at Max(%d)", uEvent, MAX_COUNT_PER_EVENT);
                    eResponse = BLE_MESSAGE_CODE_FAIL;
                }
            break;

            /***********************************************************************************************
              Get event count
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_EVENT_COUNT:
                uEvent = (sCommandLn[6] - 0x30)*10 + sCommandLn[7] - 0x30;
                sprintf(sText, "Event %02d Count = %02d",  uEvent, BLE_GetEventCount(uEvent));
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;


            /***********************************************************************************************
             Clear events
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_CLEAR_EVENT_COUNTS:
                BLE_ClearEvents();
                strcpy(sText, "Clear Events");
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;


            /***********************************************************************************************
              Set Device Name
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_SET_SCAN_RESP_NAME:

                if (strlen(&sCommandLn[6]) <= DEVICENAME_PRESERVE_BYTES)
                {
                    if (BLE_SetDeviceName(&sCommandLn[6]))
                    {
                        sprintf(sText, "Device Name Set - Takes effect next reset", uEvent);
                        eResponse = BLE_MESSAGE_CODE_SUCCESS;
                    }
                    else
                    {
                        strcpy(sText, "Device Name Set Fail - Previously Set set?");
                        eResponse = BLE_MESSAGE_CODE_FAIL;
                    }
                }
                else
                {
                    sprintf(sText, "Device Name Must be <= %d bytes", DEVICENAME_PRESERVE_BYTES);
                    eResponse = BLE_MESSAGE_CODE_FAIL;
                }

            break;

            /***********************************************************************************************
              Get Device Name
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_SCAN_RESP_NAME:
                U8 uSize;

                GAPRole_GetParameter(GAPROLE_SCAN_RSP_DATA, sText);
                uSize = sText[0]-1;

                if (uSize > 0 && uSize <= DEVICENAME_PRESERVE_BYTES)
                { memcpy(sText, &sText[2], uSize);
                  sText[uSize] = 0;
                }
                else
                    sText[0] = 0;

                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;


            /***********************************************************************************************
              RESET Command
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_RESET_BLE:

                sprintf(sText, "%02d,99, *** Resetting the BLE Board ***", uIdx);
                UART_Naked_Tx(sText);

                for(i=0; i<10000; i++) f*=1.00001;
                HapiResetDevice();
            break;


            /***********************************************************************************************
              FORCE GP Fault
            ***********************************************************************************************
            case WIFI_MESSAGE_CODE_FORCE_GP_FAULT:

                void (*Verboten)(void) = 0xFFFFFFFFFF;

                sprintf(sText, "%02d,99, *** Forcing a GP Fault ***");
                UART_Naked_Tx(sText);

                // time for uart message to get out
                for(i=0; i<10000; i++) f*=1.00001;

                // cause a GP fault
                Verboten();
            break;

            */

            /***********************************************************************************************
              FORCE WATCHDOG Command
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_FORCE_WD_TIMEOUT:
                sprintf(sText, "%02d,99,*** FORCING A Watchdog Trigger back to Bootloader ***", uIdx);
                UART_Naked_Tx(sText);

                // Wait for WD trigger - but not forever to prevent a lockup in case of fail
                for (i=0; i<40000000; i++)
                    f = f * 1.00001;

                sprintf(sText, "%02d,99,*** Watchdog Trigger FAILURE - Manually resetting to Bootloader ***", uIdx);
                UART_Naked_Tx(sText);

                for(i=0; i<10000; i++) f*=1.00001;
                HapiResetDevice();

            break;



            /***********************************************************************************************
              GET STATS Command
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_STACK_INFO:



                BLE_StackCheck();

                sprintf(sText, "Stack Info - System:%d/%d(%6.2f%%)  Gap:%d/%d(%6.2f%%)  App:%d/%d(%6.2f%%)  UART:%d/%d(%6.2f%%)",
                                    uStackUsedBytes, uStackSizeBytes, fStackUsedPct,
                                    uGapTaskStackUsedBytes,  GAPROLE_TASK_STACK_SIZE, fGapStackUsedPct,
                                    uAppTaskStackUsedBytes,  SBP_TASK_STACK_SIZE,     fAppStackUsedPct,
                                    uUartTaskStackUsedBytes, UART_TASK_STACK_SIZE,    fUartStackUsedPct);

                eResponse = BLE_MESSAGE_CODE_SUCCESS;

            break;

            /***********************************************************************************************
              Get WD Info
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_WD_INFO:

                #ifdef WD_ENABLE
                    sprintf(sText, "Watchdog Info: Load Value: %d  Low Mark:%d", WATCHDOG_SET_VALUE, uLowestWDValue);
                    eResponse = BLE_MESSAGE_CODE_SUCCESS;
                #else
                    sprintf(sText, "Watchdog DISABLED" );
                    eResponse = BLE_MESSAGE_CODE_FAIL;
                #endif

                //eResponse = WIFI_MESSAGE_CODE_GET_STACK_INFO;
            break;

            /***********************************************************************************************
              Get Counters
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_COUNTERS:
                sprintf(sText, "Counters:  Gap Events: %d  App Events: %d  Uart Events:%d", uGapEventCounter, uAppEventCounter, uUartEventCounter);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

            /***********************************************************************************************
              Connection Status Query
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_CONNECTION_STATUS:

                if (GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState)
                {
                    if (bAppMessageRead)
                    {
                        eResponse = BLE_MESSAGE_CODE_READY;
                        strcpy(sText, "Connected - Ready for Data");
                    }
                    else
                    {
                        eResponse = BLE_MESSAGE_CODE_NOTREADY;
                        strcpy(sText, "Connected - Not Ready For Data");
                    }
                }

                else
                {
                    eResponse = BLE_MESSAGE_CODE_NOCONN;
                    strcpy(sText, sNotConnected);
                }

            break;

            /***********************************************************************************************
              CRC Query
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_CRC:
                sprintf(sText, "Code:%08x  APP:%08x  BLE Stack:%08x  NVD:%08x", uCodeCRC, uAppCRC, uBLEStackCRC, uNVDCRC );
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

            /***********************************************************************************************
             FW Version Query
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_BLE_FW_VER:
                strncpy(sText, sBLE_FW_Version, BLE_VER_STR_LEN);
                sText[BLE_VER_STR_LEN]=0;
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

            /***********************************************************************************************
             CPU Version Query
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_BLE_CPU_VER:
                extern U32 uCPURev;
                sprintf(sText, "BLE CPU Rev %d.%d", uCPURev/10, uCPURev%10);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

        /***********************************************************************************************
           Get Timezone
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_TIMEZONE:
            eResponse =  Get_Simple_Text_Field(sText, sTZN, FIELD_TIMEZONE_SIZE);
        break;


        /***********************************************************************************************
           Get Country Code
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_COUNTRY_CODE:
            eResponse =  Get_Simple_Text_Field(sText, sCCD, FIELD_COUNTRY_CODE_SIZE);
        break;

        /***********************************************************************************************
           Get SSID
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_SSID:
            eResponse =  Get_Simple_Text_Field(sText, sSSID, FIELD_SSID_SIZE);
        break;

        /***********************************************************************************************
           Get Passphrase
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_PASSPHRASE:
            eResponse =  Get_Simple_Text_Field(sText, sPPH, FIELD_PASSPHRASE_SIZE);
        break;

        /***********************************************************************************************
           Get Client ID
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLIENT_ID:
            eResponse =  Get_Simple_Text_Field(sText, sCLID, FIELD_CLIENT_ID_SIZE);
        break;

        /***********************************************************************************************
           Get Client Secret
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLIENT_SECRET:
            eResponse =  Get_Simple_Text_Field(sText, sCLS, FIELD_CLIENT_SECRET_SIZE);
        break;

        /***********************************************************************************************
           Get Cloud Environment
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLOUD_ENV:
            eResponse =  Get_Simple_Text_Field(sText, sCLE, FIELD_CLOUD_ENV_SIZE);
        break;

        /***********************************************************************************************
           Set Cloud Environment
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_SET_CLOUD_ENV:
            memcpy((I8 *)sCLE, &sCommandLn[6], FIELD_CLOUD_ENV_SIZE);
            sCLE[FIELD_CLOUD_ENV_SIZE]=0;

            strncpy(sText, (I8 *)sCLE, FIELD_CLOUD_ENV_SIZE);
            sText[FIELD_CLOUD_ENV_SIZE]=0;
            eResponse = BLE_MESSAGE_CODE_SUCCESS;

        break;

         /***********************************************************************************************
           Get Cloud API Key
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLOUD_API_KEY:
            eResponse =  Get_Simple_Text_Field(sText, sCLA, FIELD_CLOUD_API_SIZE);
        break;

        /***********************************************************************************************
           Get APP Version
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_APP_VERSION:
            eResponse =  Get_Simple_Text_Field(sText, sAPV, FIELD_APP_VERSION_SIZE);
        break;

        /***********************************************************************************************
           Get Command
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_COMMAND:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                sText[0] = cCMD;
                sText[1]=0;
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            }
        break;

        /***********************************************************************************************
           Set status byte
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_SET_STATUS:
           memcpy((I8 *)sCLE, &sCommandLn[6], FIELD_CLOUD_ENV_SIZE);

           cSTA =
           sCLE[FIELD_CLOUD_ENV_SIZE]=0;

           strncpy(sText, (I8 *)sCLE, FIELD_CLOUD_ENV_SIZE);
           sText[FIELD_CLOUD_ENV_SIZE]=0;
           eResponse = BLE_MESSAGE_CODE_SUCCESS;

        break;

        /***********************************************************************************************
           Set WiFi Firmware Version
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_SET_WIFI_VERSION:
            strncpy((I8 *)sFWV, &sCommandLn[6],FIELD_WIFI_VERSION_SIZE);
            sFWV[FIELD_WIFI_VERSION_SIZE] = 0;
            sprintf(sText,"WiFi Version Set: %s", sFWV);
            eResponse = BLE_MESSAGE_CODE_SUCCESS;
        break;

        /************************************************************************************************
           Set Serial Number
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_SET_SERIAL_NUMBER:
            strncpy((I8 *)sSSN, &sCommandLn[6],FIELD_SERIAL_NUMBER_SIZE);
            sSSN[FIELD_SERIAL_NUMBER_SIZE] = 0;
            sprintf(sText,"Serial Number Set: %s", sSSN);
            eResponse = BLE_MESSAGE_CODE_SUCCESS;
        break;

        /***********************************************************************************************
           Set scan result if last value read by client
         ***********************************************************************************************/
          case WIFI_MESSAGE_CODE_SET_SCAN_RESULT:

              if (GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState)
              {
                  if (bScanResultRead)
                  {
                      extern char sMsgToApp[];

                      if (sCommandLn[6])
                      {
                          bScanResultRead = bFALSE;    // Prevent new msgs until this one is read by App

                          // Limit to FIELD_SR_DATA_SIZE chars
                          strncpy((I8 *)sSR_Data, &sCommandLn[6], FIELD_SR_DATA_SIZE);
                          sSR_Data[FIELD_SR_DATA_SIZE]=0;

                          eResponse = BLE_MESSAGE_CODE_SUCCESS;
                          strcpy(sText, "Updated");
                      }
                      else
                      {
                          eResponse = BLE_MESSAGE_CODE_FAIL;
                          strcpy(sText, "No Data");
                      }
                  }
                  else
                  {
                      eResponse = BLE_MESSAGE_CODE_NOTREADY;
                      strcpy(sText, "Not Updated - last value not picked up yet");
                  }
              }
              else
              {
                  eResponse = BLE_MESSAGE_CODE_NOCONN;
                  strcpy(sText, sNotConnected);

              }

          break;

          /***********************************************************************************************
             Set scan result counter
           ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_COUNTER:

                if ( sCommandLn[6] >= 0x30 && sCommandLn[6] <= 0x39 &&
                     sCommandLn[7] >= 0x30 && sCommandLn[7] <= 0x39    )
                {
                    cSR_Count =  (sCommandLn[6] - 0x30) * 10 + sCommandLn[7] - 0x30;
                    sprintf(sText,"SR Count set to %02d (=x%02x)", cSR_Count, cSR_Count);
                    eResponse = BLE_MESSAGE_CODE_SUCCESS;
                }
                else
                {
                    strcpy(sText, "Invalid Ascii BCD number - must be ""00-99""");
                    eResponse = BLE_MESSAGE_CODE_FAIL;
                }
            break;

            /***********************************************************************************************
              Send Text to App
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_APP_MSG:

                if (GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState)
                {
                    if (bAppMessageRead)
                    {
                        extern char sMsgToApp[];

                        if (sCommandLn[6])
                        {
                            bAppMessageRead = bFALSE;    // lockout further writes until picked up

                            // TODO - Re evaluate mnessage sizes
                            //strncpy(sMsgToApp, &sCommandLn[6], MSG_TO_APP_SIZE-6);
                            //sMsgToApp[MSG_TO_APP_SIZE]=0;

                            // Limit to 250 chars
                            strncpy(sMsgToApp, &sCommandLn[6], 250);
                            sMsgToApp[250+6]=0;


                            extern U8 cMsgToAppCtr;
                            cMsgToAppCtr++;                                       // So that App knows a new message is there

                            eResponse = BLE_MESSAGE_CODE_SUCCESS;
                            strcpy(sText, "Updated");
                        }
                        else
                        {
                            eResponse = BLE_MESSAGE_CODE_FAIL;
                            strcpy(sText, "No Data");
                        }
                    }
                    else
                    {
                        eResponse = BLE_MESSAGE_CODE_NOTREADY;
                        strcpy(sText, "Client not ready");
                    }
                }

                else
                {
                    eResponse = BLE_MESSAGE_CODE_NOCONN;
                    strcpy(sText, sNotConnected);
                }

            break;

            /***********************************************************************************************
               Collecting info from GAP....
             ***********************************************************************************************/

            /***********************************************************************************************
               Get the board address
               Device's Address. Read Only. Size is uint8_t[B_ADDR_LEN]. This item is read from the controller.
             ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_GAPROLE_BD_ADDR:
                GAPRole_GetParameter(GAPROLE_BD_ADDR, sVal);
                sprintf(sText,"%02x:%02x:%02x:%02x:%02x:%02x", sVal[0], sVal[1], sVal[2], sVal[3], sVal[4], sVal[5]);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

            /***********************************************************************************************
                Address of connected device. Read only. Size is uint8_t[B_MAX_ADV_LEN]. Set to all zeros when not connected
             ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_GAPROLE_CONN_BD_ADDR:
                if (GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState)
                {
                    GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, sVal);
                    sprintf(sText,"%02x:%02x:%02x:%02x:%02x:%02x", sVal[0], sVal[1], sVal[2], sVal[3], sVal[4], sVal[5]);
                    eResponse = BLE_MESSAGE_CODE_SUCCESS;
                }
                else
                {
                    strcpy(sText, sNotConnected);
                    eResponse = BLE_MESSAGE_CODE_NOCONN;
                }
            break;






            /***********************************************************************************************
              Invalid Command
            ***********************************************************************************************/
            default:
                eResponse = BLE_MESSAGE_CODE_UNRECOGNIZED_CMD;
                strcpy(sText, "Command Not Recognized");
            break;


        }


    }

    // Build and send response Message
    if (bERROR)
        if (uIdx < 100)
            Send_Serial_Response_Msg(uIdx, BLE_MESSAGE_CODE_COMMAND_ERROR, "Command Format Error");
        else
            Send_Serial_Response_Msg(99, BLE_MESSAGE_CODE_COMMAND_ERROR, "Command Format Error");
    else
        Send_Serial_Response_Msg(uIdx, eResponse, sText);

    bCommandProcessed = bTRUE;

}


/*********************************************************************
Send_Serial_Response_Msg
     Build and send response Message
*********************************************************************/
void Send_Serial_Response_Msg(U32 uIdx, BLE_MESSAGE_CODES_ENUM eResp, I8 *sAltText)
{
    extern I8   sSolicitedMsg[];
    extern B32  bSolicitedMsgPend;
    U32         uLen;

    // IF another message is being sent, throw this one away
    if (!bSolicitedMsgPend)
    {
        sSolicitedMsg[SOLICITED_MSG_BUF_SIZE-1] = 0;
        sSolicitedMsg[0] = (((U8)uIdx) / 10) + '\x30';
        sSolicitedMsg[1] = (((U8)uIdx) % 10) + '\x30';
        sSolicitedMsg[2] = ',';

        sSolicitedMsg[3] = (((U8)eResp) / 10) + '\x30';
        sSolicitedMsg[4] = (((U8)eResp) % 10) + '\x30';

        uLen = strlen(sAltText);

        if (uLen > ALT_TEXT_MAX_SIZE)
            uLen = ALT_TEXT_MAX_SIZE;

        if (uLen)
        {
            sSolicitedMsg[5] = ',';
            strncpy(&sSolicitedMsg[6], sAltText, ALT_TEXT_MAX_SIZE);
            sSolicitedMsg[7+ALT_TEXT_MAX_SIZE] = 0;

            sSolicitedMsg[uLen+6] = UART_MSG_TERMINATOR;
            sSolicitedMsg[uLen+7] = 0;
        }
        else
        {
            sSolicitedMsg[5] = UART_MSG_TERMINATOR;
            sSolicitedMsg[6] = 0;
        }

        bSolicitedMsgPend = bTRUE;    // UART Task function will send when flag is set
    }
}



/***********************************************************************************************
 UART_Unsolicited_Tx
    Send unsolicited messages.
    These will App messages to the device or Log messages from the BLE.

 ***********************************************************************************************/
B32 UART_Unsolicited_Tx(BLE_MESSAGE_CODES_ENUM eType, I8 * sMsg)
{
    U32 uLen;
    extern I8 sUnsolicitedMsg[];

    if (!bUnsolicitedMsgPend)
    {
        sUnsolicitedMsg[UNSOLICITED_MSG_BUF_SIZE-1] = 0;

        // Unsolicuted messages always use message number "00"
        sUnsolicitedMsg[0] = '\x30';
        sUnsolicitedMsg[1] = '\x30';
        sUnsolicitedMsg[2] = ',';

        // Add type: 00,<CMD>,
        sUnsolicitedMsg[3] = (((U8)eType) / 10) + '\x30';
        sUnsolicitedMsg[4] = (((U8)eType) % 10) + '\x30';
        sUnsolicitedMsg[5] = ',';

        // Add Payload
        uLen = strlen(sMsg);
        strncpy(&sUnsolicitedMsg[6], sMsg, MAX_UNSOLICITED_MSG_SIZE);

        // Terminate
        sUnsolicitedMsg[uLen+6] = UART_MSG_TERMINATOR;
        sUnsolicitedMsg[uLen+7] = 0;

        bUnsolicitedMsgPend = bTRUE;  // simple mechanism to prevent stepping on
        return bTRUE;
    }

    return bFALSE;

}




/***********************************************************************************************
 UART_Post_State

 ***********************************************************************************************/
void UART_Post_State(gaprole_States_t eState)
{
    static gaprole_States_t eLastState = GAPROLE_STATES_NUM_OF;


    if (eState != eLastState)
    {
        eLastState = eState;

        switch (eState)
        {
            case GAPROLE_INIT:                  UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Init");                             break;
            case GAPROLE_STARTED:               UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Started");                          break;
            case GAPROLE_ADVERTISING:           UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Advertising");                      break;
            case GAPROLE_ADVERTISING_NONCONN:   UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Non-Connectable advertising");      break;
            case GAPROLE_WAITING:               UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Waiting - Not Connected");  break;
            case GAPROLE_WAITING_AFTER_TIMEOUT: UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Waiting after Connection Timeout"); break;
            case GAPROLE_CONNECTED:             UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Connected");                        break;
            case GAPROLE_CONNECTED_ADV:         UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: Connected and advertising");        break;
            case GAPROLE_ERROR:                 UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG,"GAP State: ERROR");                            break;
        }

    }

}


/***********************************************************************************************
 Log_With_U8
    Max Message size =

    BLOCKS - DO NOT CALL FROM UART TASK
 ***********************************************************************************************/
void Log_With_U8(char *sMsg, U8 uByte)
{
    char sLog[128];
    char sUns[11];

    strncpy(sLog, sMsg, 128-5);
    sLog[127-5] = 0;

    ByteToAscii(uByte, &sUns[2]);
    sUns[0] = '0';  sUns[1] = 'x';  sUns[4] = 0;
    strcat(sLog, sUns);

    while(bUnsolicitedMsgPend)HITWD();
    UART_Log(sLog);
}


/***********************************************************************************************
 Log_With_U32
    Max Message size =

    BLOCKS - DO NOT CALL FROM UART TASK
 ***********************************************************************************************/
void Log_With_U32(char *sMsg, U32 uTrailingVal)
{
    char sLog[128];
    char sUns[11];

    strncpy(sLog, sMsg, 128-11);
    sLog[127-11] = 0;

    UnsLongToAscii(uTrailingVal, &sUns[2]);
    sUns[0] = '0';  sUns[1] = 'x';  sUns[10] = 0;
    strcat(sLog, sUns);

    while(bUnsolicitedMsgPend)HITWD();
    UART_Log(sLog);
}



/***********************************************************************************************
 UNUSED/TEST
 ***********************************************************************************************/
#if 0



#if 0
            /***********************************************************************************************
             Get PWD Serial Number Value
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_SERNUM:
                strncpy(sText, (I8 *)devInfoSerialNumber, DEVINFO_STR_ATTR_LEN);
                sText[DEVINFO_STR_ATTR_LEN]=0;
                //eResponse = BLE_MESSAGE_CODE_SERNUM_REPORT;
            break;

            /***********************************************************************************************
             Set PWD Serial Number Value
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_SET_SERNUM:
                strncpy((I8 *)devInfoSerialNumber, &sCommandLn[6], DEVINFO_STR_ATTR_LEN);
                devInfoSerialNumber[DEVINFO_STR_ATTR_LEN]=0;
                strncpy(sText, (I8 *)devInfoSerialNumber, DEVINFO_STR_ATTR_LEN);
                sText[DEVINFO_STR_ATTR_LEN]=0;
                //eResponse = BLE_MESSAGE_CODE_SERNUM_SET;
            break;

            /***********************************************************************************************
             Get PWD Serial Model Value
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_MODEL:
                strncpy(sText, (I8 *)devInfoModelNumber, DEVINFO_STR_ATTR_LEN);
                sText[DEVINFO_STR_ATTR_LEN]=0;
                //eResponse = BLE_MESSAGE_CODE_PWD_MODEL_REPORT;
            break;

            /***********************************************************************************************
             Set PWD Serial Model Value
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_SET_MODEL:
                strncpy((I8 *)devInfoModelNumber, &sCommandLn[6], DEVINFO_STR_ATTR_LEN);
                devInfoModelNumber[DEVINFO_STR_ATTR_LEN]=0;
                strncpy(sText, (I8 *)devInfoModelNumber, DEVINFO_STR_ATTR_LEN);
                sText[DEVINFO_STR_ATTR_LEN]=0;
                //eResponse = BLE_MESSAGE_CODE_PWD_MODEL_SET;
            break;
#endif
            /***********************************************************************************************
             WIFI_MESSAGE_CODE_GET_TESTMSG_1
            ***********************************************************************************************
            extern U8 sExtraMsg1[];
            case WIFI_MESSAGE_CODE_GET_TESTMSG_1:
               strncpy(sText, (I8 *)sExtraMsg1, EXTRA1_SIZE);
               sText[EXTRA1_SIZE]=0;
               eResponse = WIFI_MESSAGE_CODE_GET_TESTMSG_1;
            break;
            */

           /***********************************************************************************************
              WIFI_MESSAGE_CODE_SET_SCAN_RESULT_X
           ***********************************************************************************************
            extern U8 sSR_Data[NUM_SR_FIELDS][SR_SIZE+1];
            I32 iSRIdx;

            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_1: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_2:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_3: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_4:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_5: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_6:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_7: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_8:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_9: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_10:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_11: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_12:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_13: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_14:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_15: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_16:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_17: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_18:
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_19: case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_20:


                iSRIdx = uCmd-WIFI_MESSAGE_CODE_SET_SCAN_RESULT_1;
                if (iSRIdx < NUM_SR_FIELDS)
                {
                    strncpy((I8 *)sSR_Data[iSRIdx], &sCommandLn[6], SR_SIZE);
                    sSR_Data[iSRIdx][SR_SIZE]=0;
                    strncpy(sText, (I8 *)sSR_Data[iSRIdx], SR_SIZE);

                    sText[SR_SIZE]=0;
                    eResponse = uIdx;
                }
                else
                    bERROR = bTRUE;
            break;
            */


            /***********************************************************************************************
             Set PWD Serial Number Value
            ***********************************************************************************************
            case WIFI_MESSAGE_CODE_SET_SCAN_RESP_NAME:
                uKey = Task_disable();
                Phyn_Set_Scan_Response_Name(&sCommandLn[6]);

                extern U8 TRUE_U8;
                extern I32 GapPri;

#ifdef WAIT_FOR_ID_CHANGE
                void GAP_Setup();
                GAP_Setup();
#endif
                //extern Task_Handle GapTaskHandle;
                //Task_setPri(GapTaskHandle, GapPri);

                GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, 1,   &TRUE_U8);
                Task_restore(uKey);

                //Phyn_Set_Scan_Response_Name("ID Test");

                strncpy(sText, (I8 *)sScanName, SCANNAME_MAX_LEN);
                sText[SCANNAME_MAX_LEN]=0;
               // eResponse = BLE_MESSAGE_CODE_SCANNAME_SET;

                bIDSet = bTRUE;
            break;
*/


#if 0

            /***********************************************************************************************
               Get the board direct adv address
               Direct Advertisement Address. Read/Write. Size is uint8_t[B_ADDR_LEN]. Default is NULL.
             ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_GAPROLE_ADV_DIRECT_ADDR:
                GAPRole_GetParameter(GAPROLE_ADV_DIRECT_ADDR, sVal);
                sprintf(sText,"%02x:%02x:%02x:%02x:%02x:%02x", sVal[0], sVal[1], sVal[2], sVal[3], sVal[4], sVal[5]);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;


            /***********************************************************************************************
               Connection Handle. Read Only. Size is uint16_t.
             ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_GAPROLE_CONNHANDLE:
                GAPRole_GetParameter(GAPROLE_CONNHANDLE, sVal);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

            /***********************************************************************************************
               GAP Peripheral Role State. Read Only. Size is uint8_t.
             ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_GAPROLE_STATE:
                GAPRole_GetParameter(GAPROLE_STATE, sText);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

            /***********************************************************************************************
               Address type of connected device. Read only. Size is uint8_t.
             ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_GAPROLE_BD_ADDR_TYPE:
                GAPRole_GetParameter(GAPROLE_BD_ADDR_TYPE, sText);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

            /***********************************************************************************************
               Reason of the last connection terminated event. Size is uint8_t.
             ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_GAPROLE_CONN_TERM_REASON:
                GAPRole_GetParameter(GAPROLE_CONN_TERM_REASON, sText);
                eResponse = BLE_MESSAGE_CODE_SUCCESS;
            break;

#endif


#endif
