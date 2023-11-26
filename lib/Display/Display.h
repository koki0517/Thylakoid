#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../EEPROM/EEPROM_Address.h"

#include <ILI9341_t3.h>
#include <SPI.h>
#include <SdFat.h>

// 動画のファイル名
typedef struct{
  char MATSUKEN[13] = "MATSUKEN";
  char HATA[12] = "HATASEN";
} MOVIE;

// 写真のファイル名
typedef struct{
  char HOME[9] = "HOME";
} PHOTO;

enum PhotoError{
  FILE_OK,
  FILE_NOT_FOUND,
  FILE_SIZE_ERROR,
};

class Display {
public:
  Display();
  bool init();
  void clear(uint16_t color = ILI9341_BLACK);
  
  uint8_t playMovie(char *filename);
  uint8_t drawPhoto(char *filename, int x, int y);

  // 使うことはないだろう
  uint8_t convertPhotoBMPtoRGB565(char *readFileName, char *writeFileName, bool ifDisplay, bool writeSize = true);
  uint8_t convertMovieBMPtoRGB565(char *writeFileName, int numPhotos, bool ifDisplay);

  void updateEEPROM();

  // EEPROMをねじ込まれる要員
  unsigned long clockDisplay = 60000000;
  uint8_t displayDirection = 3;
private:
  #define TFT_DC  9
  #define TFT_CS 37
  #define RESET 10
  #define SD_CS BUILTIN_SDCARD
  #define SD_CONFIG  SdioConfig(FIFO_SDIO)
  ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, RESET);
  SdFs sd;

  #define BUFFPIXEL 240
  uint16_t read16(FsFile &f);
  uint32_t read32(FsFile &f);
};
