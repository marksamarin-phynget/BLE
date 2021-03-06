
#include <xdc/runtime/Error.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <string.h>
#include <stdio.h>

#include "icall.h"
#include "hal_assert.h"
#include "bcomdef.h"
#include "peripheral.h"
#include "simple_peripheral.h"

/* Header files required to enable instruction fetch cache */
#include <inc/hw_memmap.h>
#include <driverlib/vims.h>


#include <driverlib/chipinfo.h>

#include "PhynDefs.h"
#include "PhynUart.h"
#include "PhynSerial.h"
#include "PWD_Util.h"

#ifndef USE_DEFAULT_USER_CFG

    #include "ble_user_config.h"

// BLE user defined configuration
bleUserCfg_t user0Cfg = BLE_USER_CFG;

#endif // USE_DEFAULT_USER_CFG

PIN_State zBlueLedState, zRedLedState;
PIN_Handle hBlueLed, hRedled;
U16 CRC16(U32 uStartAddr, U32 uLength, U16 uSeed);

#include <GPTimerCC26XX.h>

#include <inc/hw_types.h>
#include <driverlib/sys_ctrl.h>
#include <driverlib/PRCM.h>

unsigned long ulTick;
void GPTick (GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask);

GPTimerCC26XX_Value uTimer_Value = 0;

static void StackAssertHandler(uint8 assertCause, uint8 assertSubcause);

B32 bHeartbeatEnable = bTRUE;   // Only turned of in Assert


U32 uAppCRC, uBLEStackCRC,  uNVDCRC, uCodeCRC;        // Holds various CRC32 values, uCodeCRC should match bootloader CRC
U32 uAppCRC1;

//****** VERSIONING
#include "PhynGatt.h"

#define VERSION_ADDR (0x1FF80)
#define VERSION  (0.20)
I8 sBLE_FW_Version[BLE_VER_STR_LEN + 1] = "BLE App V0.20";
I8 sMsg[256];

U32 uCPURev;

const I8 sNoWatchdogMsg[]  = "00,99,NO_RELEASE - Watchdog Disabled";
const I8 sNoRCOSCogMsg[]   = "00,99,NO_RELEASE - USE_RCOSC turned off - Is LF Crystal Operational?";
const I8 sTerminalIfcMsg[] = "00,99,NO_RELEASE - Terminal Interface Enabled";
const I8 sBlinkDemoMsg[]   = "00,99,NO_RELEASE - Blink Demo";

I32 GapPri;
U32 uResetMask;

// TODO: Update Version Number
// TODO: Add GP Fault Handler
// TODO: Fix Asserts

