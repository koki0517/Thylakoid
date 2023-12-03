#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Gyro.h"
#include "ColorSensor.h"

TaskHandle_t taskUI, taskSensor, taskMain;
SemaphoreHandle_t mutexGyro, mutexUI, mutexSerial, mutexI2C0, mutexI2C1;
QueueHandle_t QueueGyro, queueColor;

void xCreateRTOStools(){
  /* == Mutex ============================================================ */
  mutexGyro = xSemaphoreCreateMutex(); // ジャイロのミューテックス 意外と使わないかも?
  mutexUI = xSemaphoreCreateMutex(); // UIを司る権利
  mutexSerial = xSemaphoreCreateMutex(); // シリアルモニタとお話する権利
  mutexI2C0 = xSemaphoreCreateMutex(); // I2C0をぶん回す権利
  mutexI2C1 = xSemaphoreCreateMutex(); // I2C1をぶん回す権利

  /* == Queue ============================================================ */
  // 補正されたジャイロのデータ
  QueueGyro = xQueueCreate(1, sizeof(filteredGyroXYZ));
  queueColor = xQueueCreate(1, sizeof(Color));
}