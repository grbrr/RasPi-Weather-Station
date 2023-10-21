import serial
import json

import influxdb_client, os, time
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from dotenv import load_dotenv

load_dotenv()


token = os.getenv('token')
org = "Home"
url = "http://weatherpi.local:8086"

client = influxdb_client.InfluxDBClient(url=url, token=token, org=org)

bucket="Weather"

write_api = client.write_api(write_options=SYNCHRONOUS)
   
""" for value in range(5):
  point = (
    Point("measurement1")
    .tag("tagname1", "tagvalue1")
    .field("field1", value)
  )
  write_api.write(bucket=bucket, org="Home", record=point)
  time.sleep(1) # separate points by 1 second """

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
        record = (
            Point("BME280")
            .tag("Location", "Outside")
            .field("Temperature", data["Temperature"])
            .field("Humidity", data["Humidity"])
            .field("Pressure", data["Pressure"])
        )
        write_api.write(bucket=bucket, org="Home", record=record)