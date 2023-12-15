#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "Gyro.h"
#include "Display.h"
#include "PiZero2W.h"
#include "ColorSensor.h"
#include "DFPlayer.h"
#include "WallToF.h"
#include "FloorToF.h"
#include "LoadCell.h"
#include "LineSensor.h"

Gyro gyro;
PiZero2W lidar;
ColorSensor colorLeft(&Wire);
ColorSensor colorRight(&Wire1);
Voice voice;
WallToF walltof;
FloorToF floortof;
LoadCell loadcell; // initはない
LineSensor line;
Display display;

MOVIE KARAMERU_BASUKE,HATA,MATSUKEN;
PHOTO Photo;
LineSensorData lineData;

uint8_t LOP_pin = 0; // ToDo: ピン番号を確認する

bool initDevices(){
  // なんとなくbool型にしたけど別にvoidでいい希ガス

  if (!voice.init()){
    // mp3プレイヤーが見つかんなかったYO
    while (1){
      Serial.println("I missed your gyro.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  if (!display.init()){
    // ディスプレイが見つかんなかったYO
    while (1){
      // voice.play(ERROR_DISPLAY); //ERROR_TOUCHの場合実装してないやんけ
      Serial.println("I missed your display.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
    KARAMERU_BASUKE.FileName = "KARAMERU_BASUKE";
    KARAMERU_BASUKE.numPhotos = display.getFrameCount(&KARAMERU_BASUKE);  
    HATA.FileName = "HATASEN";
    HATA.numPhotos = display.getFrameCount(&HATA);
    MATSUKEN.FileName = "MATSUKEN";
    MATSUKEN.numPhotos = display.getFrameCount(&MATSUKEN);
  }

  if (gyro.init(/*サンプリングレート*/1000) < 0){
    // ICM42688が見つかんなかったYO 
    while (1){
      // voice.play(ERROR_GYRO);
      Serial.println("I missed your gyro.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  if (!lidar.init()){
    // ラズパイが見つかんなかったYO
    while (1){
      // voice.play(ERROR_RASPI);
      Serial.println("I missed your RP2040.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  if (!walltof.init()){
    // 壁を監視するToFのどれかが見つかんなかったYO
    while (1){
      // voice.play(ERROR_WALL_TOF);
      Serial.println("I missed your Wall ToF.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  floortof.init(); // 成功か否かを返す関数がないのでinit()のみ

  if (!colorLeft.init()){
    // ラズパイが見つかんなかったYO
    while (1){
      // voice.play(ERROR_COLOR);
      Serial.println("I missed your Left Color Sensor.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  if (!colorRight.init()){
    // ラズパイが見つかんなかったYO
    while (1){
      // voice.play(ERROR_COLOR);
      Serial.println("I missed your Left Color Sensor.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  pinMode(LOP_pin, arduino::INPUT_PULLUP);
}