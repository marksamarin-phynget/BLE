#ifndef _PhynDefs_H_
#define _PhynDefs_H_

typedef char                    I8;
typedef unsigned char           U8;

typedef signed   short          U16;
typedef unsigned short          I16;

typedef signed   int            I32;
typedef unsigned int            U32;

typedef signed   long long      I64;
typedef unsigned long long      U64;

typedef float                   F32;
typedef double                  F64;

typedef U8                      B8;
typedef U32                     B32;

#define bTRUE   1
#define bFALSE  0


#define WATCHDOG_SET_VALUE (5000000)
#define RX_MESSAGE_TIMEOUT_MS (1000)

// FLASH CONFIGURATION - Mapped by linker script, not these declarations
#define APP_START_ADDR (0)
#define APP_SIZE       (0xF000)

#define BLE_STACK_ADDR (0xF000)
#define BLE_STACK_SIZE (0x10000)

#define NVD_ADDR       (0x1F000)
#define NVD_SIZE       (0x1000)


#define MAX_EVENT_OCCURANCES_LOGGED (32)

#define NVD_PRESERVE_BYTES  (512)     /* This has to be balanced against the system stack usage */
#define ONESHOT_EVENT_MASK_ADDR (0x20000 - NVD_PRESERVE_BYTES)

// Watchdog Service
#ifdef WD_ENABLE
    void BLE_HitWatchdog(void);
    #define HITWD() BLE_HitWatchdog()
#else
    #define HITWD()
#endif

#endif
