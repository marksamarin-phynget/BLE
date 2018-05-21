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

#ifndef CONFIG_H
#define CONFIG_H

#include <xdc/std.h>

/*
This file defines data types and variables for the application configuration.
 */

/*
RF basic settings as found in the typical settings section of Smart RF Studio.
Each one defines a test case in this application.
*/
typedef enum
{
    RfSetup_Fsk = 0, // frequency shift keying
    RfSetup_Lrm,     // long range mode
    RfSetup_Ook,     // on off keying
    RfSetup_Hsm,     // high speed mode
    NrOfRfSetups
} RfSetup;

/* Whether the application works as sender or receiver */
typedef enum
{
    OperationMode_Rx = 0,
    OperationMode_Tx,
    NrOfOperationModes
} OperationMode;

/* Contains a pre-defined setting for frequency selection */
typedef struct
{
    const char* const label;
    const uint16_t frequency;
    const uint16_t fractFreq;
} FrequencyTableEntry;

/*
Holds the application config that is prepared in the menu and
used in the rx and tx functions.
*/
typedef struct
{
    RfSetup rfSetup;             // test case index
    OperationMode operationMode; // tx/rx mode index
    uint32_t packetCount;        // desired packet count
    uint8_t frequency;           // index in config_frequencyTable
} ApplicationConfig;

extern const FrequencyTableEntry config_frequencyTable[];   // lookup table for frequency settings
extern const char* const config_opmodeLabels[];             // loopup table for operation mode labels
extern const uint32_t config_packetCountTable[];            // lookup table for different packet count options
extern const char* const config_rfSetupLabels[];            // lookup table for RfSetup labels

extern const uint8_t config_NrOfPacketCounts;               // total amount of packet count options
extern const uint8_t config_NrOfFrequencies;                // total amount of frequency table entries

#endif
