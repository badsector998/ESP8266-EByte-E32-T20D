/*
 * LoRa E32-TTL-100
 * Receive fixed transmission message as a specified point.
 * https://www.mischianti.org
 *
 * E32-TTL-100----- Arduino UNO or esp8266
 * M0         ----- 3.3v (To config) GND (To send) 7 (To dinamically manage)
 * M1         ----- 3.3v (To config) GND (To send) 6 (To dinamically manage)
 * TX         ----- PIN 2 (PullUP)
 * RX         ----- PIN 3 (PullUP & Voltage divider)
 * AUX        ----- Not connected (5 if you connect)
 * VCC        ----- 3.3v/5v
 * GND        ----- GND
 *
 */
#include "Arduino.h"
#include "LoRa_E32.h"
#include "device-info.cpp"
 
// ---------- esp8266 pins --------------
//LoRa_E32 e32ttl(D2, D3, D5, D7, D6);
//LoRa_E32 e32ttl(D2, D3); // Config without connect AUX and M0 M1
 
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D2, D3); // e32 TX e32 RX
LoRa_E32 e32ttl(&mySerial, D5, D7, D6);
// -------------------------------------
 
// -------------------------------------
void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);
//The setup function is called once at startup of the sketch
void setup()
{
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }
    delay(100);
 
    e32ttl.begin();
 
//  e32ttl.resetModule();
    // After set configuration comment set M0 and M1 to low
    // and reboot if you directly set HIGH M0 and M1 to program
    ResponseStructContainer c;
    c = e32ttl.getConfiguration();
    Configuration configuration = *(Configuration*) c.data;
    configuration.ADDL = BROADCAST_ADDRESS;
    configuration.ADDH = BROADCAST_ADDRESS;
    configuration.CHAN = 0x04;
    configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
    e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    printParameters(configuration);
    // ---------------------------
    Serial.println();
    Serial.println("Start listening!");
}
 
// The loop function is called in an endless loop
void loop()
{
    if (e32ttl.available()  > 1){
        ResponseContainer rs = e32ttl.receiveMessage();
        // First of all get the data
        String message = rs.data;
 
        Serial.println(rs.status.getResponseDescription());
        Serial.println(message);
    }
}