#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Gyro.h"

TaskHandle_t taskUI, taskSensor, taskMain;
SemaphoreHandle_t mutexGyro, mutexUI, mutexSerial, mutexI2C0, mutexI2C1;
QueueHandle_t QueueGyro;

void xCreateRTOStools(){
  // ジャイロのミューテックス 意外と使わないかも?
  mutexGyro = xSemaphoreCreateMutex(); // ジャイロをぶん回す権利(ミューテックス) データの取得はキューからどうぞ
  mutexUI = xSemaphoreCreateMutex(); // UIを司る権利
  mutexSerial = xSemaphoreCreateMutex(); // シリアルモニタとお話する権利
  mutexI2C0 = xSemaphoreCreateMutex(); // I2C0をぶん回す権利
  mutexI2C1 = xSemaphoreCreateMutex(); // I2C1をぶん回す権利

  // 補正されたジャイロのデータ
  QueueGyro = xQueueCreate(1, sizeof(filteredGyroXYZ));
}