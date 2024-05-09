# Wireless Tiny Gauge 

# Wireless sensor screen
This project is based around a sensor concentrator built on an ESP32S dev board with multiple ESP32 OLED packages as displays. 

Using this project you can place the sensor concentrator in the engine bay and provice 12v and connect into any 5v pressure transducers. Data will be sent over ESP-NOW to ESP32 OLED packages. These only require power.

Bill of materials
|Item|Quantity|URL|
|ESP32S|1| https://tinyurl.com/p3h965xu |
|ESP32 OLED|2| https://tinyurl.com/muxk6epx |
|Buck Converter|2|


Wiring
| ESP32 | Sensor 1 |
|--|--|
| 5v | 5v In |
|GND|GND In |
|GPIO 13|Sense|

| ESP32 | Sensor 2 |
|--|--|
| 5v | 5v In |
|GND|GND In |
|GPIO 12|Sense|

# See 3D-Parts folder for sample STL files.

Included in the parts folder is a single / Dual and Subaru clock pod replacement trim.

# Below is my proto board example
<insert image here>