#include "PhynDefs.h"
#include "PWD_Util.h"

#include <driverlib/flash.h>
#include <driverlib/watchdog.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

//#include "PhynSerial.h"
#include "PhynUart.h"
//#include "PhynGatt.h"



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
#define BACKDOOR_ENABLE_ADDRESS (0x58)         /* 0x1FFFD8   (0x58=88)*/

/***********************************************************************************************
 BLE_EnableBootloader
***********************************************************************************************/
void BLE_EnableBootloader(void)
{

    U8  uCCFGArea[128];
    U32 uFlashOpResult, uSectorSize;

    // reenable Bootloader
    uSectorSize = FlashSectorSizeGet();

    if (uSectorSize != 0x1000)
        UART_Naked_Tx("00, 99,NO_RELEASE **** FLASH SECTOIR SIZE != 4K");

    // Read in last 128 bytes of config area (only last 88 bytes used)
    memcpy (uCCFGArea, (U8 *)0x1FF80, 128);

    // Set Enables
    *(U32 *)(&uCCFGArea[124]) = 0xFFFFFFFF;
    *(U32 *)(&uCCFGArea[BACKDOOR_ENABLE_ADDRESS]) = BACKDOOR_ENABLE_PATTERN;

    // Write back modifications - Destroys addresses 0x1F000-0x1FF7F
    uFlashOpResult =  FlashSectorErase(0x1F000);
    uFlashOpResult =  FlashProgram (uCCFGArea, 0x20000 - 128, 128);
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
 BLE_WatchdogExpired
***********************************************************************************************/
__interrupt void BLE_WatchdogExpired()
{
    IntDisable(INT_WDT_IRQ);
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
/***********************************************************************************************
 BLE_HitWatchdog
***********************************************************************************************/
void BLE_StackCheck()
{
    extern int __STACK_TOP, __STACK_SIZE;
    U32 uAddr;


    uStackTopAddr = (U32)&__STACK_TOP;
    uStackSizeBytes = (U32)&__STACK_SIZE;

    uAddr = uStackTopAddr;

    for (uAddr = uStackTopAddr; uAddr > uStackTopAddr - (U32)uStackSizeBytes; uAddr-=4)
        if (*(U32 *)uAddr == 0xBEBEBEBE)
            break;

    uStackUsedBytes =  uStackTopAddr - uAddr;

    fStackUsedPct =  (F32)uStackUsedBytes / (F32)uStackSizeBytes * 100.0;

}
