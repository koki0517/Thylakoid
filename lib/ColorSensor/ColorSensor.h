#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include "../EEPROM/EEPROM_Address.h"

// 判断する色
const uint8_t OTHERS = 0; // 緑でも赤でもない
const uint8_t GREEN = 1; // 緑色
const uint8_t RED = 2; // 赤色

class ColorSensor {
public:
  ColorSensor(TwoWire *theWire = &Wire);
  bool init();
  void rgb(uint16_t* red,uint16_t* green,uint16_t* blue);
  void hsv(uint16_t* h,uint16_t* s,uint16_t* v);
  void updateEEPROM();
  uint8_t colorHSV(); // HSVベースで判断
private:
  Adafruit_TCS34725 color;
  TwoWire *theWire;
  // 緑を緑たらしめるHSVの条件
  const uint16_t HueGreenMin, HueGreenMax, SqturationGreenMin, ValueGreenMin;

  // 赤を赤たらしめるHSVの条件
  const uint16_t HueRedMin, HueRedMax, SqturationRedMin, SqturationRedMin, ValueRedMin;
};
