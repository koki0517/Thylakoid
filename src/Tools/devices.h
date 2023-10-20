#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "Gyro.h"
#include "Display.h"
#include "PiZero2W.h"
#include "ColorSensor.h"
#include "DFPlayer.h"
#include "ToF.h"
#include "LoadCell.h"
#include "LineSensor.h"

PiZero2W lidar;
ColorSensor colorLeft(&Wire);
ColorSensor colorRight(&Wire1);
Voice voice;
WallToF walltof(&Wire1);
FloorToF floortof(&Wire);
LoadCell loadcell; // initはない
LineSensor line;

void initDevices(){
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

  if (!floortof.init()){
    // 床を監視するToFのどれかが見つかんなかったYO
    while (1){
      // voice.play(ERROR_FLOOR_TOF);
      Serial.println("I missed your Floor ToF.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

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
}