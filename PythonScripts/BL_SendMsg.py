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

	
if len(sys.argv) != 3:
	print('\tUsage:  Python BL_SendMsg.py <Port> <Msg>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_SendMsg.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
bConnected = False
SP = App_Msg.OpenComPort(sys.argv[1])

# Get current scan msg
TxMessage = "81,60," + sys.argv[2]
CMReadback = App_Msg.ClientMessage(SP, TxMessage).decode('utf-8')
print(CMReadback)
