#ifndef __PWD_Util_H
#define __PWD_Util_H

#include "PhynDefs.h"

typedef enum
{
    EVENT_RESET             = 0,
    EVENT_WD_FAIL           = 1,
    EVENT_MALLOC_FAIL       = 2,
    EVENT_SYSTACK_OVERFLOW  = 3,
    EVENT_BLE_CONNECT       = 4,

    // Reset Causes - values must match order of RESETCTL.RESET_SRC field in CPU
    POWER_ON_RESET          = 21,
    RESET_PIN_RESET         = 22,
    BROWN_OUT_VDDS_RESET    = 23,
    BROWN_OUT_VDD_RESET     = 24,
    BROWN_OUT_VDDR_RESET    = 25,
    CLOCK_LOSS_RESET        = 26,
    SW_SYSRESET_RESET       = 27,
    SW_PRCM_RESET           = 28,
    WD_RESET                = 29,
    LOCKUP_RESET            = 30,

    EVENT_TEST              = 31
}EVENT_ENUM;


void ByteToAscii(U8 cByte, char * sAsc);
void UnsLongToAscii(U32 uUnsLong, char * sAsc);
void UnsHalfToAscii(U16 uHalf, char * sAsc);

void BLE_EnableBootloader(void);
B32  BLE_VerifyBootloaderIsEnabled(void);

void BLE_HitWatchdog();
void  BLE_SetupAndStartWatchdog();

void BLE_StackCheck();

// Logging by flag array in FLASH
I32 BLE_IncrementEventCount(EVENT_ENUM eEvent);
I32 BLE_GetEventCount(EVENT_ENUM eEvent);
void BLE_ClearEvents();

B32 BLE_SetDeviceName(I8 *sDevName);
B32 BLE_GetDeviceName(I8 *sDevName);
#endif
