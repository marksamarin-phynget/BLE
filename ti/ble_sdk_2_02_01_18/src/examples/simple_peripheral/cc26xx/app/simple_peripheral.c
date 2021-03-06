/******************************************************************************

 @file  simple_peripheral.c

 @brief This file contains the Simple BLE Peripheral sample application for use
        with the CC2650 Bluetooth Low Energy Protocol Stack.

 Group: WCS, BTS
 Target Device: CC2650, CC2640, CC1350

 ******************************************************************************
 
  ******************************************************************************
 Release Name: ble_sdk_2_02_01_18
 Release Date: 2016-10-26 15:20:04
 *****************************************************************************/

#include <string.h>
#include <stdio.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "hci_tl.h"
#include "gatt.h"
#include "linkdb.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "simple_gatt_profile.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "icall_apimsg.h"

#include "util.h"

#include "simple_peripheral.h"

// Necessary for Internal LF Oscillator
#ifdef USE_RCOSC
    //#include "C:\BLE\ti\simplelink\ble_sdk_2_02_01_18\src\common\cc26xx\rcosc\rcosc_calibration.h"
    #include "rcosc_calibration.h"
#endif

#include <ti/mw/display/Display.h>
#include "board_key.h"
#include "board.h"
#include "simple_peripheral.h"

// For routing LF Osc out of GPIO
#include <driverlib/aon_ioc.h>


// To determine silicon version
#include <driverlib/chipinfo.h>

#if defined( USE_FPGA ) || defined( DEBUG_SW_TRACE )
#include <driverlib/ioc.h>
#endif // USE_FPGA | DEBUG_SW_TRACE


// Phyn Datatypes
#include "PhynDefs.h"

// Phyn BLE Profile
#include "PhynGatt.h"

// for MAX_PDU_SIZE define
#include "ble_user_config.h"


// For LEDs
#include <ti/drivers/pin/PINCC26XX.h>
#include "PWD_Board.h"

// For UART
#include "PhynUart.h"
#include "PhynSerial.h"
#include "PWD_Util.h"

// For system tick access
#include <driverlib\systick.h>


/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr;  // event header.
} sbpEvt_t;

#ifdef WD_ENABLE
    B32 bWatchdogNotRunning = bFALSE;
#endif

U32 uAppEventCounter;

B32 bStack75Pct, bStack90Pct, bStack95Pct, bStack100Pct;

const U8  FALSE_U8=0, TRUE_U8=1;

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID AppEntityID;

// Semaphore globally used to post events to the application thread
static ICall_Semaphore AppSem;

// Clock instances for internal periodic events.
static  Clock_Struct    periodicClock;
static  Clock_Struct    zHeartbeatClock;
        Clock_Struct    zMsgTimeoutClock;

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// events flag for internal application events.
static uint16_t events;

U32 uATT_MTU = MAX_PDU_SIZE-4;

B32 bNewAppMsg;


// Task configuration
Task_Struct sbpTask;
Char sbpTaskStack[SBP_TASK_STACK_SIZE];

// Profile state and parameters
//static gaprole_States_t gapProfileState = GAPROLE_INIT;

///* GAP - SCAN RSP data (max size = 31 bytes)
uint8_t scanRspData[] =
{
  // complete name
  0x10,  // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'P',
  'o',
  'w',
  'e',
  'r',
  'e',
  'd',
  ' ',
  'b',
  'y',
  ' ',
  'P',
  'h',
  'y',
  'n',

  // connection interval range
  0x05,   // length of this data
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
  LO_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),   // 100ms
  HI_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
  LO_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),   // 1s
  HI_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm

};


// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8_t advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID, to notify central devices what services are included
  // in this peripheral
  // removed oad STUFF - SEE GARBADE AREA

  0x05,  // lenght of this data
  GAP_ADTYPE_16BIT_MORE,      // some of the UUID's, but not all

  #ifndef PHYN_PWD
      LO_UINT16(SIMPLEPROFILE_SERV_UUID),
      HI_UINT16(SIMPLEPROFILE_SERV_UUID)
   #else
      LO_UINT16(PHYNPROFILE_UUID),
      HI_UINT16(PHYNPROFILE_UUID)
   #endif
};

// GAP GATT Attributes
//static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "Simple BLE Peripheral";
I8 attDeviceName[] = "Test";


// Globals used for ATT Response retransmission
static gattMsgEvent_t *pAttRsp = NULL;
static uint8_t rspTxRetry = 0;


U32 uGapTaskCounter, uAppTaskCounter, uUartTaskCounter;


extern B32  bTenConsecutiveZeros;   // Set by UART Rx to trigger CPU reset

/*********************************************************************
 * FUNCTIONS
*********************************************************************/
static void     Heartbeat_Service                               (UArg arg);
//static void     Uart_Testing                                    (UArg arg);

static void     SimpleBLEPeripheral_init                        (void);
static void     SimpleBLEPeripheral_taskFxn                     (UArg a0, UArg a1);

static uint8_t  SimpleBLEPeripheral_processStackMsg             (ICall_Hdr *pMsg);
static uint8_t  SimpleBLEPeripheral_processGATTMsg              (gattMsgEvent_t *pMsg);
static void     SimpleBLEPeripheral_processAppMsg               (sbpEvt_t *pMsg);
static void     SimpleBLEPeripheral_processStateChangeEvt       (gaprole_States_t newState);
static void     SimpleBLEPeripheral_processCharValueChangeEvt   (uint8_t paramID);

static void     SimpleBLEPeripheral_performPeriodicTask         (void);
static void     SimpleBLEPeripheral_clockHandler                (UArg arg);

static void     SimpleBLEPeripheral_sendAttRsp                  (void);
static void     SimpleBLEPeripheral_freeAttRsp                  (uint8_t status);

static void     SimpleBLEPeripheral_stateChangeCB               (gaprole_States_t newState);
static void     SimpleBLEPeripheral_enqueueMsg                  (uint8_t event, uint8_t state);

