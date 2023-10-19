#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

SemaphoreHandle_t mutexGyro;
QueueHandle_t queueGyroX, queueGyroY, queueGyroZ;

void xCreateRTOStools(){
  mutexGyro = xSemaphoreCreateMutex();
  queueGyroX = xQueueCreate( 10, sizeof( uint32_t ) );
  queueGyroY = xQueueCreate( 10, sizeof( uint32_t ) );
  queueGyroZ = xQueueCreate( 10, sizeof( uint32_t ) );
}