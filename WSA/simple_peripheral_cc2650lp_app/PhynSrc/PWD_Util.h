#ifndef __PWD_Util_H
#define __PWD_Util_H

#include "PhynDefs.h"

typedef enum
{
    EVENT_RESET=0,
    EVENT_MALLOC_FAIL=1,
    EVENT_SYSTACK_OVERFLOW=2,
    EVENT_BLE_CONNECT=3,

    EVENT_TEST = 31


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
#endif
