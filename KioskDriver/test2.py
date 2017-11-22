import time
from kiosk_mcu_lib import KioskDriver

k = KioskDriver('COM9', 9600)



while True:
  if (k.in_waiting() > 0):
    message = k.read(k.in_waiting())
    print('read: ', message)
    k.processCMD(message)
    #print(state)
