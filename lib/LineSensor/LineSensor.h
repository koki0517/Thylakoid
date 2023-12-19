#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

typedef struct{
  int refrector0;
  int refrector1;
  int refrector2;
  int refrector3;
  int refrector4;
  int refrector5;
  int refrector6;
  int refrector7;
  int refrector8;
} LineSensorData;

class LineSensor { 
public:
  LineSensor();
  int read(uint8_t pin);
  void readAll(LineSensorData *data);
  // void updateEEPROM(); // いつか必要になるハズ
private:
  uint8_t pins[9] = {22,21,20,15,14,41,40,39,38};
};