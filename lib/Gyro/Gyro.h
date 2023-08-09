#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <ICM42688.h>
#include <EEPROM.h>
#include "../EEPROM/EEPROM_Address.h"

class Gyro {
public:
  int init(); // こいつだけintだから注意
  void get_xyz(float* gyroX,float* gyroY,float* gyroZ);
  int8_t hill();
  bool seesaw();
  void updateEEPROM();
private:
  float startingTilt; // 機体の重量とかシーソーそのものによって変わってくる 当日の調整必須 UIで設定できたら最高
};