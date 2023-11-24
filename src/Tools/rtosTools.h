#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Gyro.h"

SemaphoreHandle_t mutexGyro, mutexUI, mutexSerial;
QueueHandle_t QueueGyro;

void xCreateRTOStools(){
  // ジャイロのミューテックス
  mutexGyro = xSemaphoreCreateMutex(); // ジャイロをぶん回す権利(ミューテックス) データの取得はキューからどうぞ
  mutexUI = xSemaphoreCreateMutex(); // UIを司る権利
  mutexSerial = xSemaphoreCreateMutex(); // シリアルモニタとお話する権利

  // 補正されたジャイロのデータ
  QueueGyro = xQueueCreate(1, sizeof(filteredGyroXYZ));
}