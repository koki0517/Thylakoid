#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "../../../Tools/eventList.h"
extern QueueHandle_t queueEventsHappened;

enum class whichColorSensor{
  BOTH,
  LEFT,
  RIGHT
};

uint8_t foundGreen(whichColorSensor whichColor);
