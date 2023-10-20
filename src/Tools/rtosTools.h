#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Gyro.h"

SemaphoreHandle_t mutexGyro;
QueueHandle_t QueueGyro;

void xCreateRTOStools(){
  // ジャイロのミューテックス
  mutexGyro = xSemaphoreCreateMutex();

  // 補正されたジャイロのデータ
  QueueGyro = xQueueCreate(1, sizeof(filteredGyroXYZ));
}