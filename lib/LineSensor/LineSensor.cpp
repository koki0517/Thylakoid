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

// // いつか必要になるハズ
// void LoadCell::updateEEPROM(){
//   EEPROM.get(EEP_LoadCell_isPressed,isPressed);
// }