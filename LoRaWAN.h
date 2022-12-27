#ifndef LoRaWAN_Radioenge_h
#define LoRaWAN_Radioenge_h

#include "Stream.h"

#define _ATZ_               0
#define _DADDR_             1
#define _APPKEY_            2   
#define _APPSKEY_           3   
#define _NWKSKEY_           4   
#define _APPEUI_            5   
#define _ADR_               6
#define _DR_                7
#define _DCS_               8
#define _PNM_               9
#define _RX2FQ_             10 
#define _RX2DR_             11
#define _RX1DL_             12  
#define _RX2DL_             13 
#define _JN1DL_             14
#define _JN2DL_             15
#define _NJM_               16
#define _NWKID_             17
#define _CLASS_             18  
#define _JOIN_              19
#define _NJS_               20
#define _SENDB_             21
#define _SEND_              22
#define _VER_               23
#define _CFM_               24
#define _SNR_               25
#define _RSSI_              26
#define _BAT_               27
#define _BAUDRATE_          28    
#define _NBTRIALS_          29      
#define _KEEPALIVE_         30    
#define _TXCFM_             31
#define _CHMASK_            32  
#define _ADC_               33
#define _GPIOC_             34
#define _WPIN_              35
#define _RPIN_              36
#define _AJOIN_             37

#define ABP                 0
#define OTAA                1

#define CS                  0
#define TTN                 1
#define EN                  2

#define OUTPUT_OPENDRAIN    2
#define OUTPUT_FA_PUSHPULL  3
#define OUTPUT_FA_OPENDRAIN 4

#define INPUT_ADC           6
#define INTERRUPT_RISING    7
#define INTERRUPT_FALLING   8
#define INTERRUPT_CHANGE    10

#define BUFFER_SIZE   256

class LoRaWAN_Radioenge{
  private:
    Stream* SerialLoRaWAN;

    bool feedback = false;
    String _DADDR,
           _APPKEY,
           _APPSKEY,
           _NWKSKEY,
           _APPEUI;

    String AT_CMD[38] = {
          "ATZ",
          "DADDR",
          "APPKEY",
          "APPSKEY",
          "NWKSKEY",
          "APPEUI",
          "ADR",
          "DR",
          "DCS",
          "PNM",
          "RX2FQ",
          "RX2DR",
          "RX1DL",
          "RX2DL",
          "JN1DL",
          "JN2DL",
          "NJM",
          "NWKID",
          "CLASS",
          "JOIN",
          "NJS",
          "SENDB",
          "SEND",
          "VER",
          "CFM",
          "SNR",
          "RSSI",
          "BAT",
          "BAUDRATE",
          "NBTRIALS",
          "KEEPALIVE",
          "TXCFM",
          "CHMASK",
          "ADC",
          "GPIOC",
          "WPIN",
          "RPIN",
          "AJOIN"
    };

    char g_payload[BUFFER_SIZE];
    uint8_t array[BUFFER_SIZE];
    String* payloads = new String[5];  

    String feedbackSerial(String val, bool exception = false){
      String buff = "";
      uint8_t count = 8;

 
      SerialLoRaWAN->println(val);
      if(feedback)
        Serial.println("TX: " + val);

      while(true){
        if(SerialLoRaWAN->available() > 0){
          buff = SerialLoRaWAN->readString();

          if(feedback)
            Serial.println("RX: " + buff);
          
          buff.replace("\n", "");
          buff.replace("\r", "");
      
          if(!exception){
            if(buff.indexOf("ERRO") > 0)
              return "";
            break;
          }
          else{         
            if(buff.indexOf("ERRO") > 0 && count > 0)
              count -= 1;
            else if(count <= 0)
              break;
            else if(buff == "AT_JOIN_OK" || buff == "AT_ALREADY_JOINED")
              break;
          }
        }
      }
      return buff;
    }

    String commandAT(uint8_t cmd, String val = "", bool exception = false){
      String AT = "AT+";
      
      if(!exception && val == "")
        AT = AT + AT_CMD[cmd] + "=?";
      else if(exception)
        AT = AT + AT_CMD[cmd];
      else
        AT = AT + AT_CMD[cmd] + "=" + val;

      delay(50);
      return feedbackSerial(AT, exception);
    }

