#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include <Wire.h>
#include <VL6180X.h>
#include "../EEPROM/EEPROM_Address.h"

/*
VL6180Xは近距離用
他のセンサとのアドレスの干渉に注意！！
*/

// センサーの通し番号(XSHUTピンに対応させる)
enum Floor_ToF_number{
  ToF_FLOOR_LEFT = 9,
  ToF_FLOOR_RIGHT = 10,
};

class FloorToF {
public:
  void init();
  uint16_t read(uint8_t sensor_number);
  bool findProtrusion(uint8_t sensor_number);
  void updateEEPROM();
private:
  VL6180X VL6[2];
  const char FirstAddress = 0x01;
  uint8_t XSHUT_FLOOR[2] = {9,10};
  uint8_t numToF = sizeof(XSHUT_FLOOR)/sizeof(uint8_t);
  uint16_t FloorProtrusion = 100;
};

// デフォルトの上限は255mm