//void Phyn_Set_Scan_Response_Name(I8 * sNewName);


void GAP_Setup();
/*********************************************************************
 * PROFILE CALLBACKS
*********************************************************************/

// GAP Role Callbacks
static gapRolesCBs_t SimpleBLEPeripheral_gapRoleCBs =
{
  SimpleBLEPeripheral_stateChangeCB     // Profile State Change Callbacks
};

/* GAP Bond Manager Callbacks
static gapBondCBs_t simpleBLEPeripheral_BondMgrCBs =
{
  NULL, // Passcode callback (not used by application)
  NULL  // Pairing / Bonding state Callback (not used by application)
};
*/



/*********************************************************************
 * Globals
*********************************************************************/
Display_Handle dispHandle = NULL;


/*********************************************************************
  SimpleBLEPeripheral_createTask
*********************************************************************/
void SimpleBLEPeripheral_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = sbpTaskStack;
  taskParams.stackSize = SBP_TASK_STACK_SIZE;
  taskParams.priority = SBP_TASK_PRIORITY;

  Task_construct(&sbpTask, SimpleBLEPeripheral_taskFxn, &taskParams, NULL);
}



/*********************************************************************
 * @fn      SimpleBLEPeripheral_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc), and
 *          profile initialization/setup.
 *
 * @param   None.
 *
 * @return  None.
 */

void AppAssertHandler(uint8 assertCause, uint8 assertSubcause);




static void SimpleBLEPeripheral_init(void)
{
    // ******************************************************************
    // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
    // ******************************************************************
    // Register the current thread as an ICall dispatcher application
    // so that the application can send and receive messages.


    ICall_registerApp(&AppEntityID, &AppSem);


    //halAssertInit(AppAssertHandler, HAL_ASSERT_LEGACY_MODE_DISABLED );

    #ifdef USE_RCOSC
          RCOSC_enableCalibration();
    #endif // USE_RCOSC

    #if defined( DEBUG_SW_TRACE )
          IOCPortConfigureSet(IOID_8, IOC_PORT_RFC_TRC, IOC_STD_OUTPUT | IOC_CURRENT_4MA | IOC_SLEW_ENABLE);      // configure RF Core tracer IO
    #endif // DEBUG_SW_TRACE

    volatile uint32_t uBoardRev = ChipInfo_GetMinorHwRev();

    IOCPortConfigureSet(IOID_5, IOC_PORT_AON_CLK32K, IOC_STD_OUTPUT);
    AONIOC32kHzOutputEnable();


    // Create an RTOS queue for message from profile to be sent to app.
    appMsgQueue = Util_constructQueue(&appMsg);

    // Create one-shot clocks for internal periodic events.
    Util_constructClock(&periodicClock, SimpleBLEPeripheral_clockHandler, SBP_PERIODIC_EVT_PERIOD, 0, false, SBP_PERIODIC_EVT);

    // Create clock for heartbeat.
    Util_constructClock(&zHeartbeatClock, Heartbeat_Service, 500, 0, true, SBP_PERIODIC_EVT);

    // Create clock for UART Testing.
    Util_constructClock(&zMsgTimeoutClock, Reset_Serial_Rx, RX_MESSAGE_TIMEOUT_MS, 0, false, SBP_PERIODIC_EVT);


  Phyn_AddServices();                          // Add Phyn application Services


  // Register for GATT local events and ATT Responses pending for transmission
  //GATT_RegisterForMsgs(AppEntityID);


  // HCI_LE_ReadMaxDataLenCmd();


  /*********************************************************************
   GATT Setup
  *********************************************************************/
#ifndef WAIT_FOR_ID_CHANGE

   GAP_Setup();


   // Register for GATT local events and ATT Responses pending for transmission
   GATT_RegisterForMsgs(AppEntityID);

   HCI_LE_ReadMaxDataLenCmd();



#endif

}


void GAP_Setup()
{

    //ICall_createRemoteTasks();                // Create remmote Tasks (BLE Stack)
    //GAPRole_createTask();                     // Creates the GAP Task

    GAP_SetParamValue(TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL);

     // Setup the GAP Peripheral Role Profile
     {
       // For all hardware platforms, device starts advertising upon initialization
       uint8_t initialAdvertEnable = TRUE;
       extern U16 uProgCRC;

       // By setting this to zero, the device will go into the waiting state after
       // being discoverable for 30.72 second, and will not being advertising again
       // until the enabler is set back to TRUE
       uint16_t volatile advertOffTime = 0;               // volatile to suppress no reference warning


       // Set the GAP Role Parameters

       // Advertising Setup
       GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),     &initialAdvertEnable);
       //GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, 1,   &FALSE_U8);


       // Scan response (device) name setup -
       // Will use one in FLASH if stored, reve3rts to default name if not stored
       I8 sName[DEVICENAME_PRESERVE_BYTES+1];
       I8 sDefDevName[DEVICENAME_PRESERVE_BYTES+1];

       strcpy(sDefDevName, DEFAULT_DEVICENAME);

       if (BLE_GetDeviceName(sName))
           Set_Scan_Response_Name(sName);
       else
           Set_Scan_Response_Name(sDefDevName);

       U8  enableUpdateRequest = DEFAULT_ENABLE_UPDATE_REQUEST;
       U16 desiredMinInterval  = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
       U16 desiredMaxInterval  = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
       U16 desiredSlaveLatency = DEFAULT_DESIRED_SLAVE_LATENCY;
       U16 desiredConnTimeout  = DEFAULT_DESIRED_CONN_TIMEOUT;

       GAPRole_SetParameter(GAPROLE_ADVERT_DATA,            sizeof(advertData), advertData);
       GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE,    sizeof(uint8_t),    &enableUpdateRequest);
       GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL,      sizeof(uint16_t),   &desiredMinInterval);
       GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL,      sizeof(uint16_t),   &desiredMaxInterval);
       GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY,          sizeof(uint16_t),   &desiredSlaveLatency);
       GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER,     sizeof(uint16_t),   &desiredConnTimeout);
     }

     // Set the GAP Characteristics
    // GGS_SetParameter(GGS_DEVICE_NAME_ATT, strlen(attDeviceName), attDeviceName);


     // Set advertising interval
     {
       uint16_t advInt = DEFAULT_ADVERTISING_INTERVAL;

       GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, advInt);
       GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, advInt);
       GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, advInt);
       GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, advInt);
     }

     // Setup the GAP Bond Manager - BONDING IS DISABLED
     {
       uint8_t bonding = FALSE;                                     // NO BONDING
       GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);

