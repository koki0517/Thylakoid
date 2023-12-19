#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Gyro.h"
#include "ColorSensor.h"
#include "FloorToF.h"

extern QueueHandle_t queueGyro, queueColor;
extern SemaphoreHandle_t mutexI2C;
extern Gyro gyro;
extern ColorSensor colorLeft, colorRight;
extern FloorToF floortof;

void task_Sensor(void *pvParameters);