    String bool_to_intString(bool val){
      if(val)
        return "1";
      return "0";
    }

    void deserializeAT(uint8_t cmd){
      String val_buff = commandAT(cmd);

      char val_char[val_buff.length()];
      val_buff.toCharArray(val_char, val_buff.length());
        
      uint8_t count = 0;  
      for(uint8_t i = 0; i < val_buff.length(); ++i){
        if(val_char[i] != ':')
          payloads[count] += val_char[i];
        else
          ++count;
      }
    }

    uint16_t GPIO(uint8_t cmd, uint8_t pin, uint8_t val = 2){
      String buff = "";
      
      if(val != 2)
        buff = commandAT(cmd, String(pin) + ":" + String(val));
      else
        buff = commandAT(cmd, String(pin));        
      buff.remove(0, 2);   
      buff.replace("AT_OK", "");
      buff.replace("AT_ERROR", String(0xFFFF));
      return (uint16_t)buff.toInt();
    }

  public:
    uint8_t port = 1, confirmado = 0, retries = 0;
    int periodicidade = 0;
  
    LoRaWAN_Radioenge(Stream* _SerialLoRaWAN){
      SerialLoRaWAN = _SerialLoRaWAN;
    }

    void printParameters(){

      Serial.println("---------------------------------------------------");
      Serial.println("                  LoRaWAN Radioenge\n");
      Serial.println(" DevAddr        = " + _DADDR);
      Serial.println(" AppEui         = " + _APPKEY);
      Serial.println(" AppKey         = " + _APPSKEY);
      Serial.println(" AppSKey        = " + _NWKSKEY);
      Serial.println(" AppEui/JoinEui = " + _APPEUI + "\n");
      Serial.println("                    elcereza.com");
      Serial.println("--------------------------------------------------\n");
    }

    void begin(bool _feedback = false){
      feedback = _feedback;      
      
      DADDR();
      APPKEY();
      APPSKEY();
      NWKSKEY();
      APPEUI();
    }

    String DADDR(String val = ""){
      if(val != "") commandAT(_DADDR_, val);
      _DADDR = commandAT(_DADDR_);
      return _DADDR;
    }

    String APPKEY(String val = ""){
      if(val != "") commandAT(_APPKEY_, val);
      _APPKEY = commandAT(_APPKEY_);
      return _APPKEY;
    }

    String APPSKEY(String val = ""){
      if(val != "") commandAT(_APPSKEY_, val);
      _APPSKEY = commandAT(_APPSKEY_);
      return _APPSKEY;
    }

    String NWKSKEY(String val = ""){
      if(val != "") commandAT(_NWKSKEY_, val);
      _NWKSKEY = commandAT(_NWKSKEY_);
      return _NWKSKEY;
    }

    String APPEUI(String val = ""){
      if(val != "") commandAT(_APPEUI_, val);
      _APPEUI = commandAT(_APPEUI_);
      return _APPEUI;
    }

    String CHMASK(String val = ""){
      if(val != "") commandAT(_CHMASK_, val);
      return commandAT(_CHMASK_);
    }

    void ATZ(){
      feedbackSerial("ATZ");      
    }    

    bool ADR(bool val = NULL){
      if(val != NULL) commandAT(_ADR_, bool_to_intString(val));
      return commandAT(_ADR_).toInt();
    }

    uint8_t DR(uint8_t val = 14){
      if(val < 14) (uint8_t)commandAT(_DR_, String(val)).toInt();
      return (uint8_t)commandAT(_DR_).toInt();
    }

    bool DCS(bool val = NULL){
      if(val != NULL) commandAT(_DCS_, bool_to_intString(val));
      return commandAT(_DCS_).toInt();
    }

    bool PNM(bool val = NULL){
      if(val != NULL) commandAT(_PNM_, bool_to_intString(val));
      return commandAT(_PNM_).toInt();
    }

    uint32_t RX2FQ(uint32_t val = NULL){
      if(val != NULL) commandAT(_RX2FQ_, String(val));
      return (uint32_t)commandAT(_RX2FQ_).toInt();
    }

    uint16_t RX2DR(uint16_t val = NULL){
      if(val != NULL) commandAT(_RX2DR_, String(val));
      return (uint16_t)commandAT(_RX2DR_).toInt();
    }

