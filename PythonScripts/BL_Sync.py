#!/usr/bin/env python
#********************************************************************************************************						
# BL_Sync.py
# 	Perform a bootloader Sync operation
#********************************************************************************************************
import sys
import time
import signal
import serial
from time import sleep
import BL_Msg

if len(sys.argv) != 2:
	print('\n\BL_Sync.py\n')
	print('\tOpens given serial port at 115200 8N1 and transmits a x55x55 sequence.')
	print("\tThis will allow the bootloader to synchronize if it has not already.\n")
	print("\tThen 100 zeros will be transmitted to ensure that the bootloader command buffer")
	print("\tis cleared.\n")


	print('\tUsage:  Python BL_Sync.py <Port>\n')
	print('\twhere: <port> is COM1, /dev/ttyUSB0, etc.\n')
	print('\tex:    Python BL_Sync.py COM7\n')
	print('\nNothing Done.....exit\n\n')
	sys.exit(1)


SP = BL_Msg.OpenComPort(sys.argv[1])
ack = BL_Msg.Sync(SP)
print('ack = ' + ack.hex())

if ack[1] == 0xcc:
	print("Sync Acknowledged...Bootloader ready")
else:    
	if ack[1] == 0x33:
		print("Sync Nacked - Bootloader Probably already synced and ready....")
	else:    
		print("Bootloader Unresponsive....")

SP.close()
print ('\nFINISHED.\n\n')
sys.exit(1)

 
    
 
 
 