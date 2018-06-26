#!/usr/bin/env python
#********************************************************************************************************						
# _Msg.py
# 	
#********************************************************************************************************
import sys
import time
import signal
import serial
from time import sleep

def OpenComPort(SP):

	ser = serial.Serial(port=SP, baudrate=115200, timeout=0)  # open first serial port with 9600,8,N,1
	print ('Port Opened: ', ser.portstr)       # check which port was really used
	return ser

def GetSimpleAppField(SP, sMsg):
	SP.write(sMsg)        
	sleep(.500)
	ack = SP.read(2048)	
	print ('Tx: ',sMsg.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	

def BLE_SetMsg(SP, TxMsg):
	SP.write(TxMsg)        
	sleep(.500)
	RxMsg = SP.read(256)	
	print ('Tx: ',TxMsg.decode('utf-8'))
	print ('Rx: ', RxMsg.decode('utf-8'))
	sys.stdout.flush()
	return RxMsg	
	

def GetBLESWVersion(SP):
	getfwver = '72,26,FW Version Request....\x00'.encode('utf-8')
	return GetSimpleAppField(SP, getfwver)

def GetBLECPUVersion(SP):
	getfwver = '73,27,FW Version Request....\x00'.encode('utf-8')
	return GetSimpleAppField(SP, getfwver)
	
	
def GetCRCs(SP):
	getcrc = '74,25,CRC Request....\x00'.encode('utf-8')
	return GetSimpleAppField(SP, getcrc)
			
def GetFWVersion(SP):
	getfwver = '75,51,FW Version Request....\x00'.encode('utf-8')
	return GetSimpleAppField(SP, getfwver)
	

def SetSN(SP, SerNum):
	mkmsg = '76,43,' + SerNum + '\x00'
	return BLE_SetMsg(SP, mkmsg.encode('utf-8'))
	
def SetWiFiVersion(SP, Version):	
	mkmsg = '77,42,' + Version + '\x00'
	return BLE_SetMsg(SP, mkmsg.encode('utf-8'))

def SWReset(SP):
	mkmsg = '76,14,SW Reset...\x00'
	return BLE_SetMsg(SP, mkmsg.encode('utf-8'))

def WDReset(SP):
	mkmsg = '76,16,WD Reset...\x00'
	return BLE_SetMsg(SP, mkmsg.encode('utf-8'))

def GPFault(SP):
	mkmsg = '76,15,Force GP Fault...\x00'
	return BLE_SetMsg(SP, mkmsg.encode('utf-8'))

	

	
def GetSN(SP):
	return BLE_SetMsg(SP, '75,52,Serial Number Request....\x00'.encode('utf-8'))


def SetScanResultCounter(SP, Counter):
	
	ScanIdMsg = '78,44,' + Counter + '\x00'
	ScanId = ScanIdMsg.encode('utf-8')
	SP.write(ScanId)        

	sleep(.500)
	ack = SP.read(256)	
	print ('Tx: ',ScanId.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	
def SetScanResult(SP, ScanResult):
	
	ScanIdMsg = '79,45,' + ScanResult + '\x00'
	ScanId = ScanIdMsg.encode('utf-8')
	SP.write(ScanId)        

	sleep(.500)
	ack = SP.read(256)	
	print ('Tx: ',ScanId.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def SetScanRespMsg(SP, ID):
	
	ScanIdMsg = '80,20,' + ID + '\x00'
	ScanId = ScanIdMsg.encode('utf-8')
	SP.write(ScanId)        

	sleep(.500)
	ack = SP.read(256)	
	print ('Tx: ',ScanId.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def GetScanRespMsg(SP):
	getsn = '81,21,Get Scan ID....\x00'.encode('utf-8')
	SP.write(getsn)        

	sleep(.500)
	ack = SP.read(2048)	
	print ('Tx: ',getsn.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	
	return ack	
	

def SetCloudEnv(SP, Env):
	
	mkmsg = '81,37,' + Env + '\x00'
	setenv = mkmsg.encode('utf-8')
	SP.write(setenv)        

	sleep(.500)
	ack = SP.read(256)	
	print ('Tx: ',setenv.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	


def AddEvent(SP, Event):	
	mkmsg = '76,22,' + Event + '\x00'
	addevent = mkmsg.encode('utf-8')
	SP.write(addevent)        

	sleep(.500)
	ack = SP.read(256)	
	print ('Tx: ',addevent.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def GetEventCount(SP, Event):	
	mkmsg = '76,23,' + Event + '\x00'
	getevent = mkmsg.encode('utf-8')
	SP.write(getevent)        

	sleep(.500)
	ack = SP.read(256)	
	print ('Tx: ',getevent.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def ClearEventCounts(SP):	
	mkmsg = '76,24,Clear Events...\x00'
	clearevents = mkmsg.encode('utf-8')
	SP.write(clearevents)        

	sleep(.500)
	ack = SP.read(256)	
	print ('Tx: ',clearevents.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

		
def ClientMessage(SP, Message):	
	mkmsg =  Message + '\x00'
	return GetSimpleAppField(SP,mkmsg.encode('utf-8'))
	
def GetConnectionStatus(SP):
	return GetSimpleAppField(SP, '78,10,Connection Status Request....\x00'.encode('utf-8'))
	
def GetStackInfo(SP):
	return GetSimpleAppField(SP, '79,17,Get Stack Info...\x00'.encode('utf-8'))
	
def GetWdInfo(SP):
	return GetSimpleAppField(SP, '80,18,Get Watchdog Info...\x00'.encode('utf-8'))
	
def GetCounters(SP):
	return GetSimpleAppField(SP, '81,19,Get Counters....\x00'.encode('utf-8'))
	
def GetTimezone(SP):
	return GetSimpleAppField(SP, '82,30,Get Timezone....\x00'.encode('utf-8'))
	
def GetCountryCode(SP):
	return GetSimpleAppField(SP, '83,31,Get Country Code....\x00'.encode('utf-8'))
	
def GetSSID(SP):
	return GetSimpleAppField(SP, '84,32,Get SSID....\x00'.encode('utf-8'))
	
def GetPassphrase(SP):
    return GetSimpleAppField(SP, '85,33,Get Passphrase....\x00'.encode('utf-8'))

def GetClientID(SP):
    return GetSimpleAppField(SP, '86,34,Get Client ID....\x00'.encode('utf-8'))

def GetClientSecret(SP):
    return GetSimpleAppField(SP, '87,35,Get Client Secret....\x00'.encode('utf-8'))
		
def GetCloudEnv(SP):
    return GetSimpleAppField(SP, '88,36,Get Cloud Env....\x00'.encode('utf-8'))

def GetCloudAPIKey(SP):
    return GetSimpleAppField(SP, '89,38,Get Cloud API Key....\x00'.encode('utf-8'))
	
def GetAppVersion(SP):
    return GetSimpleAppField(SP, '90,39,Get App Version....\x00'.encode('utf-8'))

def GetCommand(SP):
    return GetSimpleAppField(SP, '91,40,Get Command....\x00'.encode('utf-8'))
	
def GetServerAddress(SP):
    return GetSimpleAppField(SP,  '92,52,Get BLE Server Address...\x00'.encode('utf-8'))

def GetClientAddress(SP):
    return GetSimpleAppField(SP, '93,53,Get BLE Client Address...\x00'.encode('utf-8'))
		