#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

// 範囲は0~428"3"
// float 4 byte
// double 8 byte

extern int16_t eepGyroRangeStart;
extern int16_t eepGyroRangeEnd;

int16_t eepGyro_RangeStart = 0;
int16_t eepGyro_RangeEnd = 3;

extern int16_t
extern int16_t