#if 0
       uint32_t passkey = 0; // passkey "000000"
       uint8_t pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
       uint8_t mitm = TRUE;
       uint8_t ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
       uint8_t bonding = FALSE;                                     // NO BONDING

       GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof(uint32_t),
                               &passkey);
       GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
       GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
       GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
       GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);
#endif
     }

    // GGS_AddService(GATT_ALL_SERVICES);           //
     //GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT attributes
     //DevInfo_AddService();                        // Device Information Service

    // Start the Device, register characteristic read/write callbacks
    VOID GAPRole_StartDevice(&SimpleBLEPeripheral_gapRoleCBs);

    // Start Bond Manager
    //VOID GAPBondMgr_Register(&simpleBLEPeripheral_BondMgrCBs);

    // Register with GAP for HCI/Host messages
    GAP_RegisterForMsgs(AppEntityID);

}

U8 sNewAdv[31];
/*********************************************************************
Set_Scan_Response_Name

**********************************************************************/
void Set_Scan_Response_Name(I8 * sName)
{
    //static I8 sNewAdv[31];
    extern I8 sScanName[];
    U32 uIdx;

    U32  uNameLen = strlen (sName);

    if (uNameLen > DEVICENAME_PRESERVE_BYTES)
        uNameLen = DEVICENAME_PRESERVE_BYTES;

    sName[uNameLen]=0;

    // complete name
    sNewAdv[0] = uNameLen + 1;                               // size of name + sNewAdv[1]
    sNewAdv[1] = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
    strncpy(&sNewAdv[2], sName, DEVICENAME_PRESERVE_BYTES);

    // connection interval range
    uIdx = uNameLen + 2;
    sNewAdv[uIdx++] = 0x05;                                           // length of this data
    sNewAdv[uIdx++] = GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
    sNewAdv[uIdx++] = LO_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL);   // 100ms
    sNewAdv[uIdx++] = HI_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL);
    sNewAdv[uIdx++] = LO_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL);   // 1s
    sNewAdv[uIdx++] = HI_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL);

    // Tx power level
    sNewAdv[uIdx++] = 0x02;                                             // length of this data
    sNewAdv[uIdx++] = GAP_ADTYPE_POWER_LEVEL;
    sNewAdv[uIdx++] = 0;                                                // 0dBm

    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, uIdx, &sNewAdv[0]);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_taskFxn
 *
 * @brief   Application task entry point for the Simple BLE Peripheral.
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 */

//MJS
typedef struct
{
  uint8_t   cType;
  uint16_t  uEvent_sig;
  uint8_t   uFlag_state;
}  __attribute__((__packed__))  Log_t;

#ifdef TRACK_STATES
    volatile    Log_t   zBLELog[32];
    volatile    U8      cBLELogCtr;
#endif

volatile    U8     uHCICount;
volatile    U16      uGapConnHandle;



#ifndef BLINKY_TEST

