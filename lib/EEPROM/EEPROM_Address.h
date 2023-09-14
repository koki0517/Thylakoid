#pragma once

// これなくてもいい希ガス
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

// 範囲は0~428"3"
// float 4 byte
// double 8 byte

const int16_t EEP_Kp = 0;
const int16_t EEP_Ki = 4;
const int16_t EEP_Kd = 8;

const int16_t EEP_STARTINGTILT = 12; // float from 0 to 3
