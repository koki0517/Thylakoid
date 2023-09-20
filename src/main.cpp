/* 
  これはメインなプログラムだよ!
  FreeRTOSで動くよ！
*/

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "Gyro.h"
#include "PiZero2W.h"
#include "Display.h"
#include "ColorSensor.h"
#include "DFPlayer.h"
#include "ToF.h"
#include "LoadCell.h"

Gyro gyro;
Display display;
PiZero2W lidar;
ColorSensor colorLeft(&Wire);
ColorSensor colorRight(&Wire1);
Voice voice;
WallToF walltof(&Wire1);
FloorToF floortof(&Wire);
LoadCell loadcell; // initはない

::xSemaphoreHandle mutexGyro;

static void task1(void*) {
  /*メインタスク
  制御周期短めで動いてほしい
  */
  while (1) {
  }
}

static void task2(void*) {
  /*サブなタスク
  一定の制御周期を保証したいジャイロのためにある
  I2C系のセンサも処理する
  */
  while (1) {
    // ジャイロセンサ(ICM42688)の更新
    ::xSemaphoreTake(mutexGyro, portMAX_DELAY );
    gyro.UpdateGyro();
    ::xSemaphoreGive(mutexGyro);
    ::vTaskDelay(pdMS_TO_TICKS(1));
  }
}

static void task3(void*) {
  /*UI系の処理
  所詮UIなので30Hzとかでも問題ない
  */
  while (1) {
  }
}

FLASHMEM __attribute__((noinline)) void setup() {
  Serial.begin(115200);

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

  // RTOSの設定
  ::mutexGyro = xSemaphoreCreateMutex();

  ::xTaskCreate(task1, "task1", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task2, "task2", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task3, "task3", 8192, nullptr, 2, nullptr);
  ::vTaskStartScheduler();
  // ここの下には何も書くな!!
}

void loop(){} // nothing to do is here