/*********************************************************************
 SimpleBLEPeripheral_taskFxn
*********************************************************************/
static void SimpleBLEPeripheral_taskFxn(UArg a0, UArg a1)
{
  extern U32 uLowestWDValue;
  void  BLE_SetupAndStartWatchdog();

  // Initialize application
   SimpleBLEPeripheral_init();

   #ifdef WD_ENABLE
       BLE_SetupAndStartWatchdog();
   #endif

  UART_Log("\n\n**** PWD-BLE Starting ****\n");
  //Log_With_U32("Test Msg 1:", 0xABCD1234);
  //Log_With_U8 ("Test Msg 2:", 0xEF);

  // Application main loop
  for (;;)
  {
    // Waits for a signal to the semaphore associated with the calling thread.
    // Note that the semaphore associated with a thread is signaled when a
    // message is queued to the message receive queue of the thread or when
    // ICall_signal() function is called onto the semaphore.

    #ifdef WD_ENABLE
       HITWD();

        if (uLowestWDValue >= WATCHDOG_SET_VALUE)
            bWatchdogNotRunning = bTRUE;
    #endif

    uAppTaskCounter++;
    ICall_Errno errno = ICall_wait(500);

    // Process Queued Messages
    if (errno == ICALL_ERRNO_SUCCESS)
    {
      ICall_EntityID dest;
      ICall_ServiceEnum src;
      ICall_HciExtEvt *pMsg = NULL;


      if (ICall_fetchServiceMsg(&src, &dest, (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
      {
        uint8 safeToDealloc = TRUE;

        uAppEventCounter++;

        if ((src == ICALL_SERVICE_CLASS_BLE) && (dest == AppEntityID))
        {
          ICall_Stack_Event *pEvt = (ICall_Stack_Event *)pMsg;
          uAppEventCounter++;

          // PROCESS BLE STACK EVENTS vua event flasg
          if (pEvt->signature == 0xffff)
          {
            if (pEvt->event_flag & SBP_CONN_EVT_END_EVT)
            {
                SimpleBLEPeripheral_sendAttRsp();    // Try to retransmit pending ATT Response (if any)
            }
          }


          // PROCESS INTER-TASK EVENTS via Message stack
          else
          {
            safeToDealloc = SimpleBLEPeripheral_processStackMsg((ICall_Hdr *)pMsg);              // Process inter-task message
          }
        }


        if (pMsg && safeToDealloc)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        sbpEvt_t *pMsg = (sbpEvt_t *)Util_dequeueMsg(appMsgQueue);
        uAppEventCounter++;

        if (pMsg)
        {

            // MJS
            switch(pMsg->hdr.event)
            {

                case HCI_READ_RSSI:
                    if (SUCCESS == GAPRole_GetParameter(GAPROLE_CONNHANDLE, (void *)&uGapConnHandle))
                        if (uGapConnHandle != 0xFFFF  && !uHCICount)
                        {
                            HCI_ReadRssiCmd(uGapConnHandle);  // triggers the rssi read - result will come back as event
                            uHCICount++;

                            //HCI_EXT_PacketErrorRateCmd(uGapConnHandle, uint8 command);
                        }

            }

           // END MJS

          // Process message.
          SimpleBLEPeripheral_processAppMsg(pMsg);

          // Free the space from the message.
          ICall_free(pMsg);
        }
      }

    }

    if (events & SBP_PERIODIC_EVT)
    {
      events &= ~SBP_PERIODIC_EVT;
      uAppEventCounter++;

      Util_startClock(&periodicClock);

      // Perform periodic application task
      // Updates the fourth characteristic in the profilw
      SimpleBLEPeripheral_performPeriodicTask();
    }

    // Reset back to bootloader if we get 10 consecutive zeros
    if (bTenConsecutiveZeros)
        HapiResetDevice();



  }
}



/***********************************************************************************************
 LED BLINK TEST CODE
     Change xBLINKY_TEST to BLINKY_TEST in project compiler defines to change app to a
     simple LED blink operation for bootloader testing.
 ***********************************************************************************************/
#else  // BLINKY_TEST

#include <ti\drivers\uart\UARTCC26XX.h>
#include <ti/sysbios/knl/Task.h>
extern UART_Handle hUARTHandle;

#define LED_ON_COLOR      PIN_LED_RED
#define LED_OFF_COLOR     PIN_LED_BLUE

static void SimpleBLEPeripheral_taskFxn(UArg a0, UArg a1)
{

    PINCC26XX_setOutputValue(LED_OFF_COLOR,  0);

    extern U8 SerRxBuf[PWD_SERIAL_RXBUF_SIZE];
    memset(SerRxBuf, 0xFF, 10);

    while(1)
    {   F32 f; U32 u; I32 iLedState=0;

        f=1.000001;
        for (u=0; u<10000; u++) f*= 1.000001;
        PINCC26XX_setOutputValue(LED_ON_COLOR, iLedState);
        iLedState = 1-iLedState;

        for (u=0; u<10000; u++) f*= 1.000001;
        PINCC26XX_setOutputValue(LED_ON_COLOR, iLedState);
        iLedState = 1-iLedState;

        // Reset if we get 10 consecutive zeros
        if (bTenConsecutiveZeros)
            HapiResetDevice();

        HITWD();
    }

}

#endif



// MJS
uint8_t rssi;


/***********************************************************************************************
 HCI_Processor
    Proesses HCI command results once they come in from the GAP task

    SimpleBLEPeripheral_processStackMsg*( calls this function when it sees a
    HCI_GAP_EVENT_EVENT (event) - HCI_COMMAND_COMPLETE_EVENT_CODE (status)  combination
 ***********************************************************************************************/
void HCI_Processor(ICall_Hdr *pMsg)
{
    // Parse Command Complete Event for opcode and status

    hciEvt_CmdComplete_t* command_complete  = (hciEvt_CmdComplete_t*)pMsg;
    uint8_t status                          = command_complete->pReturnParam[0];

    //find which command this command complete is for
    switch (command_complete->cmdOpcode)
    {
        case HCI_READ_RSSI:
        {
            if (status == SUCCESS)
            {
                uint16_t handle = BUILD_UINT16(command_complete->pReturnParam[2], command_complete->pReturnParam[1]);

                if (handle == 0x00)
                    rssi = command_complete->pReturnParam[3];
            }
            break;
        }
    }

}

/*********************************************************************
   SimpleBLEPeripheral_processStackMsg
       Process an incoming stack message.  Called from main task

   pMsg - message to process

   Return:  TRUE if safe to deallocate incoming message, FALSE otherwise.
*********************************************************************/
static uint8_t SimpleBLEPeripheral_processStackMsg(ICall_Hdr *pMsg)
{
  uint8_t safeToDealloc = TRUE;
  void HCI_Processor(ICall_Hdr *pMsg);

  switch (pMsg->event)
  {
  // Process GATT message
    case GATT_MSG_EVENT:
        safeToDealloc = SimpleBLEPeripheral_processGATTMsg((gattMsgEvent_t *)pMsg);
    break;

    // Process HCI command responses
    case HCI_GAP_EVENT_EVENT:
    {
        switch(pMsg->status)
        {
            case HCI_COMMAND_COMPLETE_EVENT_CODE:
                HCI_Processor(pMsg);
            break;

            default:  break;
        }
     }


    default: break;
  }

  return (safeToDealloc);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processGATTMsg
 *
 * @brief   Process GATT messages and events.
 *
 * @return  TRUE if safe to deallocate incoming message, FALSE otherwise.
 */
static uint8_t SimpleBLEPeripheral_processGATTMsg(gattMsgEvent_t *pMsg)
{
  // See if GATT server was unable to transmit an ATT response
  if (pMsg->hdr.status == blePending)
  {
    // No HCI buffer was available. Let's try to retransmit the response
    // on the next connection event.
    if (HCI_EXT_ConnEventNoticeCmd(pMsg->connHandle, AppEntityID,
                                   SBP_CONN_EVT_END_EVT) == SUCCESS)
    {
      // First free any pending response
      SimpleBLEPeripheral_freeAttRsp(FAILURE);

      // Hold on to the response message for retransmission
      pAttRsp = pMsg;

      // Don't free the response message yet
      return (FALSE);
    }
  }
  else if (pMsg->method == ATT_FLOW_CTRL_VIOLATED_EVENT)
  {
    // ATT request-response or indication-confirmation flow control is
    // violated. All subsequent ATT requests or indications will be dropped.
    // The app is informed in case it wants to drop the connection.

    // Display the opcode of the message that caused the violation.
    Display_print1(dispHandle, 5, 0, "FC Violated: %d", pMsg->msg.flowCtrlEvt.opcode);
  }
  else if (pMsg->method == ATT_MTU_UPDATED_EVENT)
  {
    // MTU size updated
    Display_print1(dispHandle, 5, 0, "MTU Size: $d", pMsg->msg.mtuEvt.MTU);
    uATT_MTU = pMsg->msg.mtuEvt.MTU;
  }

  // Free message payload. Needed only for ATT Protocol messages
  GATT_bm_free(&pMsg->msg, pMsg->method);

  // It's safe to free the incoming message
  return (TRUE);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_sendAttRsp
 *
 * @brief   Send a pending ATT response message.
 *
 * @param   none
 *
 * @return  none
 */
static void SimpleBLEPeripheral_sendAttRsp(void)
{
  // See if there's a pending ATT Response to be transmitted
  if (pAttRsp != NULL)
  {
    uint8_t status;

    // Increment retransmission count
    rspTxRetry++;

    // Try to retransmit ATT response till either we're successful or
    // the ATT Client times out (after 30s) and drops the connection.
    status = GATT_SendRsp(pAttRsp->connHandle, pAttRsp->method, &(pAttRsp->msg));
    if ((status != blePending) && (status != MSG_BUFFER_NOT_AVAIL))
    {
      // Disable connection event end notice
      HCI_EXT_ConnEventNoticeCmd(pAttRsp->connHandle, AppEntityID, 0);

      // We're done with the response message
      SimpleBLEPeripheral_freeAttRsp(status);
    }
    else
    {
      // Continue retrying
      Display_print1(dispHandle, 5, 0, "Rsp send retry: %d", rspTxRetry);
    }
  }
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_freeAttRsp
 *
 * @brief   Free ATT response message.
 *
 * @param   status - response transmit status
 *
 * @return  none
 */
static void SimpleBLEPeripheral_freeAttRsp(uint8_t status)
{
  // See if there's a pending ATT response message
  if (pAttRsp != NULL)
  {
    // See if the response was sent out successfully
    if (status == SUCCESS)
    {
      Display_print1(dispHandle, 5, 0, "Rsp sent retry: %d", rspTxRetry);
    }
    else
    {
      // Free response payload
      GATT_bm_free(&pAttRsp->msg, pAttRsp->method);

      Display_print1(dispHandle, 5, 0, "Rsp retry failed: %d", rspTxRetry);
    }

    // Free response message
    ICall_freeMsg(pAttRsp);

    // Reset our globals
    pAttRsp = NULL;
    rspTxRetry = 0;
  }
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */

// MJS - history
volatile unsigned char cStateLog[32], cStateLogCtr;

static void SimpleBLEPeripheral_processAppMsg(sbpEvt_t *pMsg)
{
  // MJS - keep history
  cStateLog[cStateLogCtr++] = pMsg-> hdr.state;
  cStateLogCtr = (++cStateLogCtr) & 0x1f;

  switch (pMsg->hdr.event)
  {
    case SBP_STATE_CHANGE_EVT:
      SimpleBLEPeripheral_processStateChangeEvt((gaprole_States_t)pMsg->
                                                hdr.state);
      break;

    case SBP_CHAR_CHANGE_EVT:
      SimpleBLEPeripheral_processCharValueChangeEvt(pMsg->hdr.state);
      break;

    default:
      // Do nothing.
      break;
  }
}



/*********************************************************************
 * @fn      SimpleBLEPeripheral_stateChangeCB
 *
 * @brief   Callback from GAP Role indicating a role state change.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_stateChangeCB(gaprole_States_t newState)
{
  SimpleBLEPeripheral_enqueueMsg(SBP_STATE_CHANGE_EVT, newState);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processStateChangeEvt
 *
 * @brief   Process a pending GAP Role state change event.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_processStateChangeEvt(gaprole_States_t newState)
{
#ifdef PLUS_BROADCASTER
  static bool firstConnFlag = false;
#endif // PLUS_BROADCASTER

  PWD_New_Gap_State(newState);  // PWD handling for new GAP state

  switch ( newState )
  {
    case GAPROLE_STARTED:
      {

#if 0
        uint8_t ownAddress[B_ADDR_LEN];
        uint8_t systemId[DEVINFO_SYSTEM_ID_LEN];

        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

        // use 6 bytes of device address for 8 bytes of system ID value
        systemId[0] = ownAddress[0];
        systemId[1] = ownAddress[1];
        systemId[2] = ownAddress[2];

        // set middle bytes to zero
        systemId[4] = 0x00;
        systemId[3] = 0x00;

        // shift three bytes up
        systemId[7] = ownAddress[5];
        systemId[6] = ownAddress[4];
        systemId[5] = ownAddress[3];

        DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);

        // Display device address
        Display_print0(dispHandle, 1, 0, Util_convertBdAddr2Str(ownAddress));
#endif

        Display_print0(dispHandle, 2, 0, "Initialized");
      }
      break;

    case GAPROLE_ADVERTISING:
      Display_print0(dispHandle, 2, 0, "Advertising");
      break;

#ifdef PLUS_BROADCASTER
    /* After a connection is dropped a device in PLUS_BROADCASTER will continue
     * sending non-connectable advertisements and shall sending this change of
     * state to the application.  These are then disabled here so that sending
     * connectable advertisements can resume.
     */
    case GAPROLE_ADVERTISING_NONCONN:
      {
        uint8_t advertEnabled = FALSE;

        // Disable non-connectable advertising.
        GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                           &advertEnabled);

        advertEnabled = TRUE;

        // Enabled connectable advertising.
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                             &advertEnabled);

        // Reset flag for next connection.
        firstConnFlag = false;

        SimpleBLEPeripheral_freeAttRsp(bleNotConnected);
      }
      break;
#endif //PLUS_BROADCASTER

    case GAPROLE_CONNECTED:
      {
        linkDBInfo_t linkInfo;
        uint8_t numActive = 0;

        Util_startClock(&periodicClock);

        numActive = linkDB_NumActive();

        // Use numActive to determine the connection handle of the last
        // connection
        if ( linkDB_GetInfo( numActive - 1, &linkInfo ) == SUCCESS )
        {
          Display_print1(dispHandle, 2, 0, "Num Conns: %d", (uint16_t)numActive);
          Display_print0(dispHandle, 3, 0, Util_convertBdAddr2Str(linkInfo.addr));
        }
        else
        {
          uint8_t peerAddress[B_ADDR_LEN];

          GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, peerAddress);

          Display_print0(dispHandle, 2, 0, "Connected");
          Display_print0(dispHandle, 3, 0, Util_convertBdAddr2Str(peerAddress));
        }

        #ifdef PLUS_BROADCASTER
          // Only turn advertising on for this state when we first connect
          // otherwise, when we go from connected_advertising back to this state
          // we will be turning advertising back on.
          if (firstConnFlag == false)
          {
            uint8_t advertEnabled = FALSE; // Turn on Advertising

            // Disable connectable advertising.
            GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                                 &advertEnabled);

            // Set to true for non-connectabel advertising.
            advertEnabled = TRUE;

            // Enable non-connectable advertising.
            GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                                 &advertEnabled);
            firstConnFlag = true;
          }
        #endif // PLUS_BROADCASTER
      }
      break;

    case GAPROLE_CONNECTED_ADV:
      Display_print0(dispHandle, 2, 0, "Connected Advertising");
      break;

    case GAPROLE_WAITING:
      Util_stopClock(&periodicClock);
      SimpleBLEPeripheral_freeAttRsp(bleNotConnected);

      Display_print0(dispHandle, 2, 0, "Disconnected");

      // Clear remaining lines
      Display_clearLines(dispHandle, 3, 5);
      break;

    case GAPROLE_WAITING_AFTER_TIMEOUT:
      SimpleBLEPeripheral_freeAttRsp(bleNotConnected);

      Display_print0(dispHandle, 2, 0, "Timed Out");

      // Clear remaining lines
      Display_clearLines(dispHandle, 3, 5);

      #ifdef PLUS_BROADCASTER
        // Reset flag for next connection.
        firstConnFlag = false;
      #endif //#ifdef (PLUS_BROADCASTER)
      break;

    case GAPROLE_ERROR:
      Display_print0(dispHandle, 2, 0, "Error");
      break;

    default:
      Display_clearLine(dispHandle, 2);
      break;
  }

  // Update the state
  //gapProfileState = newState;
}
#if !defined FEATURE_OAD_ONCHIP  && !defined PHYN_PWD
/*********************************************************************
 * @fn      SimpleBLEPeripheral_charValueChangeCB
 *
 * @brief   Callback from Simple Profile indicating a characteristic
 *          value change.
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_charValueChangeCB(uint8_t paramID)
{
  SimpleBLEPeripheral_enqueueMsg(SBP_CHAR_CHANGE_EVT, paramID);
}
#endif //!FEATURE_OAD_ONCHIP

/*********************************************************************
 * @fn      SimpleBLEPeripheral_processCharValueChangeEvt
 *
 * @brief   Process a pending Simple Profile characteristic value change
 *          event.
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_processCharValueChangeEvt(uint8_t paramID)
{
#if !defined FEATURE_OAD_ONCHIP  && !defined PHYN_PWD
  uint8_t newValue;

  switch(paramID)
  {
    case SIMPLEPROFILE_CHAR1:
      SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR1, &newValue);

      Display_print1(dispHandle, 4, 0, "Char 1: %d", (uint16_t)newValue);
      break;

    case SIMPLEPROFILE_CHAR3:
      SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR3, &newValue);

      Display_print1(dispHandle, 4, 0, "Char 3: %d", (uint16_t)newValue);
      break;

    default:
      // should not reach here!
      break;
  }
#endif //!FEATURE_OAD_ONCHIP
}

I8 sAppUpdateMsg[SIZEOF_APP_UPDATE_MSG+1];
/*********************************************************************
 SimpleBLEPeripheral_performPeriodicTask
   Perform a periodic application task. This function gets called
   every SBP_PERIODIC_EVT_PERIOD).
**********************************************************************/
void SimpleBLEPeripheral_performPeriodicTask(void)
{
    extern char sMsgFromApp[];
    extern U8 *NewAppFieldPtr;
    extern B32 bNewAppField;

    // New message from APP for MT
    if (bNewAppMsg)
    {
        UART_Unsolicited_Tx(BLE_MESSAGE_CODE_COMMAND, sMsgFromApp);
        bNewAppMsg = bFALSE;
    }

    // New message from APP for MT
    if (bNewAppField)
    {
        UART_Unsolicited_Tx(BLE_MESSAGE_CODE_FIELD_UPDATE, sAppUpdateMsg);
        bNewAppField = bFALSE;
    }

}