    uint16_t RX1DL(uint16_t val = NULL){
      if(val != NULL) commandAT(_RX1DL_, String(val));
      return (uint16_t)commandAT(_RX1DL_).toInt();
    }

    uint16_t RX2DL(uint16_t val = NULL){
      if(val != NULL) commandAT(_RX2DL_, String(val));
      return (uint16_t)commandAT(_RX2DL_).toInt();
    }

    uint16_t JN1DL(uint16_t val = NULL){
      if(val != NULL) commandAT(_JN1DL_, String(val));
      return (uint16_t)commandAT(_JN1DL_).toInt();
    }

    uint16_t JN2DL(uint16_t val = NULL){
      if(val != NULL) commandAT(_JN2DL_, String(val));
      return (uint16_t)commandAT(_JN2DL_).toInt();
    }

    bool NJM(bool val = NULL){
      if(val != NULL) commandAT(_NJM_, bool_to_intString(val));
      return commandAT(_NJM_).toInt();
    }

    String NWKID(){
      return commandAT(_NWKID_);
    }

    bool CLASS(uint8_t val = 2){
      if(val == 0) commandAT(_CLASS_, "A");
      else if(val == 1) commandAT(_CLASS_, "C");
      if(commandAT(_CLASS_) == "C") return true;
      return false;
    }

    bool JOIN(){
      if(commandAT(_JOIN_, "", true) == "AT_JOIN_OK")
        return true;
      return false;
    }

    bool AJOIN(bool val = NULL){
      if(val != NULL) commandAT(_AJOIN_, bool_to_intString(val));
      return commandAT(_AJOIN_).toInt();
    }

    bool NJS(){
      return commandAT(_NJS_).toInt();
    }

    String VER(){
      return commandAT(_VER_);
    }    

    bool CFM(bool val = NULL){
      if(val != NULL) commandAT(_CFM_, bool_to_intString(val));
      return commandAT(_CFM_).toInt();
    }

    uint8_t SNR(){
      return (uint8_t)commandAT(_SNR_).toInt();
    }

    int RSSI(){
      return commandAT(_RSSI_).toInt();
    }

    float BAT(){
      return commandAT(_BAT_).toInt() * 100 / 253;
    }

    uint16_t BAUDRATE(uint16_t val = NULL){
      if(val != NULL) commandAT(_BAUDRATE_, String(val));
      return commandAT(_BAUDRATE_).toInt();
    }

    uint8_t NBTRIALS(uint8_t val = NULL){
      if(val != NULL) commandAT(_NBTRIALS_, String(val));
      return commandAT(_NBTRIALS_).toInt();
    }

    bool TXCFM(uint8_t _port, bool _confirmado, uint8_t _retries, char* payload){
      uint8_t index = 0;
      
      memset(array, 0, BUFFER_SIZE);
      
      strcpy((char*)&array[index], payload);
      index += strlen(payload);
      
      if(index > BUFFER_SIZE)
        return false;
      
      String _payload = "";
      for(int i = 0; i < index; ++i)
        _payload += array[index];
      
      commandAT(_SENDB_, String(_port) + ":" + String(_retries) + ":" + bool_to_intString(_confirmado) + ":" + String(_payload));
      return true;
    }

    bool KEEPALIVE(bool habilitado = NULL, uint8_t _port = NULL, uint8_t _confirmado = NULL, int _periodicidade = NULL){
      if(habilitado != NULL && _port != NULL && _confirmado != NULL, _periodicidade != NULL)
        commandAT(_KEEPALIVE_, bool_to_intString(habilitado) + ":" + String(_port) + ":" + String(_confirmado) + ":" + String(_periodicidade));      

      deserializeAT(_KEEPALIVE_);

      port          = (uint8_t)payloads[1].toInt();
      confirmado    = (uint8_t)payloads[2].toInt();
      periodicidade = payloads[3].toInt();
      
      return payloads[0].toInt();
    }

