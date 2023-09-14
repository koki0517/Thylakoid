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

Gyro gyro;
Display display;
PiZero2W lidar;
ColorSensor color;
Voice voice;

::xSemaphoreHandle mutexGyro;

static void task1(void*) {
  // メインタスク
  while (1) {
  }
}

static void task2(void*) {
  // サブなタスク センサの処理など
  while (1) {
    // ジャイロセンサ(ICM42688)の更新
    ::xSemaphoreTake(mutexGyro, portMAX_DELAY );
    gyro.UpdateGyro();
    ::xSemaphoreGive(mutexGyro);
    ::vTaskDelay(pdMS_TO_TICKS(1));
  }
}

static void task3(void*) {
  // UI系の処理
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
      // voice.play(0); //1の場合実装してないやんけ
      Serial.println("I missed your display.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  if (gyro.init() < 0){
    // ICM42688が見つかんなかったYO 
    while (1){
      // voice.play(2);
      Serial.println("I missed your gyro.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  if (!lidar.init()){
    // ラズパイが見つかんなかったYO
    while (1){
      // voice.play(3);
      Serial.println("I missed your RP2040.");
      ::vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  // RTOSの設定
  ::mutexGyro = xSemaphoreCreateMutex();

  ::xTaskCreate(task1, "task1", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task2, "task2", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task3, "task3", 8192, nullptr, 2, nullptr);
  ::vTaskStartScheduler();
}

void loop(){} // nothing to do is here