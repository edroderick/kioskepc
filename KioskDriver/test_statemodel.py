import json
import time
from kiosk_mcu_lib import KioskDriver

k = KioskDriver('COM9', 9600)

state = {"flash":0, "statusindicator":"green", "fan_top":0, "fan_bot":0, "prox":0, "helpsw":0, "upscharge":0, "upsdischarge":0, "upsfault":0}

print(state)
