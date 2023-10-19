#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../Tools/rtosTools.h"
#include "../Tools/devices.h"

Gyro gyro;

void task2(void*) {
  /*サブなタスク
  一定の制御周期を保証したいジャイロのためにある
  I2C系のセンサも処理する
  */
  while (1) {
    // ジャイロセンサ(ICM42688)の更新
    xSemaphoreTake(mutexGyro, portMAX_DELAY );
    gyro.UpdateGyro();
    xSemaphoreGive(mutexGyro);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}