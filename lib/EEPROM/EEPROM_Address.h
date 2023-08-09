#pragma once

// これなくてもいい希ガス
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

// 範囲は0~428"3"
// float 4 byte
// double 8 byte

const int16_t EEPADDR_STARTINGTILT = 0; // float from 0 to 3

const int16_t EEPADDR_ = 4;

const int16_t EEPADDR_;