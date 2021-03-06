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

#include "PWD_Board.h"

#include "PhynUart.h"
#include "PhynSerial.h"

#include <ti\drivers\uart\UARTCC26XX.h>
#include <ti/sysbios/knl/Clock.h>
#include "util.h"
#include <ti/sysbios/knl/Task.h>

U8 SerTxBuf[PWD_SERIAL_TXBUF_SIZE];
U8 SerRxBuf[PWD_SERIAL_RXBUF_SIZE];
char sCommandLn[PWD_SERIAL_RXBUF_SIZE];

char sUnsolicitedMsg[UNSOLICITED_MSG_BUF_SIZE];
char sSolicitedMsg[SOLICITED_MSG_BUF_SIZE];

UART_Params zUARTParams;
UART_Handle hUARTHandle;

B32 bUARTReadFinished  = bFALSE, bUARTWriteFinished = bTRUE, bUARTRxError = bFALSE;
B32 bKA_Time           = bFALSE;
B32 bSolicitedMsgPend  = bFALSE, bUnsolicitedMsgPend = bFALSE, bUnsolicitedMsgsOn = bTRUE;

B32 bRxTerm;      // Used by UART26XX.c but defined here so that the modified version cannot be used outside of PWD

U32 uLastUartBytesRead;
U32 uUartEventCounter;

static U32  UART_Bytes_Read(void);

static void Uart_Rx_Callback(UART_Handle handle, void *ptr, size_t size);
static void Uart_Tx_Callback(UART_Handle handle, void *ptr, size_t size);

//static B32 UART_Command_Is_Processed();
//static void UART_Move_Rx(U32 uBytesToMove);

static void UART_taskFxn(UArg a0, UArg a1);
static void UART_CreateTask(void);




ICall_Semaphore UARTsem;
ICall_EntityID UARTEntityID;

Task_Struct zUartTaskStruct;
U8          UartTaskStack[UART_TASK_STACK_SIZE];

gaprole_States_t eGapState;

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


B32 UART_Is_Unsolicited_Msg_Is_Pending()
{
    return bUnsolicitedMsgPend;
}


/*********************************************************************
 UART_Naked_Tx
     Transmits chars out of the serial port, bypassing the task functions
     Use for transmitting info only before tasks are started.

     UARTCharPut blocks so this will block if UART is not setup,
     causing a WD fail.
********************************************************************/
void UART_Naked_Tx(I8 *sData)
{
    U32 uSize=0;

    UARTEnable(UART0_BASE);

    while (*sData != 0 && uSize++ < 255)
        UARTCharPut(UART0_BASE, *sData++);

    UARTCharPut(UART0_BASE, UART_MSG_TERMINATOR);

}

/*********************************************************************
 UART_Send_Msg
********************************************************************/
static B32 UART_Send_Msg(char *sSource, U32 uSize)
{

    if (UART_Write_Is_Finished())
    {
        UART_Start_Write(sSource, uSize);
        return bTRUE;
    }
    else
        return bFALSE;

}

/*********************************************************************
 UART_Start_Write
     Loads ther given message into the UART Tx buffer and sets
     the ready flag.  Higher level functions must verify that
     the transmitter is available.
*********************************************************************/
void UART_Start_Write(char *sSource, U32 uSize)
{
    if (uSize > PWD_SERIAL_RXBUF_SIZE)
       uSize = PWD_SERIAL_RXBUF_SIZE;

   bUARTWriteFinished = bFALSE;
   UART_write(hUARTHandle, sSource, uSize);
   //Util_restartClock(&zKAClock, 5000);      // reset KA clock on any write
}

/*********************************************************************
 Is_UART_Write_Finished
    
*********************************************************************/
B32 UART_Write_Is_Finished(void)
{
    return bUARTWriteFinished;
}    
 

/*********************************************************************
 UART_Bytes_Read

*********************************************************************/
static U32 UART_Bytes_Read(void)
{
    UARTCC26XX_Object                *object;


    if (hUARTHandle)
    {
        object = hUARTHandle->object;
        return object->readCount;
    }
    else
        return 0;
}


U32 uCurrentUARTReadSize;
U8  * cpCurrentUARTRxBuffer;

