#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "Functions/RUNALL.h"
#include "Display.h"

extern QueueHandle_t queueMainTaskStartOptions;
extern Display display;
extern PHOTO Photo;

void task_Main(void *pvParameters);
