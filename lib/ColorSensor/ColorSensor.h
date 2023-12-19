#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include "../EEPROM/EEPROM_Address.h"

// 判断する色
enum Colors {
  OTHERS = 0, // 緑でも赤でもない
  GREEN = 1, // 緑色
  RED = 2, // 赤色
};

typedef struct {
  uint16_t Hue;
  uint16_t Saturation;
  uint16_t Value;
} HSV;

typedef struct {
  uint8_t LEFT;
  uint8_t RIGHT;
} Color;

class ColorSensor {
public:
  ColorSensor(TwoWire *theWire = &Wire);
  bool init();
  void rgb(uint16_t* red,uint16_t* green,uint16_t* blue);
  void hsv(HSV *hsv);
  void updateEEPROM();
  uint8_t colorHSV(); // HSVベースで判断
private:
  Adafruit_TCS34725 color;
  TwoWire *theWire;
  // 緑を緑たらしめるHSVの条件
  uint16_t HueGreenMin, HueGreenMax, SqturationGreenMin, ValueGreenMin;

  // 赤を赤たらしめるHSVの条件
  uint16_t HueRedMin, HueRedMax, SqturationRedMin, SaturationRedMin, ValueRedMin;
};