#if 0
/*********************************************************************
 ResetUartReadBuffer
     Reset the incoming UART message stream
     Call when getting a zero in the input stream
*********************************************************************/
void ResetUartReadBuffer()
{
            U32                     uKey;
            UARTCC26XX_Object       *UartObjectPtr;
   extern   Clock_Struct            zMsgTimeoutClock;

   Util_stopClock(&zMsgTimeoutClock);

   UartObjectPtr  = hUARTHandle->object;

   if (UartObjectPtr)
   {
       uKey = Hwi_disable();
       UartObjectPtr->readSize = uCurrentUARTReadSize;
       UartObjectPtr->readBuf  = SerRxBuf;
       UartObjectPtr->readCount = 0;
       Hwi_restore(uKey);
   }


}
#endif
/*********************************************************************
 Start_UART_Read

*********************************************************************/
void Start_UART_Read(U32 uSize)
{
    U32                              key;
    UARTCC26XX_Object                *UartObjectPtr;
    UARTCC26XX_HWAttrsV1 const       *hwAttrs;

    extern Clock_Struct              zMsgTimeoutClock;

    uCurrentUARTReadSize  = uSize;        /* Set readSize */
    cpCurrentUARTRxBuffer = SerRxBuf;

    if (uSize > PWD_SERIAL_RXBUF_SIZE)
       uSize = PWD_SERIAL_RXBUF_SIZE;

    bUARTReadFinished = bFALSE;

    /* Get the pointer to the object */
    UartObjectPtr  = hUARTHandle->object;
    hwAttrs = hUARTHandle->hwAttrs;

    key = Hwi_disable();                    /* Disable preemption while checking if the uart is in use. */
    UartObjectPtr->readSize = uSize;        /* Set readSize */
    UartObjectPtr->status = UART_OK;        /* Update the status of the UART module */
    UartObjectPtr->readBuf = SerRxBuf;
    UartObjectPtr->readCount = 0;
    Hwi_restore(key);

    /* Enable RX */
    HWREG(UART0_BASE + UART_O_CTL) |= UART_CTL_RXE;

    /* Enable RX interrupts */
    UARTIntEnable(hwAttrs->baseAddr, UART_INT_RX | UART_INT_RT |
                  UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE);

}

/*********************************************************************
 UART_Read_Is_Finished
    
*********************************************************************/
B32 UART_Read_Is_Finished(void)
{
    return bUARTReadFinished;
}    
 
    
/*********************************************************************
Init_PWD_UART
    
*********************************************************************/
void Init_PWD_UART()
{   
    ICall_CSState key;
 
    // Configure UART parameters.
    UART_init();      // Calls UARTCC26XX_init(), which just sets opened=false
    
    // Route Tx/Rx to correct GPIO lines
    IOCPortConfigureSet(IOID_2, PINCC26XX_MUX_MCU_UART0_TX, IOC_STD_OUTPUT);
    IOCPortConfigureSet(IOID_1, PINCC26XX_MUX_MCU_UART0_RX, IOC_STD_INPUT);


    UART_Params_init(&zUARTParams);
    zUARTParams.baudRate        = PWD_SERIAL_BAUD_RATE;
    zUARTParams.readDataMode    = UART_DATA_BINARY;
    zUARTParams.writeDataMode   = UART_DATA_BINARY;
    zUARTParams.dataLength      = UART_LEN_8;
    zUARTParams.stopBits        = UART_STOP_ONE;
    zUARTParams.parityType      = UART_PAR_NONE;
    zUARTParams.readMode        = UART_MODE_CALLBACK;
    zUARTParams.writeMode       = UART_MODE_CALLBACK;
    zUARTParams.readEcho        = UART_ECHO_OFF;
    zUARTParams.readReturnMode  = UART_RETURN_NEWLINE;   // NOT SUPPORTED :-(

    zUARTParams.readCallback    = Uart_Rx_Callback;
    zUARTParams.writeCallback   = Uart_Tx_Callback;

    key = ICall_enterCriticalSection();
 
    // Open / power on the UART.
    hUARTHandle = UART_open(Board_UART, &zUARTParams);

    // Setup partial message processing flag
    if(hUARTHandle != NULL)
    {
        UART_control(hUARTHandle, UARTCC26XX_CMD_RX_FIFO_FLUSH,       NULL);
        UART_control(hUARTHandle, UARTCC26XX_RETURN_PARTIAL_DISABLE,  NULL);
        UART_CreateTask();
    }
   // else
   //     Assert("UART Config Structure not present");
    

    UARTsem = Semaphore_create(1, NULL, NULL);

    ICall_leaveCriticalSection(key);

    Start_UART_Read(PWD_SERIAL_RXBUF_SIZE);    // Uart ready to rx
}


/*********************************************************************
  UART_CreateTask
*********************************************************************/
static void UART_CreateTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack      = UartTaskStack;
  taskParams.stackSize  = UART_TASK_STACK_SIZE;
  taskParams.priority   = UART_TASK_PRIORITY;

  Task_construct(&zUartTaskStruct, UART_taskFxn, &taskParams, NULL);

  ICall_registerApp(&UARTEntityID, &UARTsem);
}




