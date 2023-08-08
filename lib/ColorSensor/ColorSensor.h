#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>

class ColorSensor {
public:
  bool init(uint8_t addr = 0x29, TwoWire *theWire = &Wire);
  rgb(int16_t* r,int16_t* g,int16_t* b);
  hsv(int16_t* h,int16_t* s,int16_t* v);
  color(int8_t color_code);
}
