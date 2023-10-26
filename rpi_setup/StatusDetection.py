#!/usr/bin/python
import RPi.GPIO as GPIO
import os, time

GPIO.setmode(GPIO.BCM)
GPIO.setup(20, GPIO.IN)
GPIO.setup(21, GPIO.OUT)
GPIO.output(21, GPIO.HIGH)
print("RPi has started (set  pin 21)")

while True:
    if (GPIO.input(20)):
        time.sleep(1)
        print ("RPi needs to stop (by reading pin 20)")
        os.system("sudo shutdown -h now")
        break
    time.sleep(0.5)
