#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../task_Main/task_Main.h"

#include "../../Tools/rtosTools.h"
#include "../../Tools/devices.h"

#include "./Functions/LOP.h"

void task_UI(void *pvParameters);