#pragma once

/* ToFカメラを積んだRaspberry Pi Zero 2Wと通信するためのクラスだよ
  
*/

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../EEPROM/EEPROM_Address.h"

class PiZero2W {
public:
  PiZero2W();
  bool init();
  void get_lidar();
  bool stop();
};