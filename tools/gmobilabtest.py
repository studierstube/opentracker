#!/usr/bin/python

import serial
import binascii

ser = serial.Serial("/dev/ttyUSB0",57600)

if not ser.isOpen():
  print "could not open serial port\n"

print ser

outstr = "c\255 "

print len(outstr)

print "Sending device initialization command for all channels ..."

nwritten = ser.write(outstr)

#print "Chars to read: " + str(ser.inWaiting())

rc = ser.read()

if len(rc) == 1 and rc == 'c':
    print " success!"

print "Sending device start command ... "
ser.write('a')

#print ser.inWaiting()

maxcount = 30
curcount = 0

while curcount < maxcount:
    readdata = ser.read(16)
    printstr = ""
    for c in readdata:
        printstr = printstr + str(hex(ord(c))) + " "
        curcount = curcount + 1
    print printstr

print "Sending stop command ..." 
ser.write('b')

print "Still waiting: " + str(ser.inWaiting())
laststring = ser.read(ser.inWaiting())
print "Still waiting: " + str(ser.inWaiting())

# eventually flush input buffer
#ser.flushInput()
#ser.flushOutput()

#print "Still waiting: " + str(ser.inWaiting())
ser.close()

