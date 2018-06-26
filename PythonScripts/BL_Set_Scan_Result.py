#!/usr/bin/env python
#********************************************************************************************************					
# BL_Set_Scan_Result.py
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
	print('\tUsage:  Python BL_Set_Scan_Result.py <Port> <Msg>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_Set_Scan_Result.py COM7 10.5.5.222\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
SP = App_Msg.OpenComPort(sys.argv[1])


#*********************************************************************************
# Send message, report results
#*********************************************************************************
CMReadback = App_Msg.SetScanResult(SP, sys.argv[2]).decode('utf-8')
