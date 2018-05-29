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



	
def GetRemoteStatus(SerPort):
	getremotestatus = '70,41,Remote Status Request....\x00'.encode('utf-8')
	SerPort.write(getremotestatus)        

	sleep(.25)
	ack = SerPort.read(250)	
	print ('Tx: ',getremotestatus.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	

	
	
def GetStats(SerPort):
	getstats = '71,46,Stats Request....\x00'.encode('utf-8')
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
	
	
def ClientMessage(SerPort, Message):
	
	mkmsg =  Message + '\x00'
	sendmsg = mkmsg.encode('utf-8')
	SerPort.write(sendmsg)        

	sleep(.500)
	ack = SerPort.read(256)	
	print ('Tx: ',sendmsg.decode('utf-8'))
	print ('Rx: ', ack.decode('utf-8'))
	sys.stdout.flush()
	return ack	
	
	