/*********************************************************************
Heartbeat_Service
*********************************************************************/
static void Heartbeat_Service(UArg arg)
{
    static U32      iLedState = 0;
    static B32      bClearOnDisc = bFALSE;

    extern ICall_Semaphore  UARTsem;
    extern gaprole_States_t eGapState;
    extern B32              bHeartbeatEnable;

    void Sys_Checks();

    if (bHeartbeatEnable)
    {
        // Connected Heartbeat
        if (GAPROLE_CONNECTED == eGapState || GAPROLE_CONNECTED_ADV == eGapState)
        {
             PINCC26XX_setOutputValue(PIN_LED_BLUE, iLedState);
             iLedState = 1-iLedState;
             PINCC26XX_setOutputValue(PIN_LED_RED,  iLedState);
             bClearOnDisc = bTRUE;

        }


        // Disconnected Heartbeat
        else
        {
             PINCC26XX_setOutputValue(PIN_LED_BLUE, iLedState);
             iLedState = 1-iLedState;
             PINCC26XX_setOutputValue(PIN_LED_RED,  LED_OFF);

             // Clear the Remote device info fields on a disconnect
             if (bClearOnDisc)
             {
                 bClearOnDisc = bFALSE;
                 InitAppFields();
             }
        }


    }


    Sys_Checks();
    Semaphore_post(UARTsem);
    Util_restartClock(&zHeartbeatClock, 500);
}


