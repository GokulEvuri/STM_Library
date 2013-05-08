import serial

ser = serial.Serial('/dev/ttyACM0')

print ser

ser.write('print')
print ser.read(1)

ser.close()
