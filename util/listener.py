#!/usr/bin/python
import serial,os

s = serial.Serial("/dev/ttyUSB0", 115200)

f = open("Serial.txt", "w")

while True:
        x = s.readline()
        print(x)
        # f.write(x)
        # f.flush()
        # os.fsync(f)

f.close()
