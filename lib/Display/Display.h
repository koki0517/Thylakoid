#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>

/* UI階層
home/
|-- Sensor/
|   |-- gyro
|   |---- init
|   |---- raw
|   |---- settung(登り坂と下り坂の値、シーソーの値)
|   |
|   |-- Load Cell
|   |---- raw
|   |---- settung(レスキューキットと障害物の値)
|   |
|   |-- Line
|   |---- raw
|   |---- settung(黒のしきい値(自動と手動)、PIDゲイン)
|   |
|   |-- Color
|   |---- raw(RGB,HSV)
|   |---- settung(赤と緑のHSV)
|   |
|   |-- Floor ToF
|   |---- raw
|   |---- settung(バンプとかの値)
|   |
|   |-- Side ToF
|   |---- raw
|   |---- settung(壁との距離)
|   |
|   |-- Lidar
|   |---- raw
|   |---- settung(不明)
|
|-- Motor
|
|-- Run/
|   |-- ALL
|   |
|   |-- LineTrace/
|   |   |-- LineTrace
|   |   |-- GreenLeft
|   |   |-- GreenRight
|   |   |-- U-turn
|   |   |-- Black
|   |   |-- Hill
|   |
|   |-- Rescue/
|   |   |-- LineTrace
|
*/

class Display {
public:
  ILI9341();
  bool init();
  void clear();
  void home();

private:
};