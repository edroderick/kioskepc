import time
from kiosk_mcu_lib import KioskDriver

k = KioskDriver('COM9', 9600)

i = 1

#k.test_check()

def readmsg():
    time.sleep(1)
    if (k.in_waiting() > 0):
        print('read: ' , k.read(k.in_waiting()))

def testStatusIndicator():
    for x in range(8, 0, -1):
        k.setStatusIndicator(x)
        time.sleep(1)

def testFlash():
    k.setFlash(1)
    time.sleep(1)
    k.setFlash(2)
    time.sleep(1)

def testFan1():
    k.setFan1(1)
    time.sleep(1)
    k.setFan1(2)
    time.sleep(1)

def testFan2():
    k.setFan2(1)
    time.sleep(1)
    k.setFan2(2)
    time.sleep(1)

testStatusIndicator()
testFlash()
testFan1()
testFan2()
k.reset()


k.close()
