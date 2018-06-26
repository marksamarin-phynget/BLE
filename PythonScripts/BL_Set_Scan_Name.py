#!/usr/bin/env python
#********************************************************************************************************					
# BL_SetScanName.py
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

	ser = serial.Serial(port=SerPort, baudrate=115200, timeout=0)
	print (ser.portstr)       # check which port was really used
	return ser

	
if len(sys.argv) != 3:
	print('\tUsage:  Python BL_SetScanName.py <Port> <Name>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_SetScanName.py COM7 "Powered By Phyn"\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
SP = App_Msg.OpenComPort(sys.argv[1])


#*********************************************************************************
# Send message, report results
#*********************************************************************************
CMReadback = App_Msg.SetScanRespMsg(SP, sys.argv[2]).decode('utf-8')
print(CMReadback)
