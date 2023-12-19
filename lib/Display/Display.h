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

// ボタンの情報
typedef struct{
  uint8_t number; // ボタンの通し番号
  int16_t xStart; // ボタンの原点
  int16_t yStart;
  int16_t width; // ボタンの幅、高さ
  int16_t height;
} Button;

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

  uint8_t LOP_pin = 0; // ToDo: ピン番号を確認する

  Button ButtonHome[3] = {
    {1,30,70,75,105}, // RUN
    {2,120,70,80,105}, // SENSOR
    {3,220,70,75,105} // MOTER
  };

  Button ButtonBAR[7] = {
    {1,0,200,45,40}, // HOME
    {2,45,200,45,40}, // RUN
    {3,90,200,50,40}, // SENSOR
    {4,140,200,45,40}, // MOTER
    {5,185,200,45,40}, // GYRO
    {6,230,200,45,40}, // REFRECTOR
    {7,275,200,45,40} // COLOR
  };

  Button ButtonSensor[8] = {
    {1,0,0,45,35}, // RETURN
    {2,20,50,70,70}, // GYRO
    {3,90,50,70,70}, // REFRECTOR
    {4,160,50,70,70}, // COLOR
    {5,230,50,75,70}, // LOADCELL
    {6,20,125,70,70}, // FLOOR_TOF
    {7,90,125,70,70}, // WALL_TOF
    {8,160,125,70,70} // LIDAR
  };

  Button ButtonLOP[1] = {
    {1,145,120,65,40} // FINISH
  };

  uint8_t identifyButton(TS_Point *tsPoint, Button button[], uint8_t size){
    for (uint8_t i = 0; i < size; i++){
      if (tsPoint->x >= button[i].xStart && tsPoint->x < button[i].xStart + button[i].width - 1){ // X軸方向の判定
        if (tsPoint->y > button[i].yStart && tsPoint->y < button[i].yStart + button[i].height - 1){ // Y軸方向の判定
          return button[i].number;
        }
      }
    }
    return 0; // どれにも当てはまらない場合
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

/* ボタンの設定 */
enum class numButtonHome :uint8_t{
  OTHERS,
  RUN,
  SENSOR,
  MOTER
};

enum class numButtonBAR :uint8_t{
  OTHERS,
  HOME,
  RUN,
  SENSOR,
  MOTER,
  GYRO,
  REFRECTOR,
  COLOR
};

enum class numButtonSensor :uint8_t{
  OTHERS,
  RETURN,
  GYRO,
  REFRECTOR,
  COLOR,
  LOADCELL,
  FLOOR_TOF,
  WALL_TOF,
  LIDAR
};

enum class numButtonLOP :uint8_t{
  OTHERS,
  LOP_FINISH
};