/*********************************************************************
Sys_Checks
*********************************************************************/
void Sys_Checks()
{
    I8 sMsg[64];
    static B32 bAlert = bFALSE;
    static U32 SysCheckCounter;

    extern F32 fGapStackUsedPct, fUartStackUsedPct, fAppStackUsedPct;
    extern F32  fStackUsedPct;

    static U32 uLastAppCounter, uLastUartCounter;

    static U32 uMsgCount;                           // To ensure no message runaway
    static B32 bSend = bFALSE;
    static B32 bGapStack90Pct = bFALSE, bAppStack90Pct = bFALSE, bUartStack90Pct = bFALSE;

    void BLE_StackCheck();

    // Check Task Counters
    // App and UART tasks always run so their counters should always be incrementing

    if (!(++SysCheckCounter & 0x3))          // every 8th call
    {
        if (uAppTaskCounter  == uLastAppCounter)   bAlert = bTRUE;
        if (uUartTaskCounter == uLastUartCounter)  bAlert = bTRUE;

        if (bAlert)
            HapiResetDevice();

        uLastAppCounter = uAppTaskCounter;
        uLastUartCounter = uUartTaskCounter;
    }

    // Stack monitoring
    BLE_StackCheck();

    // Report System Stack High Usage
    if (fStackUsedPct > 75.0 && !bStack75Pct)
    {
        sprintf(sMsg,"00,99,WARNING -- System Stack Usage at %6.2f", fStackUsedPct);
        bStack75Pct = bTRUE;
        bSend = bTRUE;
    }

    if (fStackUsedPct > 90.0 && !bStack90Pct)
    {
        sprintf(sMsg,"00,99,WARNING -- System  Stack Usage at %6.2f", fStackUsedPct);
        bStack90Pct = bTRUE;
        bSend = bTRUE;
    }

    if (fStackUsedPct > 95.0 && !bStack95Pct)
    {
        sprintf(sMsg,"00,99,WARNING -- System Stack Usage at %6.2f", fStackUsedPct);
        bStack95Pct = bTRUE;
        bSend = bTRUE;
    }

    if (fStackUsedPct > 99.9 && !bStack100Pct)
    {
        sprintf(sMsg,"00,99, SYSTEM UNSTABLE -- System  Stack Usage At 100%%");
        bStack100Pct = bTRUE;
        bSend = bTRUE;
    }


    // Report Task Stack High Usage
    if (fGapStackUsedPct > 90.0 && !bGapStack90Pct && !bSend)
    {
        sprintf(sMsg,"00,99,WARNING -- Gap Stack Usage at %6.2f", fGapStackUsedPct);
        bGapStack90Pct = bTRUE;
        bSend = bTRUE;
    }

    if (fAppStackUsedPct > 90.0 && !bAppStack90Pct && !bSend)
    {
        sprintf(sMsg,"00,99,WARNING -- App Stack Usage at %6.2f", fAppStackUsedPct);
        bAppStack90Pct = bTRUE;
        bSend = bTRUE;
    }

    if (fUartStackUsedPct > 90.0 && !bUartStack90Pct && !bSend)
    {
        sprintf(sMsg,"00,99,WARNING -- Uart Stack Usage at %6.2f", fUartStackUsedPct);
        bUartStack90Pct = bTRUE;
        bSend = bTRUE;
    }

    if (bSend)
    {
        if (++uMsgCount < 10)
            UART_Naked_Tx(sMsg);

        bSend = bFALSE;
    }

}


