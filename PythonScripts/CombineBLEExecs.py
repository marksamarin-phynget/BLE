#!/usr/bin/env python
#********************************************************************************************************						
# CombineBLEExecs.py
# 	Combines the BLE app and stack binary executables into one binary,
#********************************************************************************************************
import sys
import time
import signal



def OpenFiles():

    try:
        App = open (AppExec,'rb')
    except:
        print ('File: ' + AppExec + ' Open Failure')
        sys.exit(1)
        
    try:
        Stack = open (StackExec,'rb')
    except:
        print ('File: ' + StackExec + ' Open Failure')
        sys.exit(1)

    try:
        Final = open (FinalExec,'xb')
    except:
        print ('File: ' + FinalExec + ' Open Failure (must not pre-exist)')
        sys.exit(1)
    
    App_Code   = App.read(0xF000)
    
	# Use stack Seek if stack binary is full image (128K)
	# Do not use if stack binary starts at 0xF000
    #Stack.seek(0xF000)   
    Stack_Code = Stack.read(0x10000)
    
	#Use User config area from App
    App.seek(0x1F000)
    CFG_Area = App.read(0x1000)
    
	#Build finak Executable
    Final.write(App_Code)         # App Binary 0-0xEFFF
    Final.write(Stack_Code)       # Stack Binary (0xF000 - 0x1EFFF)
    Final.write(CFG_Area)         # CFG Area (0x1F000 - 0x1FFFF)

    Final.close()
    return

    
def CombineFiles():

    buffer_64K = bytes(0x10000)
    App_Code = App.fread(0xF000)

    return
    
    

if len(sys.argv) != 4:
    print('\n\nCombineBLEExecs.py\n')
    print('\tCombines the BLE application binary executable with the BLE stack executable')
    print('\tfor use in the CC2640-based PWD BLE board.')
    print('\n\tNote: This works on binary files only, do not use for hex files\n\n')

    print('Usage:  Python CombineBLEExecs.py  <AppExec>,<StackExec>,<FinalExec>\n')

    print('\tFiles are overlaid into <FinalExec> with the following mapping:\n')
       
    print('\t\tFLASH Address     File')
    print('\t\t--------------------------------')
    print('\t\t0x00000-0x0EFFF   <AppExec>   --\ ')
    print('\t\t0x0F000-0x1EFFF   <StackExec> -----> <FinalExec>')
    print('\t\t0x1F000-0x1FFFF   <AppExec>   --/')
    
    
    print('\nNothing Done.....exit\n\n')
    

    sys.exit(1)

AppExec    =  sys.argv[1]    
StackExec  =  sys.argv[2]
FinalExec =  sys.argv[3]


print ('\n\nCombining:  ' + AppExec + ' ==WITH== ' + StackExec + ' ==INTO== ' + FinalExec + '....\n')

OpenFiles()
#CombineFiles()

print ('\nFINISHED.\n\n')
sys.exit(1)

 
    
 
 
 