#!/usr/bin/python

import serial

s = serial.Serial("/dev/ttyUSB0", 115200)

def sendCode(command):
    try:
        (x) = command
        s.write (str(command))
        print (str(command))
    except:
        print "Error occurred. Ignore and move on"

while(True):
    tePipe = open('/tmp/lcdminer.pipe', 'r')
    sendCode(tePipe.read());
    tePipe.close()
