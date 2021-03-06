#ifndef LORAPRS_H
#define LORAPRS_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

#include "BluetoothSerial.h"

class LoraPrs
{
public:
  LoraPrs(int loraFreq, const String & btName, const String & wifiName, 
    const String & wifiKey, const String & aprsLoginCallsign, const String & aprsPass);
  
  void setup();
  void loop();

private:
  void setupWifi(const String & wifiName, const String & wifiKey);
  void setupLora(int loraFreq);
  void setupBt(const String & btName);

  void reconnectWifi();
  
  void onLoraReceived(int packetSize);
  void onBtReceived();

  void kissResetState();

  void onAprsReceived(const String & aprsMessage);

  bool isAX25CrcValid(byte *rxPayload, int payloadLength);
  String convertAX25ToAprs(byte *rxPayload, int payloadLength, const String & signalReport);
  String decodeCall(byte *rxPtr);

  uint16_t updateCrcCcit(uint8_t newByte, uint16_t prevCrc);
  
private:
  enum KissMarker {
    Fend = 0xc0,
    Fesc = 0xdb,
    Tfend = 0xdc,
    Tfesc = 0xdd
  };

  enum KissState {
    Void = 0,
    GetCmd,
    GetData,
    Escape
  };

  enum KissCmd {
    Data = 0x00,
    NoCmd = 0x80
  };

  enum AX25Ctrl {
    UI = 0x03
  };

  enum AX25Pid {
    NoLayer3 = 0xf0
  };
  
  const String CfgLoraprsVersion = "LoRAPRS 0.1";
  
  const byte CfgPinSs = 5;
  const byte CfgPinRst = 26;
  const byte CfgPinDio0 = 14;

  const int CfgBw = 20e3;
  const byte CfgSpread = 11;
  const byte CfgCodingRate = 7;
  const byte CfgSync = 0xf3;
  const byte CfgPower = 20;

  const int CfgAprsPort = 14580;
  const String CfgAprsHost = "rotate.aprs2.net";

private:
  int loraFreq_;
  String btName_;
  String wifiName_;
  String wifiKey_;
  String aprsLogin_;

  KissCmd kissCmd_;
  KissState kissState_;

  BluetoothSerial serialBt_;
};

#endif // LORAPRS_H
