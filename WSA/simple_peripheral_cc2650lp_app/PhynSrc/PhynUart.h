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


#ifndef __PWD_UART_H
#define __PWD_UART_H


#include "bcomdef.h"
#include "PhynDefs.h"
#include "peripheral.h"

#define PWD_SERIAL_BAUD_RATE  115200
#define PWD_SERIAL_TXBUF_SIZE 1024
#define PWD_SERIAL_RXBUF_SIZE 1024



#define UART_TASK_STACK_SIZE    1000
#define UART_TASK_PRIORITY        2

#define UART_READ_TIMEOUT   (250)  // UNITS ??
#define UART_WRITE_TIMEOUT  (100)  // UNITS ??


// when UART_MSG_TERMINATOR is encountered by the UART read function readData(), the end of incoming message is signaled
#ifdef  TERMINAL_INTERFACE
    #define UART_MSG_TERMINATOR ('\r')        /* user <CR> for terminal interface */
#else
    #define UART_MSG_TERMINATOR ('\x00')      /* null-terminated chunks for PWD connection*/
#endif

void Init_PWD_UART();
void UART_Service(void);


void Start_UART_Read(U32 uSize);
void UART_Start_Write(char *sSource, U32 uSize);
B32  UART_Read_Is_Finished(void);

B32  UART_Write_Is_Finished(void);

void Uart_KA(UArg arg);

void ResetUartReadBuffer(void);
B32 UART_Is_Unsolicited_Msg_Is_Pending(void);

void UART_Naked_Tx(I8 *sData);           // Direct UART write - bypasses task abstraction (blocks)

void PWD_New_Gap_State(gaprole_States_t eState);

#endif
