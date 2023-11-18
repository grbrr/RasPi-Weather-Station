import serial
import json

import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from dotenv import load_dotenv

load_dotenv()


token = os.getenv("token")
org = "Home"
url = "http://weatherpi.local:8086"

client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)

bucket = "Weather"

write_api = client.write_api(write_options=SYNCHRONOUS)

""" for value in range(5):
  point = (
    Point("measurement1")
    .tag("tagname1", "tagvalue1")
    .field("field1", value)
  )
  write_api.write(bucket=bucket, org="Home", record=point)
  time.sleep(1) # separate points by 1 second """


def init():
    x = serial.Serial(
        port="/dev/rfcomm0",  # Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1,
    )
    return x


def get_JSON_from_serial():
    if ser.in_waiting == 0:
        return None
    x = ser.readline()
    try:
        x = x.decode("utf-8")
        if x[0] == "{" and x[-3] == "}":
            data = json.loads(x)
            return data
        else:
            print(x)
            return None
    except Exception as e:
        print("Error while reading serial:", e)
        return None


"""- int32_t for temperature with the units 100 * °C
- uint32_t for humidity with the units 1024 * % relative humidity
- uint32_t for pressure
     If macro "BME280_64BIT_ENABLE" is enabled, which it is by default, the unit is 100 * Pascal
     If this macro is disabled, Then the unit is in Pascal"""

ser = init()
while 1:
    try:
        data = get_JSON_from_serial()
    except Exception as e:
        print("Error while reading serial:", e)
        data = None
        ser = init()

    if data is not None:
        data["Temperature"] = data["Temperature"] / 100
        data["Humidity"] = round(data["Humidity"] / 1024, 2)
        data["Pressure"] = data["Pressure"] / 100
        data["Ambient Light"] = round(data["Ambient Light"] / 1.2, 2)
        dustVoltage = data["Dust"] * 3.55 / 4096
        if dustVoltage >= 0.6:
            dustDensity = 0.17 * dustVoltage - 0.1
        else:
            dustDensity = 0
        data["Dust"] = float(round(dustDensity, 2))
        data["Wind Speed"] = round(data["Rotations"]*0.068*0.10472,2) # 0.068m circumference, 0.10472m/s per rotation
        print(str(data))
        record = (
            Point("BME280")
            .tag("Location", "Outside")
            .field("Temperature", data["Temperature"])
            .field("Humidity", data["Humidity"])
            .field("Pressure", data["Pressure"])
            .field("Ambient Light", data["Ambient Light"])
            .field("Dust2", data["Dust"])
            .field("Wind Speed", data["Wind Speed"])
        )
        try:
            write_api.write(bucket=bucket, org="Home", record=record)
        except Exception as e:
            print("Error while writing to influxdb:", e)
