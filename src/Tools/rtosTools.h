#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Gyro.h"
#include "ColorSensor.h"

TaskHandle_t taskUI, taskSensor, taskMain;
SemaphoreHandle_t mutexGyro, mutexUI, mutexSerial, SemaphoreI2C;
QueueHandle_t queueGyro, queueColor;

void xCreateRTOStools(){
  /* == Mutex ============================================================ */
  mutexGyro = xSemaphoreCreateMutex(); // ジャイロのミューテックス 意外と使わないかも?
  mutexUI = xSemaphoreCreateMutex(); // UIを司る権利
  mutexSerial = xSemaphoreCreateMutex(); // シリアルモニタとお話する権利
  SemaphoreI2C = xSemaphoreCreateBinary(); // I2Cを使う権利
  xSemaphoreGive(SemaphoreI2C); // The semaphore is created in the 'empty' state, meaning the semaphore must first be given using the xSemaphoreGive() API function

  /* == Queue ============================================================ */
  // 補正されたジャイロのデータ
  queueGyro = xQueueCreate(1, sizeof(filteredGyroXYZ));
  queueColor = xQueueCreate(1, sizeof(Color));
}