/* ---------------------------------------------------
        Radioenge Equipamentos de Telecomunicações
   ---------------------------------------------------
    This library contains a set of functions to configure
    and operate the EndDevice LoRaWAN Radioenge
  
  Date: 19/03/19
*/
#ifndef _LORA_WAN_
#define _LORA_WAN_

#include <stdint.h>
#include "Arduino.h"
#include <SoftwareSerial.h>

/* ----- Defines ------ */

typedef enum
{
  RAD_OK,
  RAD_ERROR
} Status_Typedef;


typedef enum
{
  AtGet,
  AtSet,
  AtRun
} CommandType_e;


/* enum */
typedef enum
{
  AT_DEVADDR,
  AT_APPKEY,
  AT_APPSKEY,
  AT_NWKSKEY,
  AT_APPEUI,
  AT_ADR,
  AT_TXP,
  AT_DR,
  AT_DCS,
  AT_PNM,
  AT_RX2FQ,
  AT_RX2DR,
  AT_RX1DL,
  AT_RX2DL,
  AT_JN1DL,
  AT_JN2DL,
  AT_NJM,
  AT_NWKID,
  AT_CLASS,
  AT_JOIN,
  AT_NJS,
  AT_SENDB,
  AT_SEND,
  AT_VER,
  AT_CFM,
  AT_SNR,
  AT_RSSI,
  AT_BAT,
  AT_BAUDRATE,
  AT_NBTRIALS,
  AT_KEEPALIVE,
  AT_TXCFM,
  AT_CHMASK,
  AT_ADC,
  AT_GPIOC,
  AT_WPIN,
  AT_RPIN,
  NUMBER_OF_COMMANDS
} AT_Commands_e;

/* ----- Public global variables ----- */

/* ----- Public Functions Prototype ----- */

/**
  * @brief Initializes Commands interface and the SoftwareSerial object on given Rx/Tx pins and Baudrate
  * @param rxPin: Serial Rx pin
  * @param txPin: Serial Tx pin
  * @param baudRate: Serial baudrate, in bps
  * @retval pointer to the SoftwareSerial object
  */
SoftwareSerial* SerialCommandsInit(uint8_t rxPin, uint8_t txPin, uint32_t baudRate);



/**
  * @brief Initializes Transparent interface and the SoftwareSerial object on given Rx/Tx pins and Baudrate
  * @param rxPin: Serial Rx pin
  * @param txPin: Serial Tx pin
  * @param baudRate: Serial baudrate, in bps
  * @retval pointer to the SoftwareSerial object
  */
SoftwareSerial* SerialTranspInit(uint8_t rxPin, uint8_t txPin, uint32_t baudRate);



/**
  * @brief Receives a packet from the commands interface
  * @param payload[out]: buffer where the received payload will be copied to
  * @param payloadSize[out]: received payload size
  * @param timeout: reception timeout, in ms
  * @retval RAD_OK or RAD_ERROR
  */
Status_Typedef ReceivePacketCommand(char* payload, uint8_t* payloadSize, uint32_t timeout);



/**
  * @brief Receives a packet from the transparent interface
  * @param payload[out]: buffer where the received payload will be copied to
  * @param payloadSize[out]: received payload size
  * @param timeout: reception timeout, in ms
  * @retval RAD_OK or RAD_ERROR
  */
Status_Typedef ReceivePacketTransp(char* payload, uint8_t* payloadSize, uint32_t timeout);



/**
  * @brief Initializes the EndDevice as OTAA with the given APPKEY and APPEUI
  * @param p_appkey[in]: array containing the APPKEY as a string
  * @param p_appeui[in]: array containing the APPEUI as a string
  * @retval RAD_OK or RAD_ERROR
  */
Status_Typedef InitializeOTAA(char* p_appkey, char* p_appeui);



/**
  * @brief Checks if the EndDevice is joined on the network
  * @retval RAD_OK or RAD_ERROR
  */
Status_Typedef IsJoined();



/**
  * @brief Sends a JOIN request to the network
  * @param retries : number of retries to perform, 0 to infinite retries
  * @retval RAD_OK or RAD_ERROR
  */
Status_Typedef JoinNetwork(uint8_t retries);



/**
  * @brief Sends a string to the specified port
  * @param string : pointer to a string terminated with \0
  * @param port : port number
  * @retval RAD_OK or RAD_ERROR
  */
Status_Typedef SendString(char* string, uint8_t port);



Status_Typedef SendRaw(char* payload);


/**
  * @brief Sends an AT command with the specified type and payload
  * @param command : AT command of type AT_Commands_e
  * @param command_type : AtGet, AtSet or AtRun
  * @param payload : Payload terminated 
  * @retval RAD_OK or RAD_ERROR
  */
Status_Typedef SendAtCommand(AT_Commands_e command, CommandType_e command_type, char* payload);


#endif
