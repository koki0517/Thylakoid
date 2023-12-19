#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Gyro.h"
#include "ColorSensor.h"
#include "eventList.h"

TaskHandle_t taskUI, taskSensor, taskMain;
SemaphoreHandle_t mutexGyro, mutexUI, mutexSerial, mutexI2C;
QueueHandle_t queueGyro, queueColor, queueFloorToFSensor, queueMainTaskStartOptions, queueEventsHappened;

void xCreateRTOStools(){
  /* == Mutex ============================================================ */
  mutexGyro = xSemaphoreCreateMutex(); // ジャイロのミューテックス 意外と使わないかも?
  mutexUI = xSemaphoreCreateMutex(); // UIを司る権利
  mutexSerial = xSemaphoreCreateMutex(); // シリアルモニタとお話する権利
  mutexI2C = xSemaphoreCreateMutex(); // I2Cを使う権利

  /* == Queue ============================================================ */
  // 補正されたジャイロのデータ
  queueGyro = xQueueCreate(1, sizeof(filteredGyroXYZ));
  queueColor = xQueueCreate(1, sizeof(Color));
  queueMainTaskStartOptions = xQueueCreate(1, sizeof(uint8_t));
  queueEventsHappened = xQueueCreate(1, sizeof(Events));
}