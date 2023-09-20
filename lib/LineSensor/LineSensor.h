#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

class LineSensor { 
public:
  LineSensor();
  int read(uint8_t pin);
  // void updateEEPROM(); // いつか必要になるハズ
private:
  uint8_t pin[] = {22,21,20,15,14,41,40,39,38};
};