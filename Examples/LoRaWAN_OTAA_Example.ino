#include "LoRaWAN_Radioenge.h"
#include "Stream.h"

LoRaWAN_Radioenge LoRa(&Serial2);

String AppKey = "50:D1:AD:39:E5:B3:A6:FC:FF:DB:3E:A6:3E:E6:5A:A4";
String AppEui = "21:47:07:35:5E:D3:A3:A0";

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  LoRa.begin(true);
  LoRa.printParameters();

  if(LoRa.JoinNetwork(OTAA, AppKey, AppEui, CS, true, true)) 
    LoRa.SendString("teste", 1);
}

void loop() {
}
