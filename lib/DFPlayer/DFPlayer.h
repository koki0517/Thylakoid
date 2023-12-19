#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

// エラーメッセージのファイル番号(仮)
const uint8_t ERROR_DISPLAY = 0;
const uint8_t ERROR_TOUCH = 1;
const uint8_t ERROR_GYRO = 2;
const uint8_t ERROR_RASPI = 3;
const uint8_t ERROR_COLOR = 4;
const uint8_t ERROR_WALL_TOF = 5;
const uint8_t ERROR_FLOOR_TOF = 6;

class Voice {
public:
  bool init();
  void play(uint8_t num); // 現状エラーを見つけてもシリアルに出力されるだけになっている いつか返り値でエラーメッセージ返したいね
  void volume(uint8_t volume);
private:
  DFRobotDFPlayerMini myDFPlayer = DFRobotDFPlayerMini();
  SoftwareSerial mySoftwareSerial = SoftwareSerial(/*rx =*/30, /*tx =*/31);
};