/*******************************************************************************
 main
********************************************************************************/
int main()
{
        //B32 bWatchdogRunning;
        extern U16 uProgramCRC;

        // Always enable the bootloader so that resets always get back there
        #ifndef NO_BOOTLOADER
            BLE_EnableBootloader();
        #else
            #warning ***** BOOTLOADER DISABLED - JTAG USE ONLY *****
        #endif

        // Count total resets
        BLE_IncrementEventCount(EVENT_RESET);

        // Increment reset cause
        uResetMask = SysCtrlResetSourceGet();
        BLE_IncrementEventCount(POWER_ON_RESET + uResetMask);

       // uWarmResetReg = HWREG(PRCM_BASE + PRCM_O_WARMRESET);
        if (HWREG(PRCM_BASE + PRCM_O_WARMRESET) & 2) BLE_IncrementEventCount(LOCKUP_RESET);

        // Need Watchdog running immediately AFTER bootloader is enabled so that any setup fails got back to bootloader
        // Causes issues with RTOS setup so move to SimpleBLEPeripheral_taskFxn()
        //BLE_SetupAndStartWatchdog();

        uCPURev = ChipInfo_GetHwRevision();

        /* Register Application callback to trap asserts raised in the Stack */
        // App assert handler registered in SimpleBLEPeripheral_init()
        RegisterAssertCback(StackAssertHandler);
        bHeartbeatEnable = bTRUE;


        // Setup GPIO and Blink LEDs
        PIN_init(BoardGpioInitTable);

        // Setup the UART channel to the WiFi board
        Init_PWD_UART();

        // Cache setup
        VIMSConfigure(VIMS_BASE, TRUE, TRUE);             // Enable iCache prefetching
        VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);        // Enable cache

        // Calculate CRCs
        uAppCRC       = HapiCrc32((U8 *)APP_START_ADDR, APP_SIZE,                   0);
        uAppCRC1      = HapiCrc32((U8 *)APP_START_ADDR, APP_SIZE,                   2);
        uBLEStackCRC  = HapiCrc32((U8 *)BLE_STACK_ADDR, BLE_STACK_SIZE,             0);
        uCodeCRC      = HapiCrc32((U8 *)APP_START_ADDR, BLE_STACK_SIZE + APP_SIZE,  0);
        uNVDCRC       = HapiCrc32((U8 *)NVD_ADDR,       NVD_SIZE,                   0);


        // Task setup
        ICall_init();                                 // Initialize ICall module (task manager)

        #ifndef BLINKY_TEST                           // Normal Operation - enable BLE Stack and GAP tasks

            ICall_createRemoteTasks();                // Create remmote Tasks (BLE Stack)
            GAPRole_createTask();                     // Creates the GAP Task

            // Prevent GAP from running until it is setup
           // extern Task_Handle GapTaskHandle;
           // GapPri = Task_setPri(GapTaskHandle, -1);


        #else
                                                      // LED DEMO - Just blinking the LED, no BLE tasks
        #endif

        SimpleBLEPeripheral_createTask();             // Create the App Task

        // Send build info
        sprintf(sMsg,"00,99, BLE Ver %4.2f  %s  %s  CRC32=0x%08x", VERSION,__DATE__,__TIME__, uCodeCRC);
        UART_Naked_Tx(sMsg);

        #ifdef BLINKY_TEST
           // #warning "BLINKY_TEST turned on - DEMO Program Build"
           // UART_Naked_Tx(sBlinkDemoMsg);
       #endif

        #ifndef USE_RCOSC
            #warning "USE_RCOSC turned off - BLE must have operational LF Crystal"
            UART_Naked_Tx(sNoRCOSCogMsg);
        #else
            char sMsg[80];

            if (uCPURev < 23)
            {
                sprintf(sMsg,  "00, 99, BLE CPU Early revision detected (%d.%d) - BLE Message RX unlikely to work", uCPURev/10, uCPURev%10);
                UART_Naked_Tx(sMsg);
            }
        #endif

       #ifndef WD_ENABLE
           #warning "*** Watchdog Disabled ***"
            UART_Naked_Tx((I8 *)sNoWatchdogMsg);
       #endif


        #ifdef TERMINAL_INTERFACE
            #warning "*** Terminal Interface Enabled - JNAP Incompatable ***"
            UART_Naked_Tx((I8 *)sTerminalIfcMsg);
        #endif

        if (BLE_VerifyBootloaderIsEnabled())
            UART_Naked_Tx("00, 99,Bootloader is Enabled - Any RESET returns there");
        else
            UART_Naked_Tx("00, 99,NO_RELEASE **** Bootloader is not enabled");

        // enable interrupts and start SYS/BIOS - DOES NOT RETURN
        BIOS_start();

        // Should not get here - go back to bootloader if we do
        HapiResetDevice();
        return 0;
}


void GPTick (GPTimerCC26XX_Handle handle, GPTimerCC26XX_IntMask interruptMask)
{
    ulTick++;
    GPTimerCC26XX_enableInterrupt(handle, interruptMask);
}

/*******************************************************************************
   ASSERT HANDLERS
********************************************************************************/
#include "PWD_Asserts.h"
#include "PWD_Board.h"
#include <ti/drivers/UART.h>
#include "PWD_Util.h"

#include <ti/drivers/pin/PINCC26XX.h>  // For LEDs
#include "PWD_Board.h"

#include "PhynSerial.h"



extern UART_Handle hUARTHandle;

const char sAppAssertMsg[][25] =
{
  "",
  "\n99,WATCHDOG FAIL  ",         // PWD_SUBCAUSE_WATCHDOG
  "\n99,TEST ASSERT    ",         // PWD_SUBCAUSE_TEST

};

