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


def get_JSON_from_serial():
    x = ser.readline()
    try:
        data = json.loads(x)
        return data
    except Exception as e:
        print("Error while reading serial:", e)
        return None


"""- int32_t for temperature with the units 100 * °C
- uint32_t for humidity with the units 1024 * % relative humidity
- uint32_t for pressure
     If macro "BME280_64BIT_ENABLE" is enabled, which it is by default, the unit is 100 * Pascal
     If this macro is disabled, Then the unit is in Pascal"""

while 1:
    data = get_JSON_from_serial()
    if data is not None:
        data["Temperature"] = data["Temperature"] / 100
        data["Humidity"] = round(data["Humidity"] / 1024, 2)
        data["Pressure"] = data["Pressure"] / 100
        print(str(data))
