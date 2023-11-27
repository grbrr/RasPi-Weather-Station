# RasPi-Weather-Station

Repository consisting details how to build weather station with data gathered by MCU, transfer it via UART to Raspberry, save it locally in InfluxDB and show it on Grafana with possibility to send data further via MQTT.

## Hardware used

- Raspberry Pi 4B (you can use any other Raspberry Pi though)
- STM32 NUCLEO-F031K6
- BME280 sensor

![GY-B11 E/P 280 violet PCB](https://cdn2.botland.com.pl/44358-pdt_540/bme280-czujnik-wilgotnosci-temperatury-oraz-cisnienia-110kpa-i2cspi-33v.jpg)

- BH1750 sensor

![GY-302 blue PCB](https://cdn1.botland.com.pl/58777-pdt_540/czujnik-natezenia-swiatla-bh1750.jpg)

- CHN GP2Y1010AU0F sensor

![SHARP GP2Y10](https://nettigo.pl/system/images/887/big.jpg?1446026909)

## Prerequisites for Raspberry Pi

All of these below are going to be set by scripts in this repository, but if you want to do it manually or interface with your current setting, here is the list:

- UART comm enabled
- Docker-Compose
- Mosquitto
- Python
- Python3 libraries: influxdb, ??paho-mqtt??

## Installation

Copy .py scripts and .bash files to your Raspberry Pi. Run `bash prepareEnvironment.bash` to install all necessary libraries and create database in InfluxDB.
You also need to enable UART comm and disable shell via this interface. To do so, run `sudo raspi-config` and go to `Interfacing Options` -> `Serial Port` and when asked about login shell accessibility choose **No**. When asked about enabling port hardware choose **Yes**. Reboot your Raspberry Pi.
98:D3:C1:FD:EE:70
Search for HC-06 Bluetooth module. Use `sudo hcitool scan` to find its MAC address. Make sure that your module is in discoverable mode and that you don't have RF comm blocked. For the latter use command `rfkill list`.
Pairing:
`bluetoothctl`
`power on`
`scan on`
`trust <MAC address>`
`pair <MAC address>`
Default PIN is 1234.
To check if everything is working use `sudo rfcomm connect hci0 <MAC address>`. You should see something like this:

```rfcomm

Connected /dev/rfcomm0 to 98:D3:C1:FD:EE:70 on channel 1
Press CTRL-C for hangup

```

To dedicate this port to this application, use `sudo rfcomm bind hci0 <MAC address>`.
To disconnect use `sudo rfcomm release hci0`.

## External libraries used

[BME280 driver, version 3.4.3](https://github.com/boschsensortec/BME280_driver/tree/bme280_v3.4.3), newer versions use [COINES API](https://github.com/boschsensortec/COINES), it doesn't fit to this project
