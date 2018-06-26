#!/usr/bin/env python
#********************************************************************************************************					
# BL_Get_Reset_Counters.py
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

#print('\n\BL_Get_Reset_Counters\n-------------------\n')
	
if len(sys.argv) != 2:
	print('\tUsage:  Python BL_Get_Reset_Counters.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_Get_Reset_Counters.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
SP = App_Msg.OpenComPort(sys.argv[1])

#*********************************************************************************
# GET 
#*********************************************************************************
print("\nGet Reset Counters\n-----------------")
App_Msg.GetEventCount(SP, "00 - Get TOTAL Reset Count")
App_Msg.GetEventCount(SP, "21 - Get POWER-ON Reset Count")
App_Msg.GetEventCount(SP, "22 - Get RESET-PIN Reset Count")
App_Msg.GetEventCount(SP, "23 - Get VDDS BROWN-OUT Reset Count")
App_Msg.GetEventCount(SP, "24 - Get VDD BROWN-OUT Reset Count")
App_Msg.GetEventCount(SP, "25 - Get VDDR BROWN-OUT Count")
App_Msg.GetEventCount(SP, "26 - Get CLOCK-LOSS Reset Count")
App_Msg.GetEventCount(SP, "27 - Get SW-SYSRESET Reset Count")
App_Msg.GetEventCount(SP, "28 - Get SW-PRCM Reset Count")
App_Msg.GetEventCount(SP, "29 - Get Watchdog Reset Count")
App_Msg.GetEventCount(SP, "30 - Get Lockup Reset Count")

#*********************************************************************************
# Wrap Up
#*********************************************************************************
SP.close()
sys.exit(1)

 
    
 
 
 