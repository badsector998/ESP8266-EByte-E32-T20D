
# ESP8266 + LoRa EByte E32-433T20D Module

Testing purpose for LoRa EByte E32-433T20D Module with ESP8266 (Wemos D1 Mini). This codebase is developed with PlatformIO and C++.


## General Knowledge

LoRa Ebyte E32-433T20D is based on SX1278 LoRa chip with modified communication protocol with working frequency on ~433MHz. Instead using SPI this module use UART to communicate with the MCU. Simply using RX/TX pin or declare 
```c++
SoftwareSerial mySerial(D2, D3);
```
to use custom pin for RX/TX. Most of the module operation is using Serial with auxilliary pin to read the extended information about the module.

This module also capable for many transmission mode such as : 

- Transparent Transmission
- Fixed Transmission or Point to Point Transmission
- Broadcast Transmission
- Monitoring Transmission

![img](https://www.mischianti.org/wp-content/uploads/2019/10/LoRa_E32_transmittingScenarios.jpg)

Based on manual this module only capable to make transmission within 3 Km with clear line of sight.
## Project Structure

All the source file is stored within `src` folder and build options or configurations are declared on `platformio.ini` file.

Inside `platformio.ini` files the structure are described as follows :

#### Common Config
This configuration will apply to every child that declared below this section. `[env]` stands for 'environment' and it applies globally.
```plaintext
[env]
platform = espressif8266
board = d1_mini
monitor_speed = 9600
lib_deps = xreef/EByte LoRa E32 library@^1.5.10
```

#### Child Config
This section describe the child config that inherits all the `[env]` declarations with extended config such as which port that we will upload, which monitor port we want to connect and which source file we want to build and upload. The following name after `env:` describe the name of the child for this example we set name `transmitter`
```plaintext
[env:trasmitter]
upload_port = /dev/ttyUSB0
monitor_port = /dev/ttyUSB0
build_src_filter = +<transmitter.cpp> -<.git>
```

The same rules follows for the `receiver`.
```plaintext
[env:trasmitter]
upload_port = /dev/ttyUSB1
monitor_port = /dev/ttyUSB1
build_src_filter = +<receiver.cpp> -<.git>
```

Thus on the PlatformIO sections on VSCode we can build, upload and monitor simultaneously for both port `/dev/ttyUSB0` and `/dev/ttyUSB1`.
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