import time
import socket
from kiosk_mcu_lib import KioskDriver

IP = "127.0.0.1"
PORT = 5005
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #TCP
s.connect((IP, PORT))

s.send('test'.encode())
