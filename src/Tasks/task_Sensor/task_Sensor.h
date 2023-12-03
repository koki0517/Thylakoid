#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../../Tools/rtosTools.h"
#include "../../Tools/devices.h"

#include "../task_Main/Functions/Green.h"

void task_Sensor(void *pvParameters);