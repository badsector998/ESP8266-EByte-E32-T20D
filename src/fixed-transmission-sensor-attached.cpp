#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LoRa_E32.h>
#include "device-info.cpp"
#include <string>

#define SND_PIN A0

SoftwareSerial E32Serial(D2, D3);
LoRa_E32 E32_433(&E32Serial, D5, D7, D6);

void setup() {
    pinMode(SND_PIN, INPUT);

    Serial.begin(9600);
    while (!Serial) {} // wait for serial connection
    delay(100);

    E32_433.begin();

    Serial.println("========= Setting up module ============");
    ResponseStructContainer c;
    c = E32_433.getConfiguration();
    Configuration config = *(Configuration *)c.data;
    config.ADDL = 0x01; // Address low
    config.ADDH = 0x00; // Address high
    config.CHAN = 0x04; // Transmission channel
    config.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION; // fixed transmission mode
    E32_433.setConfiguration(config, WRITE_CFG_PWR_DWN_SAVE); // write config then reboot
    Serial.println("========== Setting up done ==============");
    printParameters(config);
}

void loop() {
    // read sensor value -> create payload -> send -> delay 1s -> rinse & repeat
    int current_sens_value = analogRead(SND_PIN);
    String payload = "Current sound intensity : " + (String)current_sens_value;
    ResponseStatus rs = E32_433.sendBroadcastFixedMessage(0x04, payload);
    Serial.println(rs.getResponseDescription());
    delay(1000);
}