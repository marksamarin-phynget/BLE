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



def OpenComPort(SerPort):

	ser = serial.Serial(port=SerPort, baudrate=115200, timeout=0)  # open first serial port with 9600,8,N,1
	print ('Port Opened: ', ser.portstr)       # check which port was really used
	return ser

def GetSimpleAppField(SerPort, sMsg):
	SerPort.write(sMsg)        
	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',sMsg.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	

	
def GetRemoteStatus(SerPort):
	getremotestatus = '70,41,Remote Status Request....\x00'.encode('utf-8')
	SerPort.write(getremotestatus)        

	sleep(.25)
	ack = SerPort.read(250)	
	print ('Tx: ',getremotestatus.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def GetStackInfo(SerPort):
	getstats = '71,17,Stack Info Request....\x00'.encode('utf-8')
	SerPort.write(getstats)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getstats.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	
def GetWdInfo(SerPort):
	getstats = '71,18,Watchdog Info Request....\x00'.encode('utf-8')
	SerPort.write(getstats)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getstats.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	
	
def GetCounters(SerPort):
	getstats = '71,19,Counters Request....\x00'.encode('utf-8')
	SerPort.write(getstats)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getstats.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	
	
		
def GetCRCs(SerPort):
	getcrc = '72,50,CRC Request....\x00'.encode('utf-8')
	SerPort.write(getcrc)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getcrc.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

		
def GetFWVersion(SerPort):
	getfwver = '73,51,FW Version Request....\x00'.encode('utf-8')
	SerPort.write(getfwver)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getfwver.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	
	
def SetSN(SerPort, SerNum):
	
	mkmsg = '74,53,' + SerNum + '\x00'
	setsnum = mkmsg.encode('utf-8')
	SerPort.write(setsnum)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',setsnum.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	


def GetSN(SerPort):
	getsn = '75,52,Serial Number Request....\x00'.encode('utf-8')
	SerPort.write(getsn)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getsn.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	
	return ack


def SetModel(SerPort, Model):
	
	mkmsg = '76,55,' + Model + '\x00'
	setsnum = mkmsg.encode('utf-8')
	SerPort.write(setsnum)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',setsnum.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def GetModel(SerPort):
	getsn = '77,54,Model Request....\x00'.encode('utf-8')
	SerPort.write(getsn)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getsn.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	
	return ack	


def SetScanResultCounter(SerPort, Counter):
	
	ScanIdMsg = '78,44,' + Counter + '\x00'
	ScanId = ScanIdMsg.encode('utf-8')
	SerPort.write(ScanId)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',ScanId.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	
def SetScanResult(SerPort, ScanResult):
	
	ScanIdMsg = '79,45,' + ScanResult + '\x00'
	ScanId = ScanIdMsg.encode('utf-8')
	SerPort.write(ScanId)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',ScanId.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def SetScanRespMsg(SerPort, ID):
	
	ScanIdMsg = '80,20,' + ID + '\x00'
	ScanId = ScanIdMsg.encode('utf-8')
	SerPort.write(ScanId)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',ScanId.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def GetScanRespMsg(SerPort):
	getsn = '81,21,Get Scan ID....\x00'.encode('utf-8')
	SerPort.write(getsn)        

	sleep(.500)
	ack = SerPort.read(2048)	
	print ('Tx: ',getsn.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	
	return ack	
	

def SetCloudEnv(SerPort, Env):
	
	mkmsg = '81,37,' + Env + '\x00'
	setenv = mkmsg.encode('utf-8')
	SerPort.write(setenv)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',setenv.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	


def AddEvent(SerPort, Event):	
	mkmsg = '76,22,' + Event + '\x00'
	addevent = mkmsg.encode('utf-8')
	SerPort.write(addevent)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',addevent.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def GetEventCount(SerPort, Event):	
	mkmsg = '76,23,' + Event + '\x00'
	getevent = mkmsg.encode('utf-8')
	SerPort.write(getevent)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',getevent.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

def ClearEventCounts(SerPort):	
	mkmsg = '76,24,Clear Events...\x00'
	clearevents = mkmsg.encode('utf-8')
	SerPort.write(clearevents)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',clearevents.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

	
def ClientMessage(SerPort, Message):	
	mkmsg =  Message + '\x00'
	return GetSimpleAppField(SerPort,mkmsg.encode('utf-8'))

def GetTimezone(SerPort):
	return GetSimpleAppField(SerPort, '82,30,Get Timezone....\x00'.encode('utf-8'))
	
def GetCountryCode(SerPort):
	return GetSimpleAppField(SerPort, '83,31,Get Country Code....\x00'.encode('utf-8'))
	
def GetSSID(SerPort):
	return GetSimpleAppField(SerPort, '84,32,Get SSID....\x00'.encode('utf-8'))
	
def GetPassphrase(SerPort):
    return GetSimpleAppField(SerPort, '85,33,Get Passphrase....\x00'.encode('utf-8'))

def GetClientID(SerPort):
    return GetSimpleAppField(SerPort, '86,34,Get Client ID....\x00'.encode('utf-8'))

def GetClientSecret(SerPort):
    return GetSimpleAppField(SerPort, '87,35,Get Client Secret....\x00'.encode('utf-8'))

def GetCloudEnv(SerPort):
    return GetSimpleAppField(SerPort, '88,36,Get Cloud Env....\x00'.encode('utf-8'))

def GetCloudAPIKey(SerPort):
    return GetSimpleAppField(SerPort, '89,38,Get Cloud API Key....\x00'.encode('utf-8'))
	
def GetAppVersion(SerPort):
    return GetSimpleAppField(SerPort, '90,39,Get App Version....\x00'.encode('utf-8'))

def GetCommand(SerPort):
    return GetSimpleAppField(SerPort, '91,40,Get Command....\x00'.encode('utf-8'))
	
	
	