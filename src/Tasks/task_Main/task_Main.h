#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../../Tools/rtosTools.h"
#include "../../Tools/devices.h"

#include "Functions/RUNALL.h"
#include "Functions/Green.h"

void task_Main(void *pvParameters);
