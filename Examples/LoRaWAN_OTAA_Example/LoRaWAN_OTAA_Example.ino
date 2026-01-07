#include "LoRaWAN_Radioenge.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialCommand(7, 6, false);
LoRaWAN_Radioenge LoRa(&SerialCommand);

void setup() {
  Serial.begin(9600);
  SerialCommand.begin(9600);
 
  LoRa.begin();
  LoRa.printParameters();

  if(LoRa.JoinNetwork(OTAA, TTN, true)) 
    LoRa.SendString("elcereza.com", 1);
  
  LoRa.pinMode(2, RADIOENGE_OUTPUT_PUSH_PULL);
}

void loop() {
  LoRa.digitalWrite(2, 1);
  delay(5000);
  LoRa.digitalWrite(2, 0);
  delay(5000);
}