    bool pinMode(uint8_t pin, uint8_t modo){
      uint8_t pull = 0; 
      
      if(pin > 9 || modo > 10) return false;
      else if((modo == OUTPUT_FA_PUSHPULL || modo == OUTPUT_FA_OPENDRAIN) && pin != 0 && pin != 1) return false;     
      else if(modo == INPUT_ADC && pin != 0 && pin != 1 && pin != 7 && pin != 8) return false; 
      else if((modo == INTERRUPT_RISING || modo == INTERRUPT_FALLING || modo == INTERRUPT_CHANGE) && pin == 0 && pin == 3 && pin == 7 && pin == 8) return false;
            
      if(modo == INPUT)
        modo = 0;
      else if(modo == OUTPUT)
        modo = 1;

      if(modo == INPUT_PULLUP){
        modo = 0;        
        pull = 1;        
      }
      else if(pull == INPUT_PULLDOWN){
        modo = 0;
        pull = 2;
      }
/*
      deserializeAT(_GPIOC_);

      uint8_t _modo = (uint8_t)payloads[1].toInt();
      uint8_t _pull = (uint8_t)payloads[2].toInt();
      
      if(_modo != modo || _pull != pull)
*/  
      commandAT(_GPIOC_, String(pin) + ":" + String(modo) + ":" + String(pull));

      return true;
    }

    uint8_t digitalRead(uint8_t pin){
      return (uint8_t)GPIO(_RPIN_, pin);   
    }

    uint8_t digitalWrite(uint8_t pin, uint8_t val){
      return (uint8_t)GPIO(_WPIN_, pin, val);
    }
    
    uint16_t analogRead(uint8_t pin){
      return GPIO(_ADC_, pin); 
    }

    void ConfigNetwork(uint8_t njm = NULL, uint8_t net = NULL, String appkey = "", String appeui = ""){
      if(NJM() != njm) NJM(njm);
      if(njm == OTAA && CLASS()) CLASS(false);

      String buff_appkey = appkey;
      buff_appkey.replace(":", "");
      String buff_appeui = appeui;
      buff_appeui.replace(":", "");

      if(_APPKEY != buff_appkey && buff_appkey != "") APPKEY(appkey);
      if(_APPEUI != buff_appeui && buff_appeui != "") APPEUI(appeui);

      uint16_t buff_uint16;
      buff_uint16 = RX1DL(); if((CS == net || TTN == net) && buff_uint16 != 1000) RX1DL(1000); else if(EN == net && buff_uint16 != 5000) RX1DL(5000);
      buff_uint16 = RX2DL(); if((CS == net || TTN == net) && buff_uint16 != 2000) RX2DL(2000); else if(EN == net && buff_uint16 != 6000) RX2DL(6000);
      buff_uint16 = JN1DL(); if((CS == net || TTN == net) && buff_uint16 != 5000) JN1DL(5000); else if(EN == net && buff_uint16 != 5000) JN1DL(5000);
      buff_uint16 = JN2DL(); if((CS == net || TTN == net) && buff_uint16 != 6000) JN2DL(6000); else if(EN == net && buff_uint16 != 6000) JN2DL(5000);

      String buff_string = CHMASK();
      if(net == EN && buff_string != "00ff00000000000000010000") CHMASK("00ff:0000:0000:0000:0001:0000");
      else if((CS == net || TTN == net) && buff_string != "ff0000000000000000020000") CHMASK("ff00:0000:0000:0000:0002:0000");
    }

    bool JoinNetwork(uint8_t njm = NULL, String appkey = "", String appeui = "",  uint8_t net = NULL,  bool autoconfig = true, bool automatic = NULL){
      if(autoconfig)
        ConfigNetwork(njm, net, appkey, appeui);

      if(automatic != AJOIN()) AJOIN(automatic);

      if(!NJS())
        return JOIN();
      else 
        return true;
      return false;
    }

    bool SendString(char* string, uint8_t _port = 1)
    {
      if(string == NULL || strnlen(string, BUFFER_SIZE) >= BUFFER_SIZE)
        return false;
      
      sprintf(g_payload, "%d:%s\0", _port, string);
      commandAT(_SEND_, g_payload);
      return true;
    }

    bool SendRaw(char* payload, uint8_t _port = 1)
    {
      uint8_t index = 0;
      
      memset(array, 0, BUFFER_SIZE);
      
      strcpy((char*)&array[index], payload);
      index += strlen(payload);
      
      if(index > BUFFER_SIZE)
        return false;
      
      String _payload = "";
      for(int i = 0; i < index; ++i)
        _payload += array[index];

      String val = String(_port) + ":" + String(_payload);
      
      commandAT(_SENDB_, val);
      return true;
    }
};
#endif
