#ifndef LoRaWAN_Radioenge_h
#define LoRaWAN_Radioenge_h

#include "Stream.h"

#define _ATZ_         0
#define _DADDR_       1
#define _APPKEY_      2   
#define _APPSKEY_     3   
#define _NWKSKEY_     4   
#define _APPEUI_      5   
#define _ADR_         6
#define _DR_          7
#define _DCS_         8
#define _PNM_         9
#define _RX2FQ_       10 
#define _RX2DR_       11
#define _RX1DL_       12  
#define _RX2DL_       13 
#define _JN1DL_       14
#define _JN2DL_       15
#define _NJM_         16
#define _NWKID_       17
#define _CLASS_       18  
#define _JOIN_        19
#define _NJS_         20
#define _SENDB_       21
#define _SEND_        22
#define _VER_         23
#define _CFM_         24
#define _SNR_         25
#define _RSSI_        26
#define _BAT_         27
#define _BAUDRATE_    28    
#define _NBTRIALS_    29      
#define _KEEPALIVE_   30    
#define _TXCFM_       31
#define _CHMASK_      32  
#define _ADC_         33
#define _GPIOC_       34
#define _WPIN_        35
#define _RPIN_        36
#define _AJOIN_       37

#define BUFFER_SIZE   256

#define ABP           0
#define OTAA          1

#define CS            0
#define TTN           1
#define EN            2


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

    String feedbackSerial(String val){
      String buff = "";

      SerialLoRaWAN->println(val);
      if(feedback)
        Serial.println("TX: " + val);

      while(true){
        if(SerialLoRaWAN->available() > 0){
          buff = SerialLoRaWAN->readString();
          
          if(feedback)
            Serial.println("RX: " + buff);
          break;
        }
      }

      buff.replace("\n", "");
      if(buff.indexOf("ERRO") > 0)
        return "";
      return buff;
    }

    String commandAT(uint8_t i, String val = ""){
      String cmd = "AT+";

      if(val == "")
        cmd = cmd + AT_CMD[i] + "=?";
      else if(i == _JOIN_)
         cmd = cmd + AT_CMD[i];
      else
        cmd = cmd + AT_CMD[i] + "=" + val;

      delay(50);
      return feedbackSerial(cmd);
    }

    String bool_to_intString(bool val){
      if(val)
        return "1";
      return "0";
    }

  public:
    LoRaWAN_Radioenge(Stream* _SerialLoRaWAN, bool _feedback = false){
      SerialLoRaWAN = _SerialLoRaWAN;
      feedback = _feedback;
    }

    void printParameters(){

      Serial.println("---------------------------------------------------");
      Serial.println("                  LoRaWAN Radioenge\n");
      Serial.println(" DevAddr        = " + _DADDR);
      Serial.println(" AppEui         = " + _APPKEY);
      Serial.println(" AppKey         = " + _APPSKEY);
      Serial.println(" AppSKey        = " + _NWKSKEY);
      Serial.println(" AppEui/JoinEui = " + _APPEUI);
      Serial.println();
      Serial.println("                    elcereza.com");
      Serial.println("--------------------------------------------------\n");
    }

    void begin(){
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

    uint32_t RX2FQ(uint32_t val = 0xFFFFFFFF){
      if(val < 0xFFFFFFFF) commandAT(_RX2FQ_, String(val));
      return (uint32_t)commandAT(_RX2FQ_).toInt();
    }

    uint16_t RX2DR(uint16_t val = 0xFF){
      if(val < 0xFF) commandAT(_RX2DR_, String(val));
      return (uint16_t)commandAT(_RX2DR_).toInt();
    }

    uint16_t RX1DL(uint16_t val = 0xFFFF){
      if(val < 0xFFFF) commandAT(_RX1DL_, String(val));
      return (uint16_t)commandAT(_RX1DL_).toInt();
    }

    uint16_t RX2DL(uint16_t val = 0xFFFF){
      if(val < 0xFFFF) commandAT(_RX2DL_, String(val));
      return (uint16_t)commandAT(_RX2DL_).toInt();
    }

    uint16_t JN1DL(uint16_t val = 0xFFFF){
      if(val < 0xFFFF) commandAT(_JN1DL_, String(val));
      return (uint16_t)commandAT(_JN1DL_).toInt();
    }

    uint16_t JN2DL(uint16_t val = 0xFFFF){
      if(val < 0xFFFF) commandAT(_JN2DL_, String(val));
      return (uint16_t)commandAT(_JN2DL_).toInt();
    }

    bool NJM(bool val = NULL){
      if(val != NULL) commandAT(_NJM_, bool_to_intString(val));
      return commandAT(_NJM_).toInt();
    }

    String NWKID(){
      return commandAT(_NWKID_);
    }

    bool CLASS(bool val = NULL){
      if(val) commandAT(_CLASS_, "A");
      else if(!val) commandAT(_CLASS_, "C");
      if(commandAT(_CLASS_) == "C") return false;
      return false;
    }

    void ConfigNetwork(uint8_t njm = NULL, uint8_t net = NULL, String appkey = "", String appeui = ""){
      if(NJM() != njm) NJM(njm);
      if(njm == OTAA && !CLASS()) CLASS(true);

      String buff_appkey = appkey;
      buff_appkey.replace(":", "");
      String buff_appeui = appeui;
      buff_appeui.replace(":", "");

      Serial.println("APPKEY: " + _APPKEY);
      Serial.println("buff_appkey: " + buff_appkey);

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

    bool JoinNetwork(uint8_t njm = NULL, uint8_t net = NULL, String appkey = "", String appeui = "", bool automatic = NULL){
      uint8_t count = 9;
      ConfigNetwork(njm, net, appkey, appeui);

      commandAT(_JOIN_);

      while(count > 0){
        if(SerialLoRaWAN->available() > 0){
          if(SerialLoRaWAN->readString().indexOf("ERRO") > 0)
            count -= 1;
          else{
            if(automatic != NULL)
              commandAT(_AJOIN_);
            return commandAT(_NJS_).toInt();
          }
        }
      }
      return false;
    }
};
#endif
