import time
import serial

ser = serial.Serial('COM9',9600)


def readmsg():
    in_msg = ser.read()
    print(in_msg.decode('utf-16'))


while True:
    readmsg()
    time.sleep(.05)
