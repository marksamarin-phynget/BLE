#include "PhynDefs.h"
#include "PWD_Util.h"

#include <driverlib/flash.h>
#include <driverlib/watchdog.h>
#include <driverlib/vims.h>

#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <string.h>

//#include "PhynSerial.h"
#include "PhynUart.h"
#include "peripheral.h"

#include "simple_peripheral.h"

B8 bClearEvents;



/***********************************************************************************************
ByteToAscii
***********************************************************************************************/
void ByteToAscii(U8 cByte, char * sAsc)
{
    U8 H,L;

    H = cByte >> 4;
    L = cByte & 0x0F;

    sAsc[0] = (H>9) ?  0x41 + H - 10 :  0x30 + H;
    sAsc[1] = (L>9) ?  0x41 + L - 10 :  0x30 + L;
}




/***********************************************************************************************
UnsHalfToAscii
***********************************************************************************************/
void UnsHalfToAscii(U16 uHalf, char * sAsc)
{
    ByteToAscii((U8)(uHalf >> 8), sAsc);
    ByteToAscii((U8)(uHalf & 0xFF), &sAsc[2]);
}


/***********************************************************************************************
 UnsLongToAscii
***********************************************************************************************/
void UnsLongToAscii(U32 uUnsLong, char * sAsc)
{
    U8 B;
    I32 i;

    for (i=3; i>=0; i--)
    {
        B = (uUnsLong >> (i*8)) & 0xFF;
        ByteToAscii(B, &sAsc[i*2]);
    }

}

#define BACKDOOR_ENABLE_PATTERN (0xC5FF05C5)
#define BACKDOOR_ENABLE_ADDRESS (0x58)         /* 0x1FFFD8   (0x58=88) = 0x20000-x28*/

/***********************************************************************************************
 BLE_EnableBootloader

     Enables the bootloader on the next reset by modifying the related CCFG area registers.
    Due to RAM limits, we cannot preserve the entire 4K FLASH page that contains this data.
***********************************************************************************************/
void BLE_EnableBootloader(void)
{

    U8  uCCFGArea[RESERVED_LAST_PAGE_BYTES];
    U32 volatile uFlashOpResult;   // volatile to avoid unused var warning
    U32 uSectorSize;

    // reenable Bootloader
    uSectorSize = FlashSectorSizeGet();

    if (uSectorSize != 0x1000)
        UART_Naked_Tx("00, 99,NO_RELEASE **** FLASH SECTOR SIZE != 4K");

    // Read in last 128 bytes of config area (only last 88 bytes used)
    memcpy (uCCFGArea, (U8 *)(0x20000 - RESERVED_LAST_PAGE_BYTES), RESERVED_LAST_PAGE_BYTES);

    // Set Enables
    *(U32 *)(&uCCFGArea[RESERVED_LAST_PAGE_BYTES-20]) = 0xFFFFFFFF;                  // IMAGE _VALID @ Address: 0x1FFFEC - (Device does not allow manual erase)
    *(U32 *)(&uCCFGArea[RESERVED_LAST_PAGE_BYTES-0x28]) = BACKDOOR_ENABLE_PATTERN;   // BOOTLOADER_CONFIG @ Address: 0x1FFFD8


    // Clear the event flags if necessary
    if (bClearEvents)
    {
        memset(&uCCFGArea, 0xFF, MAX_COUNT_PER_EVENT*4);
        bClearEvents = bFALSE;
    }

    // Write back modifications - Destroys addresses 0x1F000 to (0x20000 - NVD_PRESERVE_BYTES)
    VIMSModeSafeSet( VIMS_BASE,VIMS_MODE_OFF, TRUE);
    uFlashOpResult =  FlashSectorErase(0x1F000);
    uFlashOpResult =  FlashProgram (uCCFGArea, 0x20000 - RESERVED_LAST_PAGE_BYTES, RESERVED_LAST_PAGE_BYTES);
    VIMSModeSafeSet( VIMS_BASE,VIMS_MODE_ENABLED, TRUE);
}


/***********************************************************************************************
 BLE_VerifyBootloaderIsEnabled
***********************************************************************************************/
B32 BLE_VerifyBootloaderIsEnabled(void)
{
    if (*(U32 *)(0x1FFEC) != 0)
        return bTRUE;

    if (*(U32 *)(0x1FFD8) == BACKDOOR_ENABLE_PATTERN)
        return bTRUE;

    return bFALSE;
}

