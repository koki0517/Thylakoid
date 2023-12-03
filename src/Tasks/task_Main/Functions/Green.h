#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

enum whichColorSensor{
  LEFT,
  RIGHT
};

uint8_t foundGreen(uint8_t whichColorSensor){
  // ここに緑が見つかったかどうかの処理を書く
  return 0;
}