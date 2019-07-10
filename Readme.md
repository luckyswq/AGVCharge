# arm code

------

- IDE platform: Keil uVision V5.23.0.0
- OS: Windows 10
- ARM CPU: ARM cortex-M3, STM32F103ZET6
- ARM OS: FreeRTOS V10.1.1

------

PC to Control board:

- serial port, 115200, TTL

Control board to stepper:

- RS485, Modbus RTU, 115200

------

data format:

|header|mode|msg|CRC-16|end|
|---|---|---|---|---|
|0x55, 0xaa|0x00(0x01, 0x02, 0x03)|[4bytes] [4bytes]|[2bytes]|0x0d, 0x0a|

mode:

|mode code|usage|
|---|---|
|0x00|(x,y) mode, send the x,y and the arm-machine will move to that position|
|0x01|(theta, distance) mode, send the theta,distance and the arm-machine will move to that position|
|0x02|drop down and lift up|
|0x03|activate and deactivate sucker apparatus|
|0x04|auto (x, y) mode, send the target position and the arm machine will finish the whole movement|
|0x05|auto (theta, distance) mode, send the target position and the arm machine will finish the whole movement|


msg:

|msg|usage||
|---|---|---|
|8 bytes|mode 0|(x, y), x is the first 4 bytes, y is the second 4 bytes|
||mode 1|(theta, distance), same as above|
||mode 2|0x01: lift down, 0x0f: lift up|
||mode 3|0x01: activate, 0x0f: deactivate|

CRC-16:

from byte0 to the byte before CRC

