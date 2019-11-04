#include "LoRaWAN.h"

/* Includes ---------------------- */
#include <SoftwareSerial.h>
#include <stdint.h>

/* SoftwareSerial handles */
SoftwareSerial* hSerialCommands = NULL;

/* ABP credentials */
char DEVEUI[] = "00:12:F8:00:00:00:06:67";
char APPEUI[] = "70:B3:D5:7E:D0:02:4B:B4";
char DEVADDR[] = "26:02:18:00";
char NWKSKEY[] = "7B:8E:5B:92:04:D3:C8:94:10:EF:E8:0E:EA:2E:99:8A";
char APPSKEY[] = "19:CD:3B:F9:BD:5E:48:07:67:44:95:72:A5:3E:45:33";

/* Channel masks (AU915) */
char CHMASKTTN[] = "00FF:0000:0000:0000:0000:0000"; //TTN
char CHMASKATC[] = "FF00:0000:0000:0000:0000:0000"; //ATC

char str_counter[32];
int counter = 0;

void setup() {
  Serial.begin(9600); /* Initialize monitor serial */
  Serial.println("Initializing...");
  delay(10000);

  /* Initialize SoftwareSerial */
  hSerialCommands = SerialCommandsInit(7, 6, 9600);
  
  /* Configure the EndDevice as OTAA */
  InitializeABP(DEVEUI, APPEUI, DEVADDR, NWKSKEY, APPSKEY);
  SendAtCommand(AT_CHMASK, AtSet, CHMASKATC);
}

void loop() {
  /* Sends a string containing a counter every 15s */
  sprintf(str_counter, "Counter: %d\r\n\0", counter++);
  Serial.println(str_counter);
  SendString(str_counter, 2);
  delay(15000);
}
