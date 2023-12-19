#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "Gyro.h"
#include "LineSensor.h"
#include "ColorSensor.h"
#include "Green.h"

extern QueueHandle_t queueGyro, queueColor;
extern TaskHandle_t taskUI;
extern LineSensor line;

void runAll(bool startTaskUI = true);