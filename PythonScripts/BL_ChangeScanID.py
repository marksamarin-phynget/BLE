#!/usr/bin/env python
#********************************************************************************************************					
# BLE_Regression_Test.py
# 	
#********************************************************************************************************
import sys
import time
import signal
from time import sleep

import serial
import App_Msg
import datetime

#*********************************************************************************
# Function: OpenComPort
#*********************************************************************************
def OpenComPort(SerPort):

	ser = serial.Serial(port=SerPort, baudrate=115200, timeout=0)  # open first serial port with 9600,8,N,1
	print (ser.portstr)       # check which port was really used
	return ser

#*********************************************************************************
# TEST START	
#*********************************************************************************
print('\n\nBLE_ChangeScanID\n-------------------\n')
	
if len(sys.argv) != 3:
	print('\tUsage:  Python BLE_ChangeScanID.py <Port> <ID>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.')
	print('\t       <ID>   is 1-20 chars (truncated)\n')
	print('\tex:    Python BLE_ChangeScanID.py /dev/ttyUSB1\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
bConnected = False
SP = App_Msg.OpenComPort(sys.argv[1])
NewID = sys.argv[2]

#*********************************************************************************
# CHANGE Scan ID
#*********************************************************************************
print("\nCHANGING Scan ID Message to: " + NewID + "\n")

# Get current scan msg
OrigScanRespMsg = App_Msg.GetScanRespMsg(SP)

# Set to a test ID setting and hold for 10 sec
App_Msg.SetScanRespMsg(SP, NewID)

# Readback new ID
TestScanMsgReadback = App_Msg.GetScanRespMsg(SP).decode('utf-8')

IDLen = len(NewID)

if NewID != TestScanMsgReadback[6:IDLen + 6]:
	print('\n\t\t\t\t********* Set Scan response ID Fail\n')
else:
	print('\nDevice Scan ID is now: ' + NewID + '\n')