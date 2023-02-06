#include "LoRaWAN_Radioenge.h"

LoRaWAN_Radioenge LoRa(&Serial2);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  
  LoRa.begin(true);
  LoRa.printParameters();

  if(LoRa.JoinNetwork(OTAA, TTN, true, !true)) 
    LoRa.SendString("elcereza.com", 1);
  
   LoRa.pinMode(2, OUTPUT);
}

void loop() {
  LoRa.digitalWrite(2, 1);
  delay(5000);
  LoRa.digitalWrite(2, 0);
  delay(5000);
}
