#pragma once

// これなくてもいい希ガス
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

// 範囲は0~428"3"
// float 4 byte
// double 8 byte

const int16_t EEP_STARTINGTILT_START = 0;
const int16_t EEP_STARTINGTILT_END = 3;

const int16_t 