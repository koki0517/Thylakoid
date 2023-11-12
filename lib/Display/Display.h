#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../EEPROM/EEPROM_Address.h"

#include <ILI9341_t3.h>
#include <SPI.h>
#include <SD.h>

class Display {
public:
  Display();
  bool init();
  void clear();
  void home();
  void updateEEPROM();

private:
  #define TFT_DC  9
  #define TFT_CS 37
  #define RESET 10
  ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, RESET);

  #define SD_CS BUILTIN_SDCARD
};
