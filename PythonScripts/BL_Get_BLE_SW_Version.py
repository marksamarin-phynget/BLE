#!/usr/bin/env python
#********************************************************************************************************					
# BL_Get_BLE_SW_Version.py
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
# START	
#*********************************************************************************
if len(sys.argv) != 2:
	print('\tUsage:  Python BL_Get_BLE_SW_Version.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_Get_BLE_SW_Version.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
SP = App_Msg.OpenComPort(sys.argv[1])

#*********************************************************************************
# GET 
#*********************************************************************************
print("\nGet BLE SW Version\n----------------------------------")
App_Msg.GetBLESWVersion(SP)


#*********************************************************************************
# Wrap Up
#*********************************************************************************
SP.close()
sys.exit(1)

 
    
 
 
 