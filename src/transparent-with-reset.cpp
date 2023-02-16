#include "Arduino.h"
#include "LoRa_E32.h"

// ---------- esp8266 pins --------------
LoRa_E32 e32ttl(D2, D3, D5, D7, D6);
// LoRa_E32 e32ttl(D2, D3); // Config without connect AUX and M0 M1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(D2, D3);  // e32 TX e32 RX
//LoRa_E32 e32ttl(&mySerial, D5, D7, D6);
// -------------------------------------

// ---------- Arduino pins --------------
//LoRa_E32 e32ttl(2, 3, 5, 7, 6);
// LoRa_E32 e32ttl100(2, 3); // Config without connect AUX and M0 M1

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(2, 3);  // e32 TX e32 RX
//LoRa_E32 e32ttl(&mySerial, 5, 7, 6);
// -------------------------------------
// ---------- Raspberry PI Pico pins --------------
// LoRa_E32 e32ttl(&Serial2, 2, 10, 11); //  RX AUX M0 M1
// -------------------------------------
// ---------------- STM32 --------------------
// HardwareSerial Serial2(USART2);   // PA3  (RX)  PA2  (TX)
// LoRa_E32 e32ttl(&Serial2, PA0, PB0, PB10); //  RX AUX M0 M1
// -------------------------------------------------

void setup() {
  Serial.begin(9600);
  delay(500);

  // Startup all pins and UART
  e32ttl.begin();

//  If you have ever change configuration you must restore It
	ResponseStructContainer c;
	c = e32ttl.getConfiguration();
	Configuration configuration = *(Configuration*) c.data;
	Serial.println(c.status.getResponseDescription());
	configuration.CHAN = 0x17;
	configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
	e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  c.close();
  Serial.println("Hi, I'm going to send message!");

  // Send message
  ResponseStatus rs = e32ttl.sendMessage("Hello, world?");
  // Check If there is some problem of succesfully send
  Serial.println(rs.getResponseDescription());
}

void loop() {
	// If something available
  if (e32ttl.available()>1) {
	  // read the String message
	ResponseContainer rc = e32ttl.receiveMessage();
	// Is something goes wrong print error
	if (rc.status.code!=1){
		rc.status.getResponseDescription();
	}else{
		// Print the data received
		Serial.println(rc.data);
	}
  }
  if (Serial.available()) {
	  String input = Serial.readString();
	  e32ttl.sendMessage(input);
  }
}