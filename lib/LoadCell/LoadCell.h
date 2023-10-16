#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include "../EEPROM/EEPROM_Address.h"

const uint8_t LEFT_LoadCell = 24;
const uint8_t RIGHT_LoadCell = 25;

class LoadCell {
public:
  LoadCell();
  bool isPressed();
  int16_t position_pressed(int *resultant_force,int *position);
  void updateEEPROM();
private:
  uint8_t pins[2] = {24,25};
  int isPressed;
};
