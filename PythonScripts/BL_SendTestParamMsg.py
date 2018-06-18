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


#Combo PIC addreses start at 1.0.0.6 and increment by 8
TestMsgLeader = [ '30,70,', 
                  '31,71,',  '32,72,', '33,73,', '34,74,', '35,75,',
				  '36,76,', '37,77,',  '38,78,', '39,79,', '40,80,',
                  '31,81,',  '32,82,', '33,83,', '34,84,', '35,85,',
				  '36,86,', '37,87,',  '38,88,', '39,89,', '40,90,'
				]
#*********************************************************************************
# Function: OpenComPort
#*********************************************************************************
def OpenComPort(SerPort):

	ser = serial.Serial(port=SerPort, baudrate=115200, timeout=0)
	print (ser.portstr)       # check which port was really used
	return ser

	
if len(sys.argv) != 4:
	print('\tUsage:  Python BL_SendTestParamMsg.py <Port> <Param #> <Msg>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_SendTestParamMsg.py COM7 1 My_Message\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

#*********************************************************************************
# OPEN serial Channel to BLE board
#*********************************************************************************
bConnected = False
SP = App_Msg.OpenComPort(sys.argv[1])

TxMessage = TestMsgLeader[int(sys.argv[2])] + sys.argv[3]
CMReadback = App_Msg.ClientMessage(SP, TxMessage).decode('utf-8')
print(CMReadback)
