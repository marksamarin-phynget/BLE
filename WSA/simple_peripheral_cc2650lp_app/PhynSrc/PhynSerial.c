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
I8     sScanName[SCANNAME_MAX_LEN+1];
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
    bAppMessageRead = bTRUE;                            //      Signal we are done, can accept another message from MT
    uPartialTxIdx=0;                                    //      Reset chunk index

    Start_UART_Read(PWD_SERIAL_RXBUF_SIZE);            //       Resets UART RX buffers

    UART_Naked_Tx("00, 99, Rx Timeout - Reset UART Rx");
}


/*********************************************************************
Process_Serial_Command
    Process the incoming command in sCommandLn
*********************************************************************/
void Process_Serial_Command()
{
    U32 uIdx, uCmd;
    B32 bERROR = bFALSE;
    char sText[ALT_TEXT_MAX_SIZE];
    BLE_MESSAGE_CODES_ENUM eResponse;
    U32 uKey;

    extern U32 uAppCRC, uBLEStackCRC, uNVDCRC, uCodeCRC;
    extern U32 uAppCRC1;

    extern I8 sBLE_FW_Version[BLE_VER_STR_LEN + 1];
    extern U8 sTZN[FIELD_TIMEZONE_SIZE+1];
    extern U8 cCCD[FIELD_COUNTRY_CODE_SIZE+1];
    extern U8 cSSID[FIELD_SSID_SIZE+1];
    extern U8 cPPH[FIELD_PASSPHRASE_SIZE+1];
    extern U8 cCLID[FIELD_CLIENT_ID_SIZE+1];
    extern U8 cCLS[FIELD_CLIENT_SECRET_SIZE+1];
    extern U8 cCLE[FIELD_CLOUD_ENV_SIZE+1];
    extern U8 cCLA[FIELD_CLOUD_API_SIZE+1];
    extern U8 cAPV[FIELD_APP_VERSION_SIZE+1];
    extern U8 cCMD;
    extern U8 cSTA;
    extern U8 cFWV[FIELD_FIRMWARE_VERSION_SIZE+1];
    extern U8 cSSN[FIELD_SERIAL_NUMBER_SIZE+1];
    extern U8 sSR_Data[FIELD_SR_DATA_SIZE+1];
    extern U8 sSR_Count[FIELD_SR_COUNT_SIZE+1];

    extern gaprole_States_t eGapState;
    extern U32 uLowestWDValue;
    extern U32 uGapEventCounter, uAppEventCounter, uUartEventCounter;

    extern U32 uStackSizeBytes;
    extern U32 uStackUsedBytes, uGapTaskStackUsedBytes, uAppTaskStackUsedBytes, uUartTaskStackUsedBytes;
    extern F32 fStackUsedPct, fGapStackUsedPct, fAppStackUsedPct, fUartStackUsedPct;

    I32 i;
    volatile F32 f=1.01;
    U32 uEvent;

    void Phyn_Set_Scan_Response_Name(I8 * sNewName);

    uIdx = ((sCommandLn[0]-'\x30') * 10) + sCommandLn[1]-'\x30';
    uCmd = ((sCommandLn[3]-'\x30') * 10) + sCommandLn[4]-'\x30';

    if (sCommandLn[2] != ',' || sCommandLn[5] != ',' || uIdx > 99 || uCmd > 99)
        bERROR = bTRUE;
    else
    {

        eResponse = (WIFI_MESSAGE_CODES_ENUM)uCmd;

        switch ((WIFI_MESSAGE_CODES_ENUM)uCmd)
        {


            /***********************************************************************************************
             DEBUG Function - Test Assertions
            ***********************************************************************************************/
            #ifdef DEBUG_MODE

                case WIFI_MESSAGE_CODE_FORCE_ASSERT:
                    PWD_FORCE_ASSERT(PWD_ASSERT_CAUSE_TEST_ASSERT);
                break;

            #endif

            /***********************************************************************************************
              Increment an event
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_INC_EVENT:
                uEvent = (sCommandLn[6] - 0x30)*10 + sCommandLn[7] - 0x30;
                sprintf(sText, "Increment Event %02d", uEvent);

                if (uEvent < MAX_EVENT_OCCURANCES_LOGGED)
                    BLE_IncrementEventCount(uEvent);
                else
                    sprintf(sText, "Cannot Increment Event %02d: at Max(%d)", uEvent, MAX_EVENT_OCCURANCES_LOGGED);
            break;

            /***********************************************************************************************
              Get event count
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_EVENT_COUNT:
                uEvent = (sCommandLn[6] - 0x30)*10 + sCommandLn[7] - 0x30;
                sprintf(sText, "Event %02d Count = %02d",  uEvent, BLE_GetEventCount(uEvent));
            break;


            /***********************************************************************************************
             Clear events
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_CLEAR_EVENT_COUNTS:
                BLE_ClearEvents();
                strcpy(sText, "Clear Events");
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
              FORCE WATCHDOG Command
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_FORCE_WD_TIMEOUT:
                sprintf(sText, "%02d,99,*** FORCING A Watchdog Trigger back to Bootloader ***", uIdx);
                UART_Naked_Tx(sText);

                // Wait for WD trigger - but not forever to prevent a lockup in case of fail
                for (i=0; i<10000000; i++)
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

               // eResponse = WIFI_MESSAGE_CODE_GET_STACK_INFO;
            break;

            /***********************************************************************************************
              Get WD Info
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_WD_INFO:

                #ifdef WD_ENABLED
                    sprintf(sText, "Watchdog Info: Load Value: %d  Low Mark:%d", WATCHDOG_SET_VALUE, uLowestWDValue);
                #else
                    sprintf(sText, "Watchdog DISABLED" );
                #endif

                //eResponse = WIFI_MESSAGE_CODE_GET_STACK_INFO;
            break;

            /***********************************************************************************************
              Get Counters
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_COUNTERS:
                sprintf(sText, "Counters:  Gap Events: %d  App Events: %d  Uart Events:%d", uGapEventCounter, uAppEventCounter, uUartEventCounter);
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
                    strcpy(sText, "Not Connected");
                }

            break;

            /***********************************************************************************************
              CRC Query
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_CRC:

                sprintf(sText, "Code:%08x  APP:%08x  BLE Stack:%08x  NVD:%08x", uCodeCRC, uAppCRC, uBLEStackCRC, uNVDCRC );
                //eResponse = BLE_MESSAGE_CODE_CRC_REPORT;
            break;

            /***********************************************************************************************
             FW Version Query
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_BLE_FW_VER:
                strncpy(sText, sBLE_FW_Version, BLE_VER_STR_LEN);
                sText[BLE_VER_STR_LEN]=0;
                //eResponse = BLE_MESSAGE_CODE_FW_VERSION;
            break;



            /***********************************************************************************************
             Get Scan Response Name
            ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_GET_SCAN_RESP_NAME:
                strncpy(sText, (I8 *)sScanName, SCANNAME_MAX_LEN);
                sText[SCANNAME_MAX_LEN]=0;
                //eResponse = BLE_MESSAGE_CODE_SCANNAME_REPORT;
            break;

            /***********************************************************************************************
             Set PWD Serial Number Value
            ***********************************************************************************************/
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




        /***********************************************************************************************
           Get Timezone
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_TIMEZONE:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, sTZN, FIELD_TIMEZONE_SIZE);
                sText[FIELD_TIMEZONE_SIZE]=0;
            }

        break;


        /***********************************************************************************************
           Get Country Code
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_COUNTRY_CODE:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cCCD, FIELD_COUNTRY_CODE_SIZE);
                sText[FIELD_COUNTRY_CODE_SIZE]=0;
            }

        break;

        /***********************************************************************************************
           Get SSID
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_SSID:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cSSID, FIELD_SSID_SIZE);
                sText[FIELD_SSID_SIZE]=0;
            }

        break;

        /***********************************************************************************************
           Get Passphrase
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_PASSPHRASE:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cPPH, FIELD_PASSPHRASE_SIZE);
                sText[FIELD_PASSPHRASE_SIZE]=0;
            }

        break;

        /***********************************************************************************************
           Get Client ID
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLIENT_ID:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cCLID, FIELD_CLIENT_ID_SIZE);
                sText[FIELD_CLIENT_ID_SIZE]=0;
            }
        break;

        /***********************************************************************************************
           Get Client Secret
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLIENT_SECRET:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cCLS, FIELD_CLIENT_SECRET_SIZE);
                sText[FIELD_CLIENT_SECRET_SIZE]=0;
            }

        break;

        /***********************************************************************************************
           Get Cloud Environment
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLOUD_ENV:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cCLE, FIELD_CLOUD_ENV_SIZE);
                sText[FIELD_CLOUD_ENV_SIZE]=0;
            }

        break;

        /***********************************************************************************************
           Set Cloud Environment
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_SET_CLOUD_ENV:
            memcpy((I8 *)cCLE, &sCommandLn[6], FIELD_CLOUD_ENV_SIZE);
            cCLE[FIELD_CLOUD_ENV_SIZE]=0;

            strncpy(sText, (I8 *)cCLE, FIELD_CLOUD_ENV_SIZE);
            sText[FIELD_CLOUD_ENV_SIZE]=0;

        break;

         /***********************************************************************************************
           Get Cloud API Key
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_CLOUD_API_KEY:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cCLA, FIELD_CLOUD_API_SIZE);
                sText[FIELD_CLOUD_API_SIZE]=0;
            }

        break;

        /***********************************************************************************************
           Get APP Version
         ***********************************************************************************************/
        case WIFI_MESSAGE_CODE_GET_APP_VERSION:

            if (!(GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState))
            {
                strcpy(sText, sNotConnected);
                eResponse = BLE_MESSAGE_CODE_NOCONN;
            }
            else
            {
                memcpy(sText, cAPV, FIELD_APP_VERSION_SIZE);
                sText[FIELD_APP_VERSION_SIZE]=0;
            }

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
            }
        break;



        /***********************************************************************************************
           Set scan result
         ***********************************************************************************************/
          case WIFI_MESSAGE_CODE_SET_SCAN_RESULT:
              strncpy((I8 *)sSR_Data, &sCommandLn[6], FIELD_SR_DATA_SIZE);
              sSR_Data[FIELD_SR_DATA_SIZE]=0;
              strncpy(sText, (I8 *)sSR_Data, FIELD_SR_DATA_SIZE);
              sText[FIELD_SR_DATA_SIZE]=0;
          break;

          /***********************************************************************************************
             Set scan result counter
           ***********************************************************************************************/
            case WIFI_MESSAGE_CODE_SET_SCAN_RESULT_COUNTER:
                strncpy((I8 *)sSR_Count, &sCommandLn[6], FIELD_SR_COUNT_SIZE);
                sSR_Count[FIELD_SR_COUNT_SIZE]=0;
                strncpy(sText, (I8 *)sSR_Count, FIELD_SR_COUNT_SIZE);
                sText[FIELD_SR_COUNT_SIZE]=0;
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

                        bAppMessageRead = bFALSE;                                 // Prevent new mags until this one is read by App

                        // TODO - Re evaluate mnessage sizes
                        //strncpy(sMsgToApp, &sCommandLn[6], MSG_TO_APP_SIZE-6);
                        //sMsgToApp[MSG_TO_APP_SIZE]=0;

                        // Limit to 250 chars
                        strncpy(sMsgToApp, &sCommandLn[6], 250);
                        sMsgToApp[250+6]=0;


                        extern U8 cMsgToAppCtr;
                        cMsgToAppCtr++;                                       // So that App knows a new message is there

                        eResponse = BLE_MESSAGE_CODE_PRESENTED_TO_APP;
                        strcpy(sText, "Data sent to BLE Client");
                    }
                    else
                    {
                        eResponse = BLE_MESSAGE_CODE_NOTREADY;
                        strcpy(sText, "BLE client Not ready for Data");
                    }
                }

                else
                {
                    eResponse = BLE_MESSAGE_CODE_NOCONN;
                    strcpy(sText, "BLE client Not Connected");
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




#endif