/*******************************************************************************
AppAssertHandler
********************************************************************************/
void AppAssertHandler(U8 uCause)
{
#if 0
    U8 sMsg[64] = "\x0d\x0a*** BLE APP ASSERT (Cause, Subcause) = ";
    U8 sAC[3], sASC[3];

    // If UART available, send as log message
    if (hUARTHandle)
    {

        // Keep as primitave as possible, no string library calls
        ByteToAscii(U8 assertCause, char * sAC);
        ByteToAscii(U8 assertSubcause, char * sASC);

        sMsg[31] = *&sAC[0];    sMsg[42] = *&sAC[1];    sMsg[43] = ',';
        sMsg[34] = *&sASC[0];   sMsg[45] = *&sASC[1];   sMsg[46] = 0;

        UART_Start_Write(sMsg, 37);
    }
#endif

    if (hUARTHandle)
        UART_Start_Write((char *)sAppAssertMsg[uCause], 20);

    // No UART, Blink LED
   while (1)
   {
        volatile F32 f=1.0;
        volatile I32 i,j;
        extern B32 bHeartbeatEnable;

        bHeartbeatEnable = bFALSE;
        PINCC26XX_setOutputValue(PIN_LED_BLUE, LED_OFF);
        PINCC26XX_setOutputValue(PIN_LED_RED,  LED_OFF);
        for (j=0; j<300000; j++) f = f * 1.000001;

        for (i=0; i<uCause+4; i++)
        {
            for (j=0; j<50000; j++) f = f * 1.000001;
            PINCC26XX_setOutputValue(PIN_LED_RED,  LED_ON);
            for (j=0; j<50000; j++) f = f * 1.000001;
            PINCC26XX_setOutputValue(PIN_LED_RED,  LED_OFF);

        }

    }


}

/*******************************************************************************
StackAssertHandler
********************************************************************************/
static void StackAssertHandler(uint8 assertCause, uint8 assertSubcause)
{

    char sMsg[64] = "\x0d\x0a*** BLE STACK ASSERT (Cause, Subcause)=";
    char sAC[3], sASC[3];

    // If UART available, send as log message
    if (hUARTHandle)
    {
        // Keep as primitave as possible, no string library calls
        ByteToAscii(assertCause, sAC);
        ByteToAscii(assertSubcause, sASC);

        sMsg[31] = *&sAC[0];    sMsg[42] = *&sAC[1];    sMsg[43] = ',';
        sMsg[34] = *&sASC[0];   sMsg[45] = *&sASC[1];   sMsg[46] = 0;

        UART_Start_Write(sMsg, 37);
    }

    PINCC26XX_setOutputValue(PIN_LED_BLUE, LED_OFF);
    PINCC26XX_setOutputValue(PIN_LED_RED,  LED_OFF);

    // No UART, Blink LED
   while (1)
   {
        volatile F32 f=1.0;
        volatile I32 i,j;

        for (j=0; j<300000; j++)f = f * 1.000001;

        for (i=0; i<assertSubcause; i++)
        {
            PINCC26XX_setOutputValue(PIN_LED_BLUE, LED_ON);
            PINCC26XX_setOutputValue(PIN_LED_RED,  LED_ON);
            for (j=0; j<300000; j++)f = f * 1.000001;
            PINCC26XX_setOutputValue(PIN_LED_BLUE, LED_OFF);
            PINCC26XX_setOutputValue(PIN_LED_RED,  LED_OFF);
            for (j=0; j<300000; j++)f = f * 1.000001;

        }


    }



#if 0
    sprintf(sAssertMsg,"ASSERT - assertCause=%d  assertSubcause=%d", assertCause, assertSubcause);
    while(1);


  // Open the display if the app has not already done so
  if ( !dispHandle )
  {
    dispHandle = Display_open(Display_Type_LCD, NULL);
  }

  Display_print0(dispHandle, 0, 0, ">>>STACK ASSERT");

  // check the assert cause
  switch (assertCause)
  {
    case HAL_ASSERT_CAUSE_OUT_OF_MEMORY:
      Display_print0(dispHandle, 0, 0, "***ERROR***");
      Display_print0(dispHandle, 2, 0, ">> OUT OF MEMORY!");
      break;

    case HAL_ASSERT_CAUSE_INTERNAL_ERROR:
      // check the subcause
      if (assertSubcause == HAL_ASSERT_SUBCAUSE_FW_INERNAL_ERROR)
      {
        Display_print0(dispHandle, 0, 0, "***ERROR***");
        Display_print0(dispHandle, 2, 0, ">> INTERNAL FW ERROR!");
      }
      else
      {
        Display_print0(dispHandle, 0, 0, "***ERROR***");
        Display_print0(dispHandle, 2, 0, ">> INTERNAL ERROR!");
      }
      break;

    case HAL_ASSERT_CAUSE_ICALL_ABORT:
      Display_print0(dispHandle, 0, 0, "***ERROR***");
      Display_print0(dispHandle, 2, 0, ">> ICALL ABORT!");
      HAL_ASSERT_SPINLOCK;
      break;

    default:
      Display_print0(dispHandle, 0, 0, "***ERROR***");
      Display_print0(dispHandle, 2, 0, ">> DEFAULT SPINLOCK!");
      HAL_ASSERT_SPINLOCK;
  }


#endif
}


