#!/usr/bin/python
# -*- coding : iso-8859-1 -*-

import serial 
import time

port = "/dev/ttyACM0"
baudrate = 9600
# starting the communication serial with the arduino
ArduinoPort = serial.Serial(port,baudrate,timeout=1)
f = open("Roll.txt","w")
Header = "Roll\n"

f.write (Header)
while True:

	#time.sleep(0.01)
	getSerialValue = ArduinoPort.readline()
	print '\n Value from the serial port is: %s' % (getSerialValue)

	f.write(getSerialValue)
	f.flush()
f.close()

#ArduinoPort.close()
