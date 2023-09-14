#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

SoftwareSerial mySoftwareSerial(/*rx =*/30, /*tx =*/31); 
DFRobotDFPlayerMini myDFPlayer;

// エラーメッセージのファイル番号(仮)
const ERROR_DISPLAY = 0;
const ERROR_TOUCH = 1;
const ERROR_GYRO = 2;
const ERROR_RASPI = 3;
const ERROR_COLOR = 4;

class Voice {
public:
  bool init();
  void play(uint8_t num); // 現状エラーを見つけてもシリアルに出力されるだけになっている いつか返り値でエラーメッセージ返したいね
  void volume(uint8_t volume);
private:
};