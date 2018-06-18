/*********************************************************************
 UART Message Format

     '<' and '>' are not part of message
     ',' are in message

 Commands (Rx):

     <IDX>,<CMD>,<Text><CR>

 Responses (Tx):

     <IDX>,<RSP>,<Text><CR>

Unsolicited Messsages (Tx)

     <98>,<Text><CR>   - Text from App
     <99>,<Text><CR>   - Log message


All values are in decimal text (i.e. 23 = x3233)
     IDX: Message index (0-89, rolls over), to keep responses synced to commands
          IDX 90-99 reserved for unsolicited messages from BLE
     CMD: 0-99

Text field is optional in non-parameter commmands, but useful for terminal debugging


Example:

Command: Poll w/optional text for debug
     Command Rx:    "22,01,Poll<CR>"         Message #22,     CMD = 1 = Poll
     Tx Response:   "22,01,Ack<CR>"          Response to #22, RSP = 1 = ACK


Command: Text to App
     Command Rx:    "43,02,Message for App<CR>"      Message #43, CMD = 2 = Text to App
     Tx Response:   "43,02<CR>"                      Response #43, RSP = 1 = OK




********************************************************************/


#ifndef __PHYNSERIAL_H
#define __PHYNSERIAL_H

#include <xdc/std.h>
#include "bcomdef.h"
#include "PhynDefs.h"
#include "peripheral.h"


#define SERIAL_MSG_OVERHEAD_BYTES (7)    /* nn,cc,<MSG><NULL> */

#define SOLICITED_MSG_BUF_SIZE   (256 + SERIAL_MSG_OVERHEAD_BYTES)

#define MAX_UNSOLICITED_MSG_SIZE     (256)    /* 5 chars for "99,"  + <CR> + terminator */
#define UNSOLICITED_MSG_BUF_SIZE     (MAX_UNSOLICITED_MSG_SIZE + SERIAL_MSG_OVERHEAD_BYTES)

#define SCANNAME_MAX_LEN     (20)


#define UART_Log(string)   UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG, string)


// define the APP read fielkds that are more than 27 chars
#define PARTIAL_FIELD_APP_COMMAND 0
#define PARTIAL_FIELD_SCAN_RESULT 1
#define NUM_PARTIAL_FIELDS        2

#define SIZEOF_APP_UPDATE_MSG (100)

typedef enum
{
    // System Control/Info
    WIFI_MESSAGE_CODE_CONNECTION_STATUS     = 10,   // Request Connection Status
    WIFI_MESSAGE_CODE_RESYNCHRONIZE         = 11,   // Reset Message counters and clear buffers
    WIFI_MESSAGE_CODE_FORCE_ASSERT          = 13,   // Force an APP assert
    WIFI_MESSAGE_CODE_RESET_BLE             = 14,   // Reset the BLE board
    WIFI_MESSAGE_CODE_FORCE_WD_TIMEOUT      = 16,   // Force (test) a watchdog reset
    WIFI_MESSAGE_CODE_GET_STACK_INFO        = 17,
    WIFI_MESSAGE_CODE_GET_WD_INFO           = 18,
    WIFI_MESSAGE_CODE_GET_COUNTERS          = 19,
    WIFI_MESSAGE_CODE_SET_SCAN_RESP_NAME    = 20,
    WIFI_MESSAGE_CODE_GET_SCAN_RESP_NAME    = 21,

    WIFI_MESSAGE_CODE_INC_EVENT             = 22,
    WIFI_MESSAGE_CODE_GET_EVENT_COUNT       = 23,
    WIFI_MESSAGE_CODE_CLEAR_EVENT_COUNTS    = 24,

    // Parameter Info
    WIFI_MESSAGE_CODE_GET_TIMEZONE             = 30,   // Get Timezone Attribute
    WIFI_MESSAGE_CODE_GET_COUNTRY_CODE         = 31,   // Get Country Code Attribute
    WIFI_MESSAGE_CODE_GET_SSID                 = 32,   // Get  Attribute
    WIFI_MESSAGE_CODE_GET_PASSPHRASE           = 33,   // Get  Attribute
    WIFI_MESSAGE_CODE_GET_CLIENT_ID            = 34,   // Get  Attribute
    WIFI_MESSAGE_CODE_GET_CLIENT_SECRET        = 35,   // Get  Attribute
    WIFI_MESSAGE_CODE_GET_CLOUD_ENV            = 36,   // Get  Attribute
    WIFI_MESSAGE_CODE_SET_CLOUD_ENV            = 37,   // Set  Attribute
    WIFI_MESSAGE_CODE_GET_CLOUD_API_KEY        = 38,   // Get  Attribute
    WIFI_MESSAGE_CODE_GET_APP_VERSION          = 39,   // Get  Attribute
    WIFI_MESSAGE_CODE_GET_COMMAND              = 40,   // Get  Attribute

    WIFI_MESSAGE_CODE_SET_STATUS               = 41,   // Set  Attribute
    WIFI_MESSAGE_CODE_SET_FIRMWARE_VERSION     = 42,   // Set  Attribute
    WIFI_MESSAGE_CODE_SET_SERIAL_NUMBER        = 43,   // Set  Attribute

    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_COUNTER  = 44,   // Set  Attribute
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT          = 45,   // Set  Attribute


    // Informational
    WIFI_MESSAGE_CODE_GET_CRC               = 50,   // Get CRCs (APP, BLE Stack, NVD)
    WIFI_MESSAGE_CODE_GET_BLE_FW_VER        = 51,   // Get BLE FW Version


#if 0
    WIFI_MESSAGE_CODE_GET_SERNUM            = 52,   // Get Serial number string posted an BLE Characteristic
    WIFI_MESSAGE_CODE_SET_SERNUM            = 53,   // Set Serial number string posted an BLE Characteristic
    WIFI_MESSAGE_CODE_GET_MODEL             = 54,   // Get Model number string posted an BLE Characteristic
    WIFI_MESSAGE_CODE_SET_MODEL             = 55,   // Set Model number string posted an BLE Characteristic

    WIFI_MESSAGE_CODE_SET_MSG_TERMINATOR    = 56,   // Set Model number string posted an BLE Characteristic

    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_1  = 71, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_2  = 72,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_3  = 73, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_4  = 74,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_5  = 75, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_6  = 76,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_7  = 77, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_8  = 78,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_9  = 79, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_10 = 70,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_11 = 81, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_12 = 82,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_13 = 83, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_14 = 84,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_15 = 85, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_16 = 86,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_17 = 87, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_18 = 88,
    WIFI_MESSAGE_CODE_SET_SCAN_RESULT_19 = 89, WIFI_MESSAGE_CODE_SET_SCAN_RESULT_20 = 90,

#endif

    // Remote (client)
    WIFI_MESSAGE_CODE_APP_MSG               = 60,  // Send Data to App
    WIFI_MESSAGE_CODE_CLEAR_APP_MSG         = 61,  // Force-clear the BLE client read lockout

   WIFI_MESSAGE_CODE_NUM_OF

} WIFI_MESSAGE_CODES_ENUM;



