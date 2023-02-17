
# ESP8266 + LoRa EByte E32-433T20D Module

Testing purpose for LoRa EByte E32-433T20D Module with ESP8266 (Wemos D1 Mini). This codebase is developed with PlatformIO and C++.


## General Knowledge

LoRa Ebyte E32-433T20D is based on SX1278 LoRa chip with modified communication protocol with working frequency on ~433MHz. Instead using SPI this module use UART to communicate with the MCU. Simply using RX/TX pin or declare 
```c++
SoftwareSerial mySerial(D2, D3); // ESP
SoftwareSerial mySerial(2, 3); // Arduino
```
to use custom pin for RX/TX. Most of the module operation is using Serial with auxilliary pin to read the extended information about the module.

This module also capable for many transmission mode such as : 

- Transparent Transmission
- Fixed Transmission or Point to Point Transmission
- Broadcast Transmission
- Monitoring Transmission

![img](https://www.mischianti.org/wp-content/uploads/2019/10/LoRa_E32_transmittingScenarios.jpg)

Based on manual this module only capable to make transmission within 3 Km with clear line of sight. The module also only capable transmitting 58 Byte data.


## Wiring

![img](https://www.mischianti.org/wp-content/uploads/2019/10/LoRa_E32-TTL-100_WemosD1_VD_PU_FullyConnected_bb-e1570517387323.jpg)

Wemos D1 Mini <----> E32 Module

D2  -> TX -> PULL-UP

D3  -> RX -> PULL-UP

D5  -> AUX

D6  -> M1

D7  -> M0

3v3 -> VCC

G   -> GND

It is recommended to use 3v instead 5v on the module for safety measure and use pull up resistor for better stability.
## Library Depedencies

- [PlatformIO](https://platformio.org/)
- [Main Depedencies by Renzo Mischianti](https://reference.arduino.cc/reference/en/libraries/ebyte-lora-e32-library/)
## Module Manual

- [Manual EByte E32-433T20D](https://www.manualslib.com/manual/2641079/Ebyte-E32-433t20d.html)

- [How To Reference](https://www.mischianti.org/2019/10/21/lora-e32-device-for-arduino-esp32-or-esp8266-library-part-2/)