/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/***** Includes *****/

/* XDCtools Header files */
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* TI-RTOS Header files */
#include <ti/drivers/rf/RF.h>
#include <ti/drivers/PIN.h>

/* Board Header files */
#include "Board.h"

/* Application specific Header files */
#include "menu.h"
#include "RFQueue.h"
#include "rf_application_settings.h"

/***** Defines *****/
#define DATA_ENTRY_HEADER_SIZE  8   /* Constant header size of a Generic Data Entry */
#define MAX_LENGTH              125 /* Max length byte the radio will accept */
#define NUM_DATA_ENTRIES        2   /* NOTE: Only two data entries supported at the moment */
#define NUM_APPENDED_BYTES      2   /* RF_cmdRxHS.rxConf.bIncludeLen = 1: Include the received length
                                    * field (2 bytes) in the stored packet  */


/***** Prototypes *****/
static void rx_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);


/***** Variable declarations *****/
static uint8_t packetReceived = false;
static uint16_t* crcOk;
static int8_t* rssi;

static RF_Object rfObject;
static RF_Handle rfHandle;
static RF_CmdHandle rxCmdHndl = NULL; /* Handle needed to abot the RX command */

/* Buffer which contains all Data Entries for receiving data.
 * Pragmas are needed to make sure this buffer is 4 byte aligned (requirement from the RF Core) */
#if defined(__TI_COMPILER_VERSION__)
    #pragma DATA_ALIGN (rxDataEntryBuffer, 4);
        static uint8_t rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
                                                                 MAX_LENGTH,
                                                                 NUM_APPENDED_BYTES)];
#elif defined(__IAR_SYSTEMS_ICC__)
    #pragma data_alignment = 4
        static uint8_t rxDataEntryBuffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
                                                                 MAX_LENGTH,
                                                                 NUM_APPENDED_BYTES)];
#elif defined(__GNUC__)
        static uint8_t rxDataEntryBuffer [RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(NUM_DATA_ENTRIES,
            MAX_LENGTH, NUM_APPENDED_BYTES)] __attribute__ ((aligned (4)));
#else
    #error This compiler is not supported.
#endif


static dataQueue_t dataQueue;                    /* Receive queue for the RF Code to fill in data */
rfc_dataEntryGeneral_t* currentDataEntry;        /* General data entry structure (type = 0) */
rfc_hsRxOutput_t rxStatistics_hs;                /* Output structure for CMD_HS_RX */
rfc_propRxOutput_t rxStatistics_prop;            /* Output structure for CMD_PROP_RX */


/*
Runs the receiving part of the test application and returns a result.
 */
TestResult rx_runRxTest(const ApplicationConfig* config)
{
    Assert_isTrue(config != NULL, NULL);

    RF_Params rfParams;
    RF_Params_init(&rfParams);

    if( RFQueue_defineQueue(&dataQueue,
                            rxDataEntryBuffer,
                            sizeof(rxDataEntryBuffer),
                            NUM_DATA_ENTRIES,
                            MAX_LENGTH + NUM_APPENDED_BYTES))
    {
        /* Failed to allocate space for all data entries */
        while(true);
    }

    RF_cmdPropRx.pQueue = RF_cmdRxHS.pQueue = &dataQueue;
    RF_cmdRxHS.pOutput = &rxStatistics_hs;
    RF_cmdPropRx.pOutput = (uint8_t*)&rxStatistics_prop;
    RF_cmdPropRx.maxPktLen = RF_cmdRxHS.maxPktLen = MAX_LENGTH;
    RF_cmdPropRx.pktConf.bRepeatOk = RF_cmdRxHS.pktConf.bRepeatOk = 1;
    RF_cmdPropRx.pktConf.bRepeatNok = RF_cmdRxHS.pktConf.bRepeatNok = 1;
    RF_cmdPropRx.rxConf.bAutoFlushCrcErr = RF_cmdRxHS.rxConf.bAutoFlushCrcErr = 1;
    RF_cmdPropRx.rxConf.bAutoFlushIgnored = 1;

    /* Request access to the radio based on test case */
    switch (config->rfSetup)
    {
        case RfSetup_Fsk:

            RF_cmdPropRadioDivSetup_fsk.centerFreq = config_frequencyTable[config->frequency].frequency;
            rfHandle = RF_open(&rfObject, &RF_prop_fsk, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup_fsk, &rfParams);
            break;

        case RfSetup_Lrm:

            RF_cmdPropRadioDivSetup_lrm.centerFreq = config_frequencyTable[config->frequency].frequency;
            rfHandle = RF_open(&rfObject, &RF_prop_lrm, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup_lrm, &rfParams);
            break;

        case RfSetup_Ook:

            RF_cmdPropRadioDivSetup_ook.centerFreq = config_frequencyTable[config->frequency].frequency;
            rfHandle = RF_open(&rfObject, &RF_prop_ook, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup_ook, &rfParams);
            break;

        case RfSetup_Hsm:

            rfHandle = RF_open(&rfObject, &RF_prop_hsm, (RF_RadioSetup*)&RF_cmdRadioSetup_hsm, &rfParams);
            break;

        default:
            break;
    }

    /* Set the frequency */
    RF_cmdFs.frequency = config_frequencyTable[config->frequency].frequency;
    RF_cmdFs.fractFreq = config_frequencyTable[config->frequency].fractFreq;
    RF_runCmd(rfHandle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);

    /* Enter RX mode and stay forever in RX */
    switch (config->rfSetup)
    {
        case RfSetup_Hsm:

            rxCmdHndl = RF_postCmd(rfHandle, (RF_Op*)&RF_cmdRxHS, RF_PriorityNormal, &rx_callback, RF_EventRxEntryDone);
            crcOk = &rxStatistics_hs.nRxOk;
            rssi = &rxStatistics_hs.lastRssi;
            break;

        default:

            rxCmdHndl = RF_postCmd(rfHandle, (RF_Op*)&RF_cmdPropRx, RF_PriorityNormal, &rx_callback, RF_EventRxEntryDone);
            crcOk = &rxStatistics_prop.nRxOk;
            rssi = &rxStatistics_prop.lastRssi;
            break;
    }

    *crcOk = 0;
    *rssi = 0;
    while(true)
    {
        if(packetReceived)
        {
            packetReceived = false;
            menu_updateRxScreen(*crcOk, *rssi);
        }

        if (menu_isButtonPressed())
        {
            /* force abort gracefully */
            RF_cancelCmd(rfHandle, rxCmdHndl, 0);
            RF_pendCmd(rfHandle, rxCmdHndl, RF_EventRxEntryDone);
            RF_close(rfHandle);
            return TestResult_Aborted;
        }
    }
}

void rx_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    if (e & RF_EventRxEntryDone)
    {
        /* Get current unhandled data entry */
        currentDataEntry = RFQueue_getDataEntry();

        /* For TC_HSM, the packet length and a pointer to the first byte in the payload can be found as follows:
         *
         * uint8_t packetLength      = ((*(uint8_t*)(&currentDataEntry->data + 1)) << 8) | (*(uint8_t*)(&currentDataEntry->data));
         * uint8_t* packetDataPointer = (uint8_t*)(&currentDataEntry->data + 2);
         *
         * For the other test cases (TC_LRM, TC_OOK and TC_FSK), the packet length and first payload byte is found here:
         *
         * uint8_t packetLength      = *(uint8_t*)(&currentDataEntry->data);
         * uint8_t* packetDataPointer = (uint8_t*)(&currentDataEntry->data + 1);
         */
        packetReceived = true;
        RFQueue_nextEntry();
    }
}
