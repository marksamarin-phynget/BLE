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
TestFail = False	

print('\n\nBLE_Regression_Test\n-------------------\n')
	
if len(sys.argv) != 2:
	print('\tUsage:  Python BLE_Get_Stats.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BLE_Get_Stats.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
bConnected = False
SP = App_Msg.OpenComPort(sys.argv[1])

#*********************************************************************************
# GET Stack Info
#*********************************************************************************
print("\nSTAT RETRIEVAL")
App_Msg.GetStackInfo(SP)
App_Msg.GetWdInfo(SP)
App_Msg.GetCounters(SP)


#*********************************************************************************
# Wrap Up
#*********************************************************************************
SP.close()
sys.exit(1)

 
    
 
 
 