#!/usr/bin/env python
#********************************************************************************************************						
# BL_Msg.py
# 	
#********************************************************************************************************
import sys
import time
import signal

import serial

from time import sleep



def OpenComPort(SerPort):

	ser = serial.Serial(port=SerPort, baudrate=115200, timeout=0)  # open first serial port with 9600,8,N,1
	print (ser.portstr)       # check which port was really used
	return ser


def Sync(SerPort):


	zeros    = bytes([0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00])
	sync     = bytes([0x55,0x55])

	print('Syncing..........\n')

	for x in range(0, 10):
		SerPort.write(zeros)
		SerPort.flush()
		sleep(.1)
    
	ack = SerPort.read(100)

	SerPort.write(sync)
	SerPort.flush()
	sleep(.21)

	for x in range(0, 10):
		SerPort.write(zeros)
		SerPort.flush()
		sleep(.1)
    
	sleep(.250)
	ack = SerPort.read(100)
	
	print ('Sync - ack: ',ack)
	return ack    

def Ping(SerPort):


	ping    = bytes([0x03,0x20,0x20])
	

	print('Sending Ping...')
	sleep(.25)
	ack = SerPort.read(100)

	SerPort.write(sync)
	SerPort.flush()
	sleep(.250)
	ack = SerPort.read(100)
	print ('ack: ',ack)
	
	if ack[1] == 0xcc:
		print('   OK')
	else:
		print('             *** ERR')
    
	return ack    
	
def SendExitCmd(SerPort):
	print('Sending exit command...\n')
	exitstring = '\x03\x25\x25'
	exit = exitstring.encode('utf-8')
	SerPort.write(exit)        
	return  

def SendGetStatusCmd(SerPort):
	#getstatusstring = '\x03\x23\x23'
	getstatus     = bytes([0x00,0x03,0x23,0x23])
	#getstatus = getstatusstring.encode('utf-8')
	SerPort.write(getstatus)      
	SerPort.flush()
	
	sleep(.5)
	ack = SerPort.read(100)	
	print ('Get Status: ', ack)
	return ack

def DisableBootloader(SerPort):

	download = bytes([0x0b,0xfd,0x21,0x00,0x01,0xff,0xd8,0x00,0x00,0x00,0x04])
	data     = bytes([0x07,0xea,0x24,0x00,0x00,0x01,0xc5])
	zeros    = bytes([0x00,0x00,0x00])
	
	#command_download = '\x00\x00\x00\x0b\xfd\x21\x00\x01\xff\xd8\x00\x00\x00\x04'
	#command_data     = '\x00\x00\x00\x07\xea\x24\x00\x00\x01\xc5'
	#download = command_download.encode('utf-8')
	#ata     = command_data.encode('utf-8')
	
	SendGetStatusCmd(SerPort)
	
	for x in range(0, 50): 
		SerPort.write(zeros)
	sleep(3.0)
	
	tot = SerPort.write(download)

	SerPort.write(zeros)
	SerPort.flush()
	sleep(3.0)
	ack = SerPort.read(100)
	
	print('cmd_download - bytes:', tot, 'ack:', ack)
	
	SendGetStatusCmd(SerPort)
	
	SerPort.write(data)
	ack = SerPort.read(100)
	sleep(1.0)
	ack2 = SerPort.read(100)
	ack3 = SerPort.read(100)
		
	print('cmd_data- ack:', ack, ack2, ack3)
	return True
	
	print(ack)
	
	if ack[1] == 0xcc:
		SerPort.write(data)
		sleep(3.0)
		ack = SerPort.read(100)

		if ack[1] == 0xcc:
			return True
		else:
			print('command_data ack fail\n')
			return False
	else:
		print('command_download ack fail\n')
		return False
		
	return False		