/*********************************************************************
 * @fn      SimpleBLEPeripheral_clockHandler
 *
 * @brief   Handler function for clock timeouts.
 *
 * @param   arg - event type
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_clockHandler(UArg arg)
{
  // Store the event.
  events |= arg;

  // Wake up the application.
  Semaphore_post(AppSem);
}

/*********************************************************************
 * @fn      SimpleBLEPeripheral_enqueueMsg
 *
 * @brief   Creates a message FOR THE APP and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 *
 * @return  None.
 */
static void SimpleBLEPeripheral_enqueueMsg(uint8_t event, uint8_t state)
{
  sbpEvt_t *pMsg;

  // Create dynamic pointer to message.
  if ((pMsg = ICall_malloc(sizeof(sbpEvt_t))))
  {
    pMsg->hdr.event = event;
    pMsg->hdr.state = state;

    // Enqueue the message.
    Util_enqueueMsg(appMsgQueue, AppSem, (uint8*)pMsg);
  }
}

/*********************************************************************
 * UNUSED/TEST code
*********************************************************************/
#if 0


/*********************************************************************
Uart_Testing
*********************************************************************/
static void Uart_Testing(UArg arg)
{
    static U8 sKeepAlive[] = "Test String";

    if (UART_Write_Is_Finished())
        Start_UART_Write(sKeepAlive, sizeof(sKeepAlive));

    Util_restartClock(&zUartTestClock, 100);
}

void SysTickIsr (void)
{
    ullSysTick++;   // 64-bit good for 584 Million years until rollver at 1 ms
}


uint64_t GetSysMs()
{
    volatile uint64_t a,b;

    a = ullSysTick;
    b = ullSysTick;

    if (a==b)
        return a;
    else
        return ullSysTick;
}


void DelayAndBlockMs(uint64_t uMilliSec)
{
    volatile uint64_t t0;

    t0 = GetSysMs();

    // 64-bit good for 584 Million years until rollver at 1 ms
    while (GetSysMs() < (t0 + uMilliSec) )
        HITWD();

}


