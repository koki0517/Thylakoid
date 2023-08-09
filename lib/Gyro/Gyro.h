#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <ICM42688.h>

class Gyro {
public:
  bool init();
  void get_xyz(float* gyroX,float* gyroY,float* gyroZ);
  int8_t hill();
  bool seesaw();
};