/***********************************************************************************************
 BLE_IncrementEventCount

     Enables the bootloader on the next reset by modifying the related CCFG area registers.
    Due to RAM limits, we cannot preserve the entire 4K FLASH page that contains this data.
***********************************************************************************************/
I32 BLE_IncrementEventCount(EVENT_ENUM eEvent)
{
    U32 i, uBitMask, uAddr, uVal;

    if (eEvent < 32)
    {
        uBitMask = 1 << (U32)eEvent;

        // Find first non-occurrence
        for (i=0; i<MAX_COUNT_PER_EVENT; i++)
        {
            uAddr = ONESHOT_EVENT_MASK_ADDR + i*4;
            uVal = *(U32 *)uAddr;

            if ((uVal & uBitMask) == uBitMask)
              break;
        }
        // Clear the bit in the word in flash corresponding to the nth occurance of the event
        if (i < MAX_COUNT_PER_EVENT)
        {
            uVal  &= ~uBitMask;
            VIMSModeSafeSet( VIMS_BASE,VIMS_MODE_OFF, TRUE);
            FlashProgram (&uVal, uAddr, 4);
            VIMSModeSafeSet( VIMS_BASE,VIMS_MODE_ENABLED, TRUE);
        }

        return i+1;
    }

    return -1;
}

/***********************************************************************************************
 BLE_GetEventCount
***********************************************************************************************/
I32 BLE_GetEventCount(EVENT_ENUM eEvent)
{
    U32 i, uBitMask, uAddr, uVal;

    if (eEvent < 32)
    {
        uBitMask = 1 << (U32)eEvent;

        // Find first non-occurrence
        for (i=0; i<MAX_COUNT_PER_EVENT; i++)
        {
            uAddr = ONESHOT_EVENT_MASK_ADDR + i*4;
            uVal = *(U32 *)uAddr;

            if ((uVal & uBitMask) == uBitMask)
              break;
         }

        return i;
    }

    return -1;
}


/***********************************************************************************************
 BLE_ClearEvents
***********************************************************************************************/
void BLE_ClearEvents()
{
    bClearEvents = bTRUE;     // BLE_EnableBootloader() will erase event flags now
    BLE_EnableBootloader();   // Erases NVD area and enables Bootloader
}



/***********************************************************************************************
 BLE_SetDeviceName
    Sets device name as stored in FLASH, but only if it has not been set yet (=FFs)
***********************************************************************************************/
B32 BLE_SetDeviceName(I8 *sDevName)
{
    I32  sOrigDevName[DEVICENAME_PRESERVE_BYTES + 1];

    // Name can only be written once into FLASH
    if (!BLE_GetDeviceName(sOrigDevName))
    {
        // Program new name into flash - disable cache to ensure readback is correct
        VIMSModeSafeSet( VIMS_BASE,VIMS_MODE_OFF, TRUE);
        FlashProgram (sDevName, DEVICENAME_PRESERVE_ADDR, DEVICENAME_PRESERVE_BYTES+1);
        VIMSModeSafeSet( VIMS_BASE,VIMS_MODE_ENABLED, TRUE);

        // Verify
        if (!strncmp((I8 *)DEVICENAME_PRESERVE_ADDR, sDevName, DEVICENAME_PRESERVE_BYTES))
           return bTRUE;
    }

    return bFALSE;
}

/***********************************************************************************************
 BLE_GetDeviceName
     Gets device name as stored in FLASH
***********************************************************************************************/
B32 BLE_GetDeviceName(I8 *sDevName)
{
    strncpy(sDevName, (I8 *)DEVICENAME_PRESERVE_ADDR, DEVICENAME_PRESERVE_BYTES);

    if (*(U32 *)(&sDevName[0]) == 0xFFFFFFFF)
        return bFALSE;

    return bTRUE;
}



/***********************************************************************************************
 BLE_WatchdogExpired
***********************************************************************************************/
__interrupt void BLE_WatchdogExpired()
{
    U32  i;
    static F32 volatile f = 1.00;

    IntDisable(INT_WDT_IRQ);
    BLE_IncrementEventCount(WD_RESET);

    for (i=0; i<4000000; i++)f = f * 1.00001;

    HapiResetDevice();
}



U32 uLowestWDValue;
/***********************************************************************************************
 BLE_SetupAndStartWatchdog
***********************************************************************************************/
void BLE_SetupAndStartWatchdog()
{

    U32 key;

    /* disable preemption while unlocking WatchDog registers */
    key = Hwi_disable();

    BLE_HitWatchdog();    // clear pending wdt triggers

    uLowestWDValue = WATCHDOG_SET_VALUE;

    IntRegister(INT_WDT_IRQ, BLE_WatchdogExpired);
    IntPrioritySet(INT_WDT_IRQ, INT_PRI_LEVEL0);     // highest priority
    IntEnable(INT_WDT_IRQ);

    /* unlock the Watchdog configuration registers */
    WatchdogUnlock();
    while(WatchdogLockState() == WATCHDOG_LOCK_LOCKED);

    WatchdogReloadSet(WATCHDOG_SET_VALUE);
    //WatchdogStallEnable();

    WatchdogIntTypeSet(WATCHDOG_INT_TYPE_INT);
    WatchdogEnable();
    WatchdogLock();

    Hwi_restore(key);

}


