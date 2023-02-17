#include "Arduino.h"
#include "LoRa_E32.h"
#include "device-info.cpp"
 
LoRa_E32 e32ttl100(D2, D3); // e32 TX e32 RX
void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);
 
void setup() {
    Serial.begin(9600);
    delay(500);
 
    // Startup all pins and UART
    e32ttl100.begin();
 
    ResponseStructContainer c;
    c = e32ttl100.getConfiguration();
    // It's important get configuration pointer before all other operation
    Configuration configuration = *(Configuration*) c.data;
    Serial.println(c.status.getResponseDescription());
    Serial.println(c.status.code);
 
    printParameters(configuration);
 
    ResponseStructContainer cMi;
    cMi = e32ttl100.getModuleInformation();
    // It's important get information pointer before all other operation
    ModuleInformation mi = *(ModuleInformation*)cMi.data;
 
    Serial.println(cMi.status.getResponseDescription());
    Serial.println(cMi.status.code);
 
    printModuleInformation(mi);
    c.close();
}
 
void loop() {
 
}