/*******************************************************************************
 * @fn          smallErrorHook
 *
 * @brief       Error handler to be hooked into TI-RTOS.
 *
 * input parameters
 *
 * @param       eb - Pointer to Error Block.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void smallErrorHook(Error_Block *eb)
{
  for (;;);
}


/**********************************************************************************************************************
CRC16
    Calculates CRC for an array of data.

    sData:     Pointer to data.
    uLength:   Number of bytes to calculate checksum for
    uSeed:     CRC starting value

**********************************************************************************************************************/
U16 CRC16(U32 uStartAddr, U32 uLength, U16 uSeed)
{
    U16 uCRC = uSeed;

    // Use lookup table for speed
    static const U16 uCRCTable[256] =
            {0x0000,0x01189,0x02312,0x0329B,0x04624,0x057AD,0x06536,0x074BF,
            0x08C48,0x09DC1,0x0AF5A,0x0BED3,0x0CA6C,0x0DBE5,0x0E97E,0x0F8F7,
            0x01081,0x00108,0x03393,0x0221A,0x056A5,0x0472C,0x075B7,0x0643E,
            0x09CC9,0x08D40,0x0BFDB,0x0AE52,0x0DAED,0x0CB64,0x0F9FF,0x0E876,
            0x02102,0x0308B,0x00210,0x01399,0x06726,0x076AF,0x04434,0x055BD,
            0x0AD4A,0x0BCC3,0x08E58,0x09FD1,0x0EB6E,0x0FAE7,0x0C87C,0x0D9F5,
            0x03183,0x0200A,0x01291,0x00318,0x077A7,0x0662E,0x054B5,0x0453C,
            0x0BDCB,0x0AC42,0x09ED9,0x08F50,0x0FBEF,0x0EA66,0x0D8FD,0x0C974,
            0x04204,0x0538D,0x06116,0x0709F,0x00420,0x015A9,0x02732,0x036BB,
            0x0CE4C,0x0DFC5,0x0ED5E,0x0FCD7,0x08868,0x099E1,0x0AB7A,0x0BAF3,
            0x05285,0x0430C,0x07197,0x0601E,0x014A1,0x00528,0x037B3,0x0263A,
            0x0DECD,0x0CF44,0x0FDDF,0x0EC56,0x098E9,0x08960,0x0BBFB,0x0AA72,
            0x06306,0x0728F,0x04014,0x0519D,0x02522,0x034AB,0x00630,0x017B9,
            0x0EF4E,0x0FEC7,0x0CC5C,0x0DDD5,0x0A96A,0x0B8E3,0x08A78,0x09BF1,
            0x07387,0x0620E,0x05095,0x0411C,0x035A3,0x0242A,0x016B1,0x00738,
            0x0FFCF,0x0EE46,0x0DCDD,0x0CD54,0x0B9EB,0x0A862,0x09AF9,0x08B70,
            0x08408,0x09581,0x0A71A,0x0B693,0x0C22C,0x0D3A5,0x0E13E,0x0F0B7,
            0x00840,0x019C9,0x02B52,0x03ADB,0x04E64,0x05FED,0x06D76,0x07CFF,
            0x09489,0x08500,0x0B79B,0x0A612,0x0D2AD,0x0C324,0x0F1BF,0x0E036,
            0x018C1,0x00948,0x03BD3,0x02A5A,0x05EE5,0x04F6C,0x07DF7,0x06C7E,
            0x0A50A,0x0B483,0x08618,0x09791,0x0E32E,0x0F2A7,0x0C03C,0x0D1B5,
            0x02942,0x038CB,0x00A50,0x01BD9,0x06F66,0x07EEF,0x04C74,0x05DFD,
            0x0B58B,0x0A402,0x09699,0x08710,0x0F3AF,0x0E226,0x0D0BD,0x0C134,
            0x039C3,0x0284A,0x01AD1,0x00B58,0x07FE7,0x06E6E,0x05CF5,0x04D7C,
            0x0C60C,0x0D785,0x0E51E,0x0F497,0x08028,0x091A1,0x0A33A,0x0B2B3,
            0x04A44,0x05BCD,0x06956,0x078DF,0x00C60,0x01DE9,0x02F72,0x03EFB,
            0x0D68D,0x0C704,0x0F59F,0x0E416,0x090A9,0x08120,0x0B3BB,0x0A232,
            0x05AC5,0x04B4C,0x079D7,0x0685E,0x01CE1,0x00D68,0x03FF3,0x02E7A,
            0x0E70E,0x0F687,0x0C41C,0x0D595,0x0A12A,0x0B0A3,0x08238,0x093B1,
            0x06B46,0x07ACF,0x04854,0x059DD,0x02D62,0x03CEB,0x00E70,0x01FF9,
            0x0F78F,0x0E606,0x0D49D,0x0C514,0x0B1AB,0x0A022,0x092B9,0x08330,
            0x07BC7,0x06A4E,0x058D5,0x0495C,0x03DE3,0x02C6A,0x01EF1,0x00F78};

    //This algorithim is based on the IrDA LAP example.
    while(uLength--)
        uCRC = (uCRC >> 8) ^ uCRCTable[(uCRC ^ *(U8 *)uStartAddr++) & 0xff];

    uCRC = ~uCRC;       //Make this crc match the one's complement that is sent in the packet.

    return uCRC;
}