// MJS
  #ifdef TRACK_STATES
      zBLELog[cBLELogCtr].uFlag_state = pEvt->event_flag;
      zBLELog[cBLELogCtr].uEvent_sig =  pEvt->signature;
      zBLELog[cBLELogCtr].cType =  'S';
      cBLELogCtr = (++cBLELogCtr & 0x1F);
  #endif

      // MJS
                  #ifdef TRACK_STATES
                      zBLELog[cBLELogCtr].uFlag_state = pEvt->event_flag;
                      zBLELog[cBLELogCtr].uEvent_sig =  pEvt->signature;
                      zBLELog[cBLELogCtr].cType =  'E';
                      cBLELogCtr = (++cBLELogCtr & 0x1F);
                  #endif

#ifdef TRACK_STATES
     zBLELog[cBLELogCtr].uFlag_state = pMsg->hdr.state;
     zBLELog[cBLELogCtr].uEvent_sig =  (uint16_t)pMsg->hdr.event;
     zBLELog[cBLELogCtr].cType =  'A';
     cBLELogCtr = (++cBLELogCtr & 0x1F);
 #endif


#if 0
  GPTimerCC26XX_Params_init (&Timer_A_Params); // Set up GPTimer 0A in edge time capture mode
  GPTimerCC26XX_Handle hTimer = GPTimerCC26XX_open (PWD_GPTIMER0A, &Timer_A_Params);
  GPTimerCC26XX_registerInterrupt(hTimer, GPTick, GPT_INT_MATCH);
  GPTimerCC26XX_setMatchValue(hTimer, 0xFFFF);

  uTimer_Value = GPTimerCC26XX_getFreeRunValue(hTimer);
  GPTimerCC26XX_start(hTimer);
#endif


  /*
      SysTickDisable();
      SysTickPeriodSet(24000);                   // 1 Ms interrupts
      SysTickIntRegister(SysTickIsr);
      SysTickIntEnable();
      SysTickEnable();
      uSysTickIsrClocks = SysTickPeriodGet();   //  number of processor clocks between interrupts
  */
  /*
      uint32_t i;

      for (i=0; i<6; i++)
      {

          PINCC26XX_setOutputValue(PIN_LED_BLUE, i & 1);
          PINCC26XX_setOutputValue(PIN_LED_RED, (~i) & 1 );

          DelayAndBlockMs(250);
      }
      PINCC26XX_setOutputValue(PIN_LED_BLUE, LED_OFF);
      PINCC26XX_setOutputValue(PIN_LED_RED,  LED_OFF);
  */


      /*
      uTimer_Value = GPTimerCC26XX_getFreeRunValue(hTimer);

      while (uTimer_Value < 0xF000)
          uTimer_Value = GPTimerCC26XX_getFreeRunValue(hTimer);

      GPTimerCC26XX_stop(hTimer);
    */


  // service UUID, to notify central devices what services are included
  // in this peripheral
#if !defined(FEATURE_OAD) || defined(FEATURE_OAD_ONCHIP)
  0x03,   // length of this data
#else //OAD for external flash
  0x05,  // lenght of this data
#endif //FEATURE_OAD
  GAP_ADTYPE_16BIT_MORE,      // some of the UUID's, but not all
#ifdef FEATURE_OAD
  LO_UINT16(OAD_SERVICE_UUID),
  HI_UINT16(OAD_SERVICE_UUID),
#endif //FEATURE_OAD
#ifndef FEATURE_OAD_ONCHIP
  LO_UINT16(SIMPLEPROFILE_SERV_UUID),
  HI_UINT16(SIMPLEPROFILE_SERV_UUID)
#endif //FEATURE_OAD_ONCHIP

#if 0
  /*********************************************************************
   GAP Setup
  *********************************************************************/
  GAP_SetParamValue(TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL);

  // Setup the GAP Peripheral Role Profile
  {
    // For all hardware platforms, device starts advertising upon initialization
    uint8_t initialAdvertEnable = TRUE;
    extern U16 uProgCRC;

    // By setting this to zero, the device will go into the waiting state after
    // being discoverable for 30.72 second, and will not being advertising again
    // until the enabler is set back to TRUE
    uint16_t advertOffTime = 0;

    uint8_t enableUpdateRequest = DEFAULT_ENABLE_UPDATE_REQUEST;
    uint16_t desiredMinInterval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    uint16_t desiredMaxInterval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    uint16_t desiredSlaveLatency = DEFAULT_DESIRED_SLAVE_LATENCY;
    uint16_t desiredConnTimeout = DEFAULT_DESIRED_CONN_TIMEOUT;

    // Set the GAP Role Parameters

    // Advertising Setup
    //GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),     &initialAdvertEnable);
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, 1,   &TRUE_U8);


    //GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof(scanRspData), scanRspData);
    Phyn_Set_Scan_Response_Name("Powered by Phyn");

    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData),  advertData);

    GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8_t),  &enableUpdateRequest);
    GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16_t),   &desiredMinInterval);
    GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16_t),   &desiredMaxInterval);
    GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16_t),       &desiredSlaveLatency);
    GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16_t),  &desiredConnTimeout);
  }

  // Set the GAP Characteristics
  // GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, strlen(attDeviceName), attDeviceName);


  // Set advertising interval
  {
    uint16_t advInt = DEFAULT_ADVERTISING_INTERVAL;

    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, advInt);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, advInt);
  }

  // Setup the GAP Bond Manager
  {
    uint32_t passkey = 0; // passkey "000000"
    uint8_t pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
    uint8_t mitm = TRUE;
    uint8_t ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    uint8_t bonding = FALSE;

    GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof(uint32_t),
                            &passkey);
    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);          // NO BONDING

  }


  GGS_AddService(GATT_ALL_SERVICES);           // GAP
  GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT attributes
 // DevInfo_AddService();                        // Device Information Service
#endif




#endif
