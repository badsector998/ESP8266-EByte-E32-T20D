#include "Arduino.h"
#include "LoRa_E32.h"
 
LoRa_E32 e32ttl100(D2, D3); // e32 TX e32 RX
 
void setup() {
  Serial.begin(9600);
  delay(500);
 
  Serial.println("Hi, I'm going to send message!");
 
  // Startup all pins and UART
  e32ttl100.begin();
 
  // Send message
  ResponseStatus rs = e32ttl100.sendMessage("Hello, world?");
  // Check If there is some problem of successfully send
  Serial.println(rs.getResponseDescription());
}
 
void loop() {
    // If something available
  if (e32ttl100.available()>1) {
      // read the String message
    ResponseContainer rc = e32ttl100.receiveMessage();
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
      e32ttl100.sendMessage(input);
  }
}