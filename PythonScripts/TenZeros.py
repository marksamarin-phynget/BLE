#!/usr/bin/env python
#********************************************************************************************************						
# CombineBLEExecs.py
# 	Combines the BLE app and stack binary executables into one binary,
#********************************************************************************************************
import sys
import time
import signal

import serial



def OpenComPort(SerPort):

	ser = serial.Serial(port=SerPort, baudrate=115200, timeout=0)  # open first serial port with 9600,8,N,1
	print (ser.portstr)       # check which port was really used
	return ser

	 
if len(sys.argv) != 2:
	print('\n\nTenZeroes.py\n')
	print('\tOpens given serial port at 115200 8N1 and transmits ten binary zeroes.')
	print("\tThis will cause a BLE board to reset back into it's bootloader.\n")

	print('\tUsage:  Python TenZeroes.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python TenZeroes.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)


SP = OpenComPort(sys.argv[1])
zerostring = '\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
zeros = zerostring.encode('utf-8')
SP.write(zeros)
SP.close()
print ('\nFINISHED.\n\n')
sys.exit(1)

 
    
 
 
 