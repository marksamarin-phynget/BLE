#!/usr/bin/env python
#********************************************************************************************************					
# BL_Get_Client_ID.py
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
# OPEN serial Channel to BLE board
#*********************************************************************************
SP = App_Msg.OpenComPort(sys.argv[1])

if SP == True:
	print("\nMonitoring Incoming BLE Messages")



# TEST BLE message RX from client if connected
# tester must manually send message from remote device (phone)
if bConnected == True:
	print("\nTESTING MESSAGING FROM BLE CLIENT - Waiting up to 60 seconds for BLE message")
	print("\nSend a text message from connected device.....")

	starttime =  datetime.datetime.now()
	thistime = 	starttime
	CMReadback[0] = 0
	MessageRx = False
	
	while (thistime - starttime).seconds < 60
		CMReadback = App_Msg.ClientMessage(SP, TestMessage).decode('utf-8')		
		if CMReadback[0] != 0:
			print (CMReadback)		
			sleep(.25)
			CMReadback = App_Msg.ClientMessage(SP, TestMessage).decode('utf-8')		
			print (CMReadback)		
			thistime += datetime.timedelta(seconds=60)
			MessageRx = True
		else:
			thistime =  datetime.datetime.now()
		
	if MessageRx == True:
		print('\nBLE MESSAGE RECEIVED\n'
	else:
		printf(''\n\t\t\t\t********* MESSAGE FROM BLE CLIENT FAIL')
		TestFail = True

if TestFail == True:
	print('\n\n\nALL TESTS PASS (you must manually verify BLE Rx message...\n\n')
else:
	print('\n\n\n**************** REGRESSION TEST FAILURE - Check above\n\n")
		