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


 CMD    Command             Response(s)         Meaning
---------------------------------------------------------

  01    App Status              00              Not Connected
                                01              Connected - Not Ready for Data
                                02              Connected - Ready for Data


  02    Connected ID            00              Not Connected
                                01,<ID>         ID of connected App

  03    Msg to App              00              App not ready to receive
                                01              Msg received, sending to App

  04    Reset Msgs              01              Resetting - App notified to clear


All communication between MT and IOS/Android is through text messaging.
Long text sequences can be constructed by sending multiple messages.
A Message is not considered complete until the <EOL> character is sent


Typical message transaction from MT to APP

                                            Comment
                                            --------------------------------------
    Rx: 09,01,App Status<CR>                MT waiting for a BLE connection to App
    Tx: 09,00,Not Connected<CR>             No Connection
    .
    .
    Rx: 10,01,App Status<CR>                MT waiting for a BLE connection to App
    Tx: 10,01,Connected-Not ready<CR>
    .
    .
    Rx: 11,01,App Status<CR>                MT waiting for a BLE connection to App
    Tx: 11,02,Connected-Ready<CR>

    Rx: 12,10,<1st text line><CR>           Send 1st line of message to app
    Tx: 12,01<CR>                           Message being presented as a BLE characteristic

    Rx: 13,10,<2nd Text Line><EOL><CR>      Send 2nd (last) line of message to app <EOL> denotes last line
    Tx: 13,03,APP not ready for Msg<CR>     Not accepted - App has not processed 1st line yet

    Rx: 14,10,2nd line text<EOL><CR>        Resending 2nd line
    Tx: 14,01,Presenting to App<CR>         Message being presented as a BLE characteristic

    Tx: 98,01,<App message-L1><CR>          App sends a response to message over 3 transactions
    Tx: 98,01,<App message-L2><CR>
    Tx: 98,01,<App message-L3><EOL><CR>


    Note: App Status could be used top poll until app is ready before sending next message at #13





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

#define UART_Log(string)   UART_Unsolicited_Tx(BLE_MESSAGE_CODE_LOG, string)

typedef enum
{

   // Standard Responses 00-09
   BLE_MESSAGE_CODE_NOCONN                  = 11,  // Not Connected to App
   BLE_MESSAGE_CODE_NOTREADY                = 12,  // Connected to App but  not ready to receive msgs
   BLE_MESSAGE_CODE_READY                   = 13,  // Connected and ready to receive msgs

   //BLE_MESSAGE_CODE_RESYNCHED               = 15,
   BLE_MESSAGE_CODE_RESETTING               = 14,  // BLE board is resetting back to Bootloader

   BLE_MESSAGE_CODE_CRC_REPORT              = 20,  // Returning CRCs
   BLE_MESSAGE_CODE_FW_VERSION              = 21,  // Returning FW version data Characteristic
   BLE_MESSAGE_CODE_SERNUM_REPORT           = 22,  // Returning  current serial number
   BLE_MESSAGE_CODE_SERNUM_SET              = 23,  // Serial number Characteristic set
   BLE_MESSAGE_CODE_PWD_MODEL_REPORT        = 24,  // Returning PWD Model Characteristic
   BLE_MESSAGE_CODE_PWD_MODEL_SET           = 25,  // PWD Model Characteristic Set
   BLE_MESSAGE_CODE_STATS                   = 26,


   BLE_MESSAGE_CODE_PRESENTED_TO_APP        = 30,  // Message posted as BLE characteristic


   // Exceptions 80-89
   BLE_MESSAGE_CODE_UNRECOGNIZED_CMD        = 88,
   BLE_MESSAGE_CODE_COMMAND_ERROR           = 89,

   // Unsolicited Tx Msgs (90-99)
   BLE_MESSAGE_CODE_APP                     = 90,     // Message from App
   BLE_MESSAGE_CODE_LOG                     = 99,     // Log Msgs

} BLE_MESSAGE_CODES_ENUM;



typedef enum
{
    // System
    WIFI_MESSAGE_CODE_CONNECTION_STATUS     = 41,   // Request Connection Status
   // WIFI_MESSAGE_CODE_RESYNCHRONIZE         = 44,   // Reset Message counters and clear buffers
    WIFI_MESSAGE_CODE_RESET_BLE             = 44,   // Reset the BLE board
    WIFI_MESSAGE_CODE_FORCE_WD_TIMEOUT      = 45,   // Force (test) a watchdog reset
    WIFI_MESSAGE_CODE_GET_STATS             = 46,

    // Informational
    WIFI_MESSAGE_CODE_GET_CRC               = 50,   // Get CRCs (APP, BLE Stack, NVD)
    WIFI_MESSAGE_CODE_GET_FW_VER            = 51,   // Get BLW FW Version
    WIFI_MESSAGE_CODE_GET_SERNUM            = 52,   // Get Serial number string posted an BLE Characteristic
    WIFI_MESSAGE_CODE_SET_SERNUM            = 53,   // Set Serial number string posted an BLE Characteristic
    WIFI_MESSAGE_CODE_GET_MODEL             = 54,   // Get Model number string posted an BLE Characteristic
    WIFI_MESSAGE_CODE_SET_MODEL             = 55,   // Set Model number string posted an BLE Characteristic

    WIFI_MESSAGE_CODE_SET_MSG_TERMINATOR    = 56,   // Set Model number string posted an BLE Characteristic

    // Remote (client)
    WIFI_MESSAGE_CODE_APP_MSG               = 60,  // Send Data to App
    WIFI_MESSAGE_CODE_CLEAR_APP_MSG         = 61,  // Force-clear the BLE client read lockout

    // Test Codes

    #ifdef DEBUG_MODE
        WIFI_MESSAGE_CODE_FORCE_ASSERT      = 20,  // Force an APP assert
    #endif

   WIFI_MESSAGE_CODE_NUM_OF

} WIFI_MESSAGE_CODES_ENUM;



#define ALT_TEXT_MAX_SIZE (128)

B32 UART_Unsolicited_Tx(BLE_MESSAGE_CODES_ENUM eType, char * sMsg);
void UART_Post_State(gaprole_States_t eState);
void Process_Serial_Command(void);
void Reset_Serial_Rx(UArg arg);

void Log_With_U32(char *sMsg, U32 uTrailingVal);
void Log_With_U8(char *sMsg, U8 uByte);

#endif /* __PHYNSERIAL_H */
