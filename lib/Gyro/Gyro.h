#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <ICM42688.h>

class Gyro {
public:
  Gyro();
  bool init();
  void get_xyz(float* bno_x,float* bno_y,float* bno_z);
};