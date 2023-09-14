#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <EEPROM.h>
#include "../EEPROM/EEPROM_Address.h"

// 判断する色
const OTHERS = 0; // 緑でも赤でもない
const GREEN = 1; // 緑色
const RED = 2; // 赤色

class ColorSensor {
public:
  ColorSensor(TwoWire *theWire = &Wire);
  bool init();
  void rgb(uint16_t* red,uint16_t* green,uint16_t* blue);
  void hsv(uint16_t* h,uint16_t* s,uint16_t* v);
  void updateEEPROM();
  uint8_t color(); // HSVベースで判断
private:
  // 緑を緑たらしめるHSVの条件
  const uint16_t HueGreenMin, HueGreenMax, SqturationGreenMin, ValueGreenMin;

  // 赤を赤たらしめるHSVの条件
  const uint16_t HueRedMin, HueRedMax, SqturationRedMin, SqturationRedMin;
};
