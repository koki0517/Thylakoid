#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"
#include "Gyro.h"
#include "ColorSensor.h"

#include "./Functions/LOP.h"

extern Display display;

void task_UI(void *pvParameters);