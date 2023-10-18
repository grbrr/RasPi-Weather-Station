# RasPi-Weather-Station
Repository consisting details how to build weather station saving data locally in InfluxDB and showing it on Grafana with possibility to send data via MQTT

## Hardware used
- Raspberry Pi 4B (you can use any other Raspberry Pi though)
- BME280 sensor

![GY-B11 E/P 280 violet PCB](https://cdn2.botland.com.pl/44358-pdt_540/bme280-czujnik-wilgotnosci-temperatury-oraz-cisnienia-110kpa-i2cspi-33v.jpg)
- BH1750 sensor

![GY-302 blue PCB](https://cdn1.botland.com.pl/58777-pdt_540/czujnik-natezenia-swiatla-bh1750.jpg)
- CHN GP2Y1010AU0F sensor

![SHARP GP2Y10](https://nettigo.pl/system/images/887/big.jpg?1446026909)

## Prerequisites for Raspberry Pi
All of these below are going to be set by scripts in this repository, but if you want to do it manually or interface with your current setting, here is the list:
- InfluxDB
- Grafana
- Mosquitto
- Python3
- Python3 libraries: influxdb, ??paho-mqtt??, smbus2, RPi.bme280

## Installation
Copy .py scripts and .bash files to your Raspberry Pi. Run `bash prepareEnvironment.bash` to install all necessary libraries and create database in InfluxDB.

Later?: You can also run `bash installCrontab.bash` to install crontab for automatic data collection.

## Pinout
| PIN | BCM | Purpose       | Connected to              |
| --- | --- | ------------- | ------------------------- |
| 1   | 3V3 | Power supply  | BME280, BH1750, GP2Y10    |
| 3   | SDA | I2C data      | BME280, BH1750            |
| 5   | SCL | I2C clock     | BME280, BH1750            |
| 6   | GND | Ground        | BME280, BH1750, GP2Y10    |