/*********************************************************************
Uart_Service
*********************************************************************/
void Uart_KA(UArg arg)
{
    bKA_Time = bTRUE;
    //Util_restartClock(&zUartKAClock, 5000);
}

/*********************************************************************
UART_taskfxn
*********************************************************************/
static void UART_taskFxn(UArg a0, UArg a1)
{
    U32 uLen;
    extern U32 uUartTaskCounter;

    extern Clock_Struct  zMsgTimeoutClock;

    // Application main loop
    for (;;)
    {
        uUartTaskCounter++;
        Semaphore_pend(UARTsem, 100);    // Service every 100ms (never post the semaphore)

        if (bSolicitedMsgPend)
        {
            uUartEventCounter++;
            uLen = strlen(sSolicitedMsg);

            sSolicitedMsg[uLen]=0;         // add NULL to end of tx

            if (UART_Send_Msg(sSolicitedMsg, uLen+1))
                bSolicitedMsgPend = bFALSE;

        }
        else
            if (bUnsolicitedMsgPend && bUnsolicitedMsgsOn)
            {
               uLen = strlen(sUnsolicitedMsg);

               sUnsolicitedMsg[uLen]=0;         // add NULL to end of tx
               uUartEventCounter++;

               if (UART_Send_Msg(sUnsolicitedMsg, uLen+1))
                   bUnsolicitedMsgPend = bFALSE;
            }


        // Read is finished when Terminator> is received or buffer full
        if (UART_Read_Is_Finished())
        {
            uUartEventCounter++;
            uLastUartBytesRead = UART_Bytes_Read();                // Determine how many bytes came in
            Start_UART_Read(PWD_SERIAL_RXBUF_SIZE);               // Setup to read next Rx stream
            Process_Serial_Command();
            Util_stopClock(&zMsgTimeoutClock);
        }
    }
}

/***********************************************************************************************
 PWD_New_Gap_State


 ***********************************************************************************************/
void PWD_New_Gap_State(gaprole_States_t eState)
{
    eGapState = eState;
    UART_Post_State(eState);

}

/***********************************************************************************************
 UART_Move_Rx


 ***********************************************************************************************
static void UART_Move_Rx(U32 uBytesToMove)
{
    memcpy(sCommandLn, SerRxBuf, uBytesToMove);
    bCommandProcessed = bFALSE;

}
*/
/***********************************************************************************************
 UARTRxErrorFunc


 ***********************************************************************************************/
void UARTRxErrorFunc(U8 cEvent, U8 *MsgPtr, U32 uMsgLen)
{
    bUARTRxError = bTRUE; 
}     

/***********************************************************************************************
 Uart_Rx_Callback
     Rx Callback - called when buffer full or Rx char = UART_MSG_TERMINATOR
        handle - handle to the UART port
        ptr    - pointer to buffer to read data into
        size   - size of the data

 ***********************************************************************************************/
static void Uart_Rx_Callback(UART_Handle handle, void *ptr, size_t size)
{
    extern B32 bCommandProcessed;

    memcpy(sCommandLn, ptr, size);
    sCommandLn[size] = 0;

    bUARTReadFinished = bTRUE;
    bCommandProcessed = bFALSE;
    bRxTerm = bFALSE;

}


/***********************************************************************************************
 Uart_Tx_Callback
 
        handle - handle to the UART port
        ptr    - pointer to buffer to read data into
        size   - size of the data

 ***********************************************************************************************/
static void Uart_Tx_Callback(UART_Handle handle, void *ptr, size_t size)
{
   bUARTWriteFinished = bTRUE;
  
}

/***********************************************************************************************
 UNUSED/TEST
 ***********************************************************************************************/
#if 0
/*
{
    .baudRate           = UART_BAUD_RATE;
    .readDataMode       = UART_DATA_TEXT;
    .writeDataMode      = UART_DATA_TEXT;
    .dataLength         = UART_LEN_8;
    .parityType         = UART_PARITY_NONE;
    .stopBits           = UART_STOP_ONE;
    .readMode           = UART_MODE_CALLBACK;
    .writeMode          = UART_MODE_CALLBACK;
    .readEcho           = UART_ECHO_OFF;
    .writeSize          = 0;
    .readSize           = 0;
    .readRetPartial     = false;
    .writeCR            = false
    .readFifoThreshold  = UART_TH_FIFO_4_8;
    .writeFifoThreshold = UART_FIFO_TX1_8;
    
    
    .readTimeout    = UART_READ_TIMEOUT;
    .writeTimeout   = UART_WRITE_TIMEOUT;
    .readCallback   = UartTxCallback;
    .writeCallback  = UartRxCallback;
    
    readReturnMode
} zUARTParams;
*/


#endif
