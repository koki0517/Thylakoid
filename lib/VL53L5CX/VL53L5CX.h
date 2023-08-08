#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

extern int16_t libar[64*3];

class VL53L5CX {
public:
  VL53L5CX();
  bool init();
  void get_lidar();
  bool stop();
};