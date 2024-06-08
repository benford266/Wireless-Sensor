# Wireless Tiny Gauge 

![Project Image](https://github.com/benford266/Wireless-Sensor/blob/Dev/Resources/IMG_3197.jpeg?raw=true)


# Project Overview
This project is based around a sensor concentrator built on an ESP32S dev board with multiple ESP32 OLED packages as displays.
Voltage dividers will be required to bring the 5v working voltage of the sensors down to the 3.3v working voltage of the ESP32 module. 

Using this project you can place the sensor concentrator in the engine bay and provide 12v and connect into any 5v pressure transducers. Data will be sent over ESP-NOW to ESP32 OLED packages. These only require power.

# Bill of materials
|Item|Quantity|URL|
|--|--|--|
|ESP32S|1| https://tinyurl.com/p3h965xu |
|ESP32 OLED|2| https://tinyurl.com/muxk6epx |
|Buck Converter|2|
|Pressure Transducer|2|
|Resistor 200Ohm|2|
|Resistor 400Ohm|2|


# Wiring
| ESP32 | Sensor 1 |
|--|--|
| 5v | 5v In |
|GND|GND In |
|Voltage Divider|Sense|

| ESP32 | Sensor 2 |
|--|--|
| 5v | 5v In |
|GND|GND In |◊
|Voltage Divider|Sense|

![Wiring Diagram Image](https://github.com/benford266/Wireless-Sensor/blob/Dev/Resources/WiringDiagram.png?raw=true)

# See 3D-Parts folder for sample STL files.

Included in the parts folder is a single / Dual mount and Subaru clock pod replacement trim.
Also included is an enclosure for the sensor concentrator

# Below is my prototype board example
![Project Image](https://github.com/benford266/Wireless-Sensor/blob/Dev/Resources/IMG_3196.jpeg?raw=true)