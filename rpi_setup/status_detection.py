#!/usr/bin/python
import RPi.GPIO as GPIO
import os, time
import signal

class GracefulKiller:
  kill_now = False
  def __init__(self):
    signal.signal(signal.SIGINT, self.exit_gracefully)
    signal.signal(signal.SIGTERM, self.exit_gracefully)

  def exit_gracefully(self, *args):
    self.kill_now = True

killer = GracefulKiller()
GPIO.setmode(GPIO.BCM)
GPIO.setup(20, GPIO.IN)
GPIO.setup(21, GPIO.OUT)
GPIO.output(21, GPIO.HIGH)

print("RPi has started (set  pin 21)", flush = True)

while not killer.kill_now:
    if (GPIO.input(20)):
        time.sleep(1)
        print ("RPi needs to stop (by reading pin 20)", flush = True)
        os.system("sudo poweroff")
    time.sleep(0.5)