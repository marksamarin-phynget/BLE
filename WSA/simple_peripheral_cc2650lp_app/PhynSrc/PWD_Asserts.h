/******************************************************************************


 ******************************************************************************
 

 ******************************************************************************
 Release Name: ble_sdk_2_02_01_18
 Release Date: 2016-10-26 15:20:04
 *****************************************************************************/

#ifndef PWD_ASSERT_H
#define PWD_ASSERT_H

#define PWD_FORCE_ASSERT(x)       AppAssertHandler(x)
#define PWD_ASSERT(x,y)           (if(!x) PWD_TRAP(y))
    
// Assert Causes (0xE0 fir APP)
//#define PWD_ASSERT                  0xE0

#define PWD_ASSERT_CAUSE_TEST_ASSERT    0x01
#define PWD_ASSERT_CAUSE_WATCHDOG       0x02


void AppAssertHandler(U8 uCause);
#endif

