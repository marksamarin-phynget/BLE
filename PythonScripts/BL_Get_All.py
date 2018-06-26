#!/usr/bin/env python
#********************************************************************************************************					
# BL_Get_All.py
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
def OpenComPort(SP):

	ser = serial.Serial(port=SP, baudrate=115200, timeout=0)  # open first serial port with 9600,8,N,1
	print (ser.portstr)       # check which port was really used
	return ser

#*********************************************************************************
# START	
#*********************************************************************************
if len(sys.argv) != 2:
	print('\tUsage:  python BL_Get_All.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    python BL_Get_All.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
SP = App_Msg.OpenComPort(sys.argv[1])

#*********************************************************************************
# GET 
#*********************************************************************************
print("\nGet ALL\n-----------------------------------")
	
App_Msg.GetConnectionStatus(SP)
App_Msg.GetStackInfo(SP)
App_Msg.GetWdInfo(SP)
App_Msg.GetCounters(SP)
App_Msg.GetTimezone(SP)
App_Msg.GetCountryCode(SP)
App_Msg.GetSSID(SP)
App_Msg.GetPassphrase(SP)
App_Msg.GetClientID(SP)
App_Msg.GetClientSecret(SP)
App_Msg.GetCloudEnv(SP)
App_Msg.GetCloudAPIKey(SP)
App_Msg.GetAppVersion(SP)
App_Msg.GetCommand(SP)
App_Msg.GetServerAddress(SP)
App_Msg.GetClientAddress(SP)
	

#*********************************************************************************
# Wrap Up
#*********************************************************************************
SP.close()
sys.exit(1)

 
    
 
 
 