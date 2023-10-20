import serial

# Python3 code to demonstrate
# convert dictionary string to dictionary
# using json.loads()
import json


ser = serial.Serial(
    port="/dev/ttyS0",  # Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1,
)

while 1:
    x = ser.readline()
    #print(x.decode("utf-8"))
    
    data = json.loads(x)
    print(str(data))

