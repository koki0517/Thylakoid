#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include "../EEPROM/EEPROM_Address.h"
#include <Dynamixel2Arduino.h>

class XL330{
public:
  XL330(Serial *DEBUG_SERIAL = &Serial){
    Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
    using namespace ControlTableItem;
  }
  bool init();
  void ledOn(unit8_t ID) dxl.ledOn(ID);
  void ledOff(unit8_t ID) dxl.ledOff(ID);
private:
  Dynamixel2Arduino dxl;
  Serial *DEBUG_SERIAL; // Serial.beginはmainでされている前提
  #define DXL_SERIAL Serial8
  const int DXL_DIR_PIN = 33;
  const float DXL_PROTOCOL_VERSION = 2.0;
  /* サーボのID
    前 ^
    1￣￣￣2
    |     |
    3_____4
    後ろ
  */
  const dxl_front_left = 1;
  const dxl_front_right = 2;
  const dxl_back_left = 3;
  const dxl_back_right = 4;
};