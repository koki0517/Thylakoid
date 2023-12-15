#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../EEPROM/EEPROM_Address.h"

#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <SdFat.h>

// 動画のファイル名
typedef struct{
  String FileName;
  int numPhotos;
} MOVIE;

// 写真のファイル名
typedef struct{
  String HOME = "HOME";
  String LOP = "LOP";
  String SENSOR = "SENSOR";
  String TASK_BAR = "BAR";
} PHOTO;

// エラーコード
enum PhotoError{
  FILE_OK,
  FILE_NOT_FOUND,
  FILE_SIZE_ERROR,
  BMP_FILE_EXISTS,
};

class Display {
public:
  Display();
  bool init();
  void clear(uint16_t color);
  
  uint8_t playMovie(MOVIE *movie);
  uint8_t drawPhoto(String *filename, int x, int y);

  // 使うことはないだろう
  uint8_t convertPhotoBMPtoRGB565(String readFileName, String writeFileName, bool ifDisplay, bool writeSize = false /*基本的にfalseにする*/);
  uint8_t convertMovieBMPtoRGB565(MOVIE *movie, bool ifDisplay);

  void updateEEPROM();
  uint64_t getFrameCount(MOVIE *movie); // 動画のフレーム数を返す

  // EEPROMをねじ込まれる要員 あゝあわれ
  unsigned long clockDisplay = 60000000;
  uint8_t displayDirection = 3;
  uint8_t existPhoto(String *filename, bool ifConvert); // ファイルが存在するかどうかを返す
  // uint8_t existMovie(MOVIE *movie, bool ifConvert); // ファイルが存在するかどうかを返す

  // タッチパネル
  bool isTouch(){return ts.touched();}
  TS_Point getTSPoint(){
    if (ts.touched()){
      int16_t x, y;
      TS_Point p = ts.getPoint();

      // 座標をディスプレイベースに変換
      x = p.x - 337;
      if (x < 0) x = 0;
      y = p.y - 172;
      if (y < 0) y = 0;
      x = x *320 / 3629;
      y = y * 240 / 3718;
      return TS_Point(x, y, p.z);
    } else return TS_Point(-1, -1, -1);
  }
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
  uint8_t getSize(String *filename, int *width, int *height);

  // タッチパネル
  #define CS_PIN  36
  XPT2046_Touchscreen ts = XPT2046_Touchscreen(CS_PIN);
};