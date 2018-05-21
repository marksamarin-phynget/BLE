#ifndef __PWD_Util_H
#define __PWD_Util_H

#include "PhynDefs.h"



void ByteToAscii(U8 cByte, char * sAsc);
void UnsLongToAscii(U32 uUnsLong, char * sAsc);
void UnsHalfToAscii(U16 uHalf, char * sAsc);

void BLE_EnableBootloader(void);
B32  BLE_VerifyBootloaderIsEnabled(void);

void BLE_HitWatchdog();
void  BLE_SetupAndStartWatchdog();

void BLE_StackCheck();

#endif