/********************************************************************************************************
        UNUSED & TEST Code
********************************************************************************************************/
#if 0
        // TEST EVENT COUNTERS
        U32 volatile uEventTest,i;

        uEventTest = BLE_IncrementEventCount(EVENT_TEST);
        for (i=0; i < 10009; i++);
        uEventTest = BLE_GetEventCount(EVENT_TEST);
        uEventTest = BLE_IncrementEventCount(EVENT_TEST);
        for (i=0; i < 10009; i++);
        uEventTest = BLE_IncrementEventCount(EVENT_TEST);
        for (i=0; i < 10009; i++);
        uEventTest = BLE_GetEventCount(EVENT_TEST);

//        for (i=0; i < 10; i++)
//            uEventTest = BLE_IncrementEventCount(EVENT_TEST);

        uEventTest = BLE_GetEventCount(EVENT_TEST);
        uEventTest = BLE_IncrementEventCount(EVENT_TEST);
        for (i=0; i < 10009; i++);
        uEventTest = BLE_IncrementEventCount(EVENT_TEST);
        for (i=0; i < 10009; i++);
        uEventTest = BLE_IncrementEventCount(EVENT_TEST);
        for (i=0; i < 10009; i++);
        uEventTest = BLE_GetEventCount(EVENT_TEST);
        uEventTest = BLE_IncrementEventCount(EVENT_TEST);
        for (i=0; i < 10009; i++);
        uEventTest = BLE_GetEventCount(EVENT_TEST);
#endif


#if defined (CC1350_LAUNCHXL) && defined (POWER_SAVING)
/*******************************************************************************
 * @fn          rFSwitchNotifyCb
 *
 * @brief       Power driver callback to toggle RF switch on Power state
 *              transitions.
 *
 * input parameters
 *
 * @param   eventType - The state change.
 * @param   eventArg  - Not used.
 * @param   clientArg - Not used.
 *
 * @return  Power_NOTIFYDONE to indicate success.
 */
static uint8_t rFSwitchNotifyCb(uint8_t eventType, uint32_t *eventArg,
                                uint32_t *clientArg)
{
  if (eventType == PowerCC26XX_ENTERING_STANDBY)
  {
    // Power down RF Switch
    PIN_setOutputValue(radCtrlHandle, Board_DIO30_SWPWR, 0);
  }
  else if (eventType == PowerCC26XX_AWAKE_STANDBY)
  {
    // Power up RF Switch
    PIN_setOutputValue(radCtrlHandle, Board_DIO30_SWPWR, 1);
  }

  // Notification handled successfully
  return Power_NOTIFYDONE;
}
#endif //CC1350_LAUNCHXL || POWER_SAVING


#if 0

typedef struct GPTimerCC26XX_Params
{
    GPTimerCC26XX_Width     width;          /*!< Timer configuration (32/16-bit)  */
    GPTimerCC26XX_Mode      mode;           /*!< Timer mode */
    GPTimerCC26XX_DebugMode debugStallMode; /*!< Timer debug stall mode */
} GPTimerCC26XX_Params;

GPTimerCC26XX_Params Timer_A_Params =
{
    .width          = GPT_CONFIG_16BIT,
    .mode           = GPT_MODE_PERIODIC_UP,
    .debugStallMode = GPTimerCC26XX_DEBUG_STALL_OFF
};
#endif
