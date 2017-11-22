import time
import serial
import socket
from kiosk_mcu_lib import KioskDriver

k = KioskDriver('COM9', 9600)

IP = "127.0.0.1"
PORT = 5005
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #TCP
s.bind((IP, PORT))
s.listen(10)
print('listening')
conn, addr = s.accept()
print('Connected with ' + addr[0] + ':' + str(addr[1]))



while True:
    test = conn.recv(1024).decode()
    print(test)
    #print('looped')
