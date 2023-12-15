#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../../../Tools/rtosTools.h"
#include "../../../Tools/devices.h"

void runAll(){
  filteredGyroXYZ gyroXYZ_1;
  vTaskResume(taskUI);
  // ジャイロのデータを取得
  xQueueReceive(queueGyro, &gyroXYZ_1, 0);
}