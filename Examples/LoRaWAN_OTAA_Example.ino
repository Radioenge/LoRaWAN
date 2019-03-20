#include <LoRaWAN.h>

/* Includes ---------------------- */
#include <SoftwareSerial.h>
#include <stdint.h>

/* SoftwareSerial handles */
SoftwareSerial* hSerialCommands = NULL;

char APPKEY[] = "50:d1:ad:39:e5:b3:a6:fc:ff:db:3e:a6:3e:e6:5a:a4";
char APPEUI[] = "d9:8c:fb:ab:a0:3a:7a:d2";
char CHMASK[] = "00FF:0000:0000:0000:0001:0000";
char str_counter[32];
int counter = 0;

void setup() {
  Serial.begin(9600); /* Initialize monitor serial */
  Serial.println("Initializing...");
  delay(10000);

  /* Initialize SoftwareSerial */
  hSerialCommands = SerialCommandsInit(7, 6, 9600);
  hSerialCommands->listen();
  
  /* Configure the EndDevice as OTAA */
  //InitializeOTAA(APPKEY, APPEUI);
  SendAtCommand(AT_CHMASK, AtSet, CHMASK);
  Serial.println("Sending JOIN.");
  if(JoinNetwork(0) == RAD_OK)
  {
    Serial.println("EndDevice has joined sucessfully.");
  }
  else
  {
    Serial.println("Error joining the network.");
  }
}

void loop() {
  /* Sends a string containing a counter every 15s */
  sprintf(str_counter, "Counter: %d\r\n\0", counter++);
  Serial.println(str_counter);
  SendString(str_counter, 2);
  delay(15000);
}
