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

#define TINY_GSM_MODEM_SIM800

#include "Arduino.h"
#include "LoRa_E32.h"
#include "device-info.cpp"
#include "PubSubClient.h"
 
// ---------- esp8266 pins --------------
//LoRa_E32 e32ttl(D2, D3, D5, D7, D6);
//LoRa_E32 e32ttl(D2, D3); // Config without connect AUX and M0 M1
 
#include <SoftwareSerial.h>
SoftwareSerial loraSerial(D2, D3); // e32 TX e32 RX
SoftwareSerial simSerial(D6, D7); // sim TX sim RX 
LoRa_E32 e32ttl(&loraSerial, D5, D7, D6);
// -------------------------------------

//------- GRPS Cofig Start ----------------
#include <TinyGsmClient.h>
TinyGsm modem(simSerial);
TinyGsmClient client(modem);

const char apn[] = "3gprs.et";
const char user[] = "3gprs";
const char pass[] = "3gprs";

void reconnect() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt.connect("test-SIM800L")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqtt.publish("outTopic","this is recconecting message");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// -------- GPRS Config Ends -----------------------

// ------------ MQTT Config Starts -----------------
PubSubClient mqtt(client);
const char* broker = "broker.hivemq.com";
const char* topic = "testing-ESP8266-SIM800L";
uint16_t port = 1883;
// ------------ MQTT Config Ends ------------------

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
    simSerial.begin(9600);

    // Initializing modem
    Serial.println("Initializing Modem");
    modem.init();

    String modem_info = modem.getModemInfo();
    Serial.print("Modem Info : ");
    Serial.println(modem_info);

    Serial.print("Waiting for Network");
    if (!modem.waitForNetwork()) {
        Serial.println(" Failed");
        delay(10000);
        return;
    }
    Serial.println(" Success");

    Serial.print("Signal Quality : ");
    Serial.println(modem.getSignalQuality());

    if (modem.isNetworkConnected()) { Serial.println("Network Connected!"); }

    Serial.print("Connecting to ");
    Serial.print(apn);
    if (!modem.gprsConnect(apn))
    {
        Serial.println(" Failed!");
        delay(10000);
        return;
    }
    Serial.println(" Success");
    String IPaddr = modem.getLocalIP();

    if (modem.isGprsConnected()) { Serial.println("GPRS Connected, with IP address : " + IPaddr); }
    // end of modem initialization

    // ------------- start initializing LoRa ----------------
    // e32ttl.resetModule();
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
    Serial.println("LoRa Start listening!");
    // end of lora init

    // set broker server
    mqtt.setServer(broker, port);
}
 
// The loop function is called in an endless loop
void loop()
{
    mqtt.loop();
    if (e32ttl.available()  > 1){

        if (!modem.isNetworkConnected()) {
            Serial.println("Network Disconnected");
            if (!modem.waitForNetwork(180000L, true)) {
                Serial.println("failed to reconnect");
                delay(10000);
                return;
            }

            if(modem.isNetworkConnected()) { Serial.println("Network Reconnected!"); }

            if(!modem.isGprsConnected()) {
                Serial.println("GPRS Network Disconnected");
                Serial.print("Connecting to ");
                Serial.println(apn);
                if (!modem.gprsConnect(apn)) {
                    Serial.println(" failed");
                    delay(10000);
                    return;
                }

                if (modem.isGprsConnected()) { Serial.println(" GPRS Reconnected!"); }
            }
        }

        if (!mqtt.connected()) {
            reconnect();
        }

        ResponseContainer rs = e32ttl.receiveMessage();
        // First of all get the data
        String message = rs.data;
 
        Serial.println(rs.status.getResponseDescription());
        Serial.println(message);

        char buffPayload[256];
        message.toCharArray(buffPayload, 256);
        // int payload = random(10);
        unsigned long lastmsg = millis();
        if (lastmsg - millis() > 10000){
            mqtt.publish(topic, buffPayload);
            lastmsg = millis();
        }
    }
}