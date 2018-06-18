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

print('\n\BL_Get_App_Data\n-------------------\n')
	
if len(sys.argv) != 2:
	print('\tUsage:  Python BL_Get_App_Data.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_Get_App_Data.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
bConnected = False
SP = App_Msg.OpenComPort(sys.argv[1])

#*********************************************************************************
# GET App Written fields 
#*********************************************************************************
print("\nGet App Write Fields\n-----------------")
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

#App_Msg.SetCloudEnv(SP,"MyCloudEnv")
#App_Msg.GetCloudEnv(SP)


#*********************************************************************************
# Wrap Up
#*********************************************************************************
SP.close()
sys.exit(1)

 
    
 
 
 