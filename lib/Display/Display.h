#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>

class Display {
public:
  ILI9341();
  bool init();
  void clear();
  void home();

private:
};