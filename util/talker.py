import serial

s = serial.Serial("/dev/ttyUSB0", 115200)

def sendCode(command):
    try:
        (x) = command
        s.write (str(int(x)))
        print (str(int(x)))
    except:
        print "Error occurred. Ignore and move on"

while(True):
    tePipe = open('/pipe/door', 'r')
    sendCode(tePipe.read());
    tePipe.close()