/***********************************************************************************************
 BLE_HitWatchdog
***********************************************************************************************/
void BLE_HitWatchdog()
{
    U32 uWV;

    HWREG(WDT_BASE + WDT_O_LOCK) = 0x1ACCE551;
    HWREG(WDT_BASE + WDT_O_ICR) = 0x0000;

    uWV =  WatchdogValueGet();

    if (uWV < uLowestWDValue)
        uLowestWDValue = uWV;

}

U32 uStackUsedBytes;
U32 uStackTopAddr, uStackSizeBytes;
F32 fStackUsedPct;


U32 uGapTaskStackUsedBytes=0;
F32 fGapStackUsedPct=0.0;
extern U8 gapRoleTaskStack[];
U32 *uGapTaskStackUsedPtr  = (U32 *)&gapRoleTaskStack[GAPROLE_TASK_STACK_SIZE-8];


U32 uAppTaskStackUsedBytes=0;
F32  fAppStackUsedPct = 0.0;
extern U8 sbpTaskStack[];
U32 *uAppTaskStackUsedPtr  = (U32 *)&sbpTaskStack[SBP_TASK_STACK_SIZE-8];

U32 uUartTaskStackUsedBytes=0;
F32 fUartStackUsedPct = 0.0;
extern U8 UartTaskStack[];
U32 *uUartTaskStackUsedPtr = (U32 *)&UartTaskStack[UART_TASK_STACK_SIZE-8];



/***********************************************************************************************
 BLE_StackCheck
***********************************************************************************************/
void BLE_StackCheck()
{
    extern int __STACK_TOP, __STACK_SIZE;
    U32 uAddr;
    U32 i;

    /*********************************************************
     SYSTEM STACK
    **********************************************************/
    uStackTopAddr = (U32)&__STACK_TOP;
    uStackSizeBytes = (U32)&__STACK_SIZE;

    uAddr = uStackTopAddr;

    for (uAddr = uStackTopAddr; uAddr > uStackTopAddr - (U32)uStackSizeBytes; uAddr-=4)
        if (*(U32 *)uAddr == 0xBEBEBEBE)
            break;

    uStackUsedBytes =  uStackTopAddr - uAddr;

    fStackUsedPct =  (F32)uStackUsedBytes / (F32)uStackSizeBytes * 100.0;

    /*********************************************************
     GAP Task STACK
    **********************************************************/
    while ((U8 *)uGapTaskStackUsedPtr > gapRoleTaskStack)
        if (*uGapTaskStackUsedPtr == 0xBEBEBEBE)
        {
            for (i=0;i<16;i++)
                if (*(uGapTaskStackUsedPtr-i) != 0xBEBEBEBE)
                {   i=99;
                    uGapTaskStackUsedPtr--;
                }

            if (i==16)
                break;
        }
        else
            uGapTaskStackUsedPtr--;

    uGapTaskStackUsedBytes = &gapRoleTaskStack[GAPROLE_TASK_STACK_SIZE] - (U8 *)uGapTaskStackUsedPtr;
    fGapStackUsedPct = (F32)uGapTaskStackUsedBytes / (F32)GAPROLE_TASK_STACK_SIZE * 100.0;

    /*********************************************************
     App Task STACK
    **********************************************************/
    while ((U8 *)uAppTaskStackUsedPtr > sbpTaskStack)
        if (*uAppTaskStackUsedPtr == 0xBEBEBEBE)
               {
                   for (i=0;i<16;i++)
                       if (*(uAppTaskStackUsedPtr-i) != 0xBEBEBEBE)
                       {   i=99;
                       uAppTaskStackUsedPtr--;
                       }

                   if (i==16)
                       break;
               }
               else
                   uAppTaskStackUsedPtr--;

    uAppTaskStackUsedBytes = &sbpTaskStack[SBP_TASK_STACK_SIZE] - (U8 *)uAppTaskStackUsedPtr;
    fAppStackUsedPct = (F32)uAppTaskStackUsedBytes / (F32)SBP_TASK_STACK_SIZE * 100.0;

    /*********************************************************
     UART Task STACK
    **********************************************************/
    while ((U8 *)uUartTaskStackUsedPtr > UartTaskStack)
        if (*uUartTaskStackUsedPtr == 0xBEBEBEBE)
               {
                   for (i=0;i<16;i++)
                       if (*(uUartTaskStackUsedPtr-i) != 0xBEBEBEBE)
                       {   i=99;
                       uUartTaskStackUsedPtr--;
                       }

                   if (i==16)
                       break;
               }
               else
                   uUartTaskStackUsedPtr--;


    uUartTaskStackUsedBytes = &UartTaskStack[UART_TASK_STACK_SIZE] - (U8 *)uUartTaskStackUsedPtr;
    fUartStackUsedPct = (F32)uUartTaskStackUsedBytes / (F32)UART_TASK_STACK_SIZE * 100.0;




}