typedef enum
{

   // Standard Responses 00-09
   BLE_MESSAGE_CODE_NOCONN                  = 81,  // Not Connected to App
   BLE_MESSAGE_CODE_NOTREADY                = 82,  // Connected to App but  not ready to receive msgs
   BLE_MESSAGE_CODE_READY                   = 83,  // Connected and ready to receive msgs

#if 0
   //BLE_MESSAGE_CODE_RESYNCHED               = 15,
   BLE_MESSAGE_CODE_RESETTING               = 14,  // BLE board is resetting back to Bootloader

   BLE_MESSAGE_CODE_CRC_REPORT              = 20,  // Returning CRCs
   BLE_MESSAGE_CODE_FW_VERSION              = 21,  // Returning FW version data Characteristic
   BLE_MESSAGE_CODE_SERNUM_REPORT           = 22,  // Returning  current serial number
   BLE_MESSAGE_CODE_SERNUM_SET              = 23,  // Serial number Characteristic set
   BLE_MESSAGE_CODE_PWD_MODEL_REPORT        = 24,  // Returning PWD Model Characteristic
   BLE_MESSAGE_CODE_PWD_MODEL_SET           = 25,  // PWD Model Characteristic Set
   BLE_MESSAGE_CODE_STATS                   = 26,
   BLE_MESSAGE_CODE_SCANNAME_REPORT         = 27,
   BLE_MESSAGE_CODE_SCANNAME_SET            = 28,



#endif
   BLE_MESSAGE_CODE_PRESENTED_TO_APP        = 70,  // Message posted as BLE characteristic

   // Exceptions 80-89
   BLE_MESSAGE_CODE_UNRECOGNIZED_CMD        = 88,
   BLE_MESSAGE_CODE_COMMAND_ERROR           = 89,

   // Unsolicited Tx Msgs (90-99)
   BLE_MESSAGE_CODE_APP                     = 90,     // Message from App
   BLE_MESSAGE_CODE_APP_FIELD               = 91,     // App field updated
   BLE_MESSAGE_CODE_LOG                     = 99,     // Log Msgs

} BLE_MESSAGE_CODES_ENUM;




#define ALT_TEXT_MAX_SIZE (128)

B32 UART_Unsolicited_Tx(BLE_MESSAGE_CODES_ENUM eType, char * sMsg);
void UART_Post_State(gaprole_States_t eState);
void Process_Serial_Command(void);
void Reset_Serial_Rx(UArg arg);

void Log_With_U32(char *sMsg, U32 uTrailingVal);
void Log_With_U8(char *sMsg, U8 uByte);


#endif /* __PHYNSERIAL_H */
