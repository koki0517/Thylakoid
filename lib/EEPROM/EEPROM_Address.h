#pragma once
/*
  EEPROMに記録されてる情報を読み出すときのアドレス
  Teensy4.1のEEPROMの範囲は0~428"3"
  EEPR"A"MじゃなくてEEPR"O"Mね
*/

// これなくてもいい希ガス
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

// これは絶対必要
#include <EEPROM.h>

// ライントレースのPID制御の比例定数
const int16_t EEP_Kp = 0;
const int16_t EEP_Ki = 4;
const int16_t EEP_Kd = 8;

// 緑を緑たらしめるHSVの条件
const uint16_t EEP_HueGreenMin = 12;
const uint16_t EEP_HueGreenMax = 14;
const uint16_t EEP_SqturationGreenMin = 16;
const uint16_t EEP_ValueGreenMin = 18;

// 赤を赤たらしめるHSVの条件
const uint16_t EEP_HueRedMin = 20;
const uint16_t EEP_HueRedMax = 22;
const uint16_t EEP_SqturationRedMin = 24;
const uint16_t EEP_ValueRedMin = 26;

const uint16_t EEP_STARTINGTILT = 28; // float from 0 to 3

const uint16_t EEP_FloorProtrusion = 32;

const uint16_t EEP_LoadCell_isPressed = 34;