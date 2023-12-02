#include "LineSensor.h"

LineSensor::LineSensor(){
  // もしかしたらいらんのかも
  for (int8_t i=0; i < 3; i++){
    pinMode(pins[i],arduino::INPUT);
  }
  //updateEEPROM()
}

int LineSensor::read(uint8_t pin){
  return analogRead(pins[pin]);
}

int LineSensor::readAll(LineSensorData *data){
  data->refrector0 = analogRead(pins[0]);
  data->refrector1 = analogRead(pins[1]);
  data->refrector2 = analogRead(pins[2]);
  data->refrector3 = analogRead(pins[3]);
  data->refrector4 = analogRead(pins[4]);
  data->refrector5 = analogRead(pins[5]);
  data->refrector6 = analogRead(pins[6]);
  data->refrector7 = analogRead(pins[7]);
  data->refrector8 = analogRead(pins[8]);
  return 0;
}

// // いつか必要になるかも
// void LineSensor::updateEEPROM(){
//   EEPROM.get(EEP_LoadCell_isPressed,isPressed);
// }