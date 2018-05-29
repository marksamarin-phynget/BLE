#!/usr/bin/env python
#********************************************************************************************************						
# BLE_Regression_Test.py
# 	
#********************************************************************************************************
import sys
import time
import signal

import serial
import App_Msg


def OpenComPort(SerPort):

	ser = serial.Serial(port=SerPort, baudrate=115200, timeout=0)  # open first serial port with 9600,8,N,1
	print (ser.portstr)       # check which port was really used
	return ser


print('\n\nBLE_Regression_Test\n-------------------\n')
	
if len(sys.argv) != 2:
	print('\tUsage:  Python BLE_Regression_Test.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BLE_Regression_Test.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)

bConnected = False
SP = App_Msg.OpenComPort(sys.argv[1])

# GET/TEST Remote Status
print("\nTESTING Remote status")
RemStatus = App_Msg.GetRemoteStatus(SP).decode('utf-8')

# Response should be code 11,12, or 13
if RemStatus[:6] == '70,11,':
	bConnected = False
	bClientReady = True
	print('REMOTE STATUS PASS')
elif RemStatus[:6] == '70,12,':
	bConnected = True
	bClientReady = False
	print('REMOTE STATUS PASS')
elif RemStatus[:6] == '70,13,':
	bConnected = True
	bClientReady = True
	print('REMOTE STATUS PASS')
else:
	print('\n\t\t\t\t********* REMOTE STATUS FAIL')
	bConnected = False
	bClientReady = False
	


# GET/TEST Stats
print("\nTESTING Stat retrieval")
App_Msg.GetStats(SP)

# TEST CRCs
print("\nTESTING CRCs")
App_Msg.GetCRCs(SP)

# TEST FW Version
print("\nTESTING FW Version")
FWReadback = App_Msg.GetFWVersion(SP).decode('utf-8')

if FWReadback[6:15] != 'BLE App V':
	print('\n\t\t\t\t********* FW Version FAIL, expecting format: BLE App Vx.xx')
else:
	print('FW VERSION PASS')

# TEST Serial Number
print("\nTESTING SERIAL NUMBER SET FUNCTION")
SNReadback = App_Msg.GetSN(SP)
sRB = SNReadback[6:].decode('utf-8')

if sRB[:6] != '*TEST*':
	testSN = '*TEST*' + sRB[6:]
else:
	testSN = '*test*' + sRB[6:]

App_Msg.SetSN(SP, testSN)
SNReadback = App_Msg.GetSN(SP).decode('utf-8')

if (SNReadback[6:]) != testSN:
	print('\n\t\t\t\t********* ERR SN SET FAIL')
	print('\t\t\t\tSet: ',testSN.encode('utf-8'),'  Readback: ',SNReadback[6:].encode('utf-8'), '\n\n')
else:
	print('SN SET PASS')
	
	
# TEST Model Number	
print("\nTESTING MODEL SET FUNCTION")
ModelReadback = App_Msg.GetModel(SP)
sRB = ModelReadback[6:].decode('utf-8')

if sRB[:6] != '*TEST*':
	testModel = '*TEST*' + sRB[6:]
else:
	testModel = '*test*' + sRB[6:]

App_Msg.SetModel(SP, testModel)
ModelReadback = App_Msg.GetModel(SP).decode('utf-8')

if (ModelReadback[6:]) != testModel:
	print('\n\t\t\t\t********* ERR Model SET FAIL')
	print('\t\t\t\tSet: ',testModel.encode('utf-8'),'  Readback: ',ModelReadback[6:].encode('utf-8'), '\n\n')
else:
	print('MODEL SET PASS')
	
	
	
print("\nTESTING MESSAGING TO BLE CLIENT")
TestMessage = "80,60,Test Message to BLE Client"
CMReadback = App_Msg.ClientMessage(SP, TestMessage).decode('utf-8')

if bConnected == True and bClientReady == True:
	if CMReadback[:6] != '80,30,':
		print('\n\t\t\t\t********* MESSAGE TO BLE CLIENT FAIL')
	else:
		print('MESSAGE TO BLE CLIENT PASS  - check on phone')
		
elif bConnected == True and bClientReady == False:
	if CMReadback[:6] != '80,12,':
		print('\n\t\t\t\t********* MESSAGE TO BLE CLIENT FAIL')
	else:
		print('MESSAGE TO BLE CLIENT PASS -  - Client needs to read previous messsage though')
			
elif bConnected == False:
	if CMReadback[:6] != '80,11,':
		print('\n\t\t\t\t********* MESSAGE TO BLE CLIENT FAIL')
	else:
		print('MESSAGE TO BLE CLIENT PASS - No BLE Client attached')

	
	

SP.close()
print ('\n\nBLE_Regression_Test FINISHED.\n\n')
sys.exit(1)

 
    
 
 
 