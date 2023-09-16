#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <EEPROM.h>
#include "../EEPROM/EEPROM_Address.h"

/*数多のToFセンサをまとめるクラス
現時点での構成は
・VL6180X x2 ...床監視用
・VL53L0X x2 ...壁との距離を測定
・VL53L1X x2 ...壁との距離を測定

全てVL53L0XにまとめないのはVL53L1Xの方がなんかカッコいいから ※それによってコードが複雑になってるのは間違いない
VL6180Xは近距離用

他のセンサとのアドレスの干渉に注意！！
*/

// センサーの通し番号(XSHUTピンに対応させる)
const uint8_t ToF_FRONT_LEFT = 5;
const uint8_t ToF_FRONT_RIGHT = 6;
const uint8_t ToF_BACK_LEFT = 3;
const uint8_t ToF_BACK_RIGHT = 4;

class WallToF {
public:
  bool init(); // タイムアウトでセンサを再起動するときもこれを使う
  uint16_t read(uint8_t sensor_number);
private:
  const char FirstAddress_VL1 = 0x30; 
  uint8_t XSHUT_WALL_VL1[] = {5,6};
  uint8_t numToF_VL1 = sizeof(XSHUT_WALL)/sizeof(uint8_t);
  const char FirstAddress_VL0 = 0x50; 
  uint8_t XSHUT_WALL_VL0[] = {3,4};
  uint8_t numToF_VL0 = sizeof(XSHUT_WALL)/sizeof(uint8_t);
};

// センサーの通し番号
const ToF_FLOOR_LEFT = 9;
const ToF_FLOOR_RIGHT = 10;
class FloorToF {
public:
  bool init();
  uint16_t read(uint8_t sensor_number);
  bool findProtrusion(uint8_t sensor_number);
  void updateEEPROM();
private:
  const char FirstAddress = 0x01;
  uint8_t XSHUT_FLOOR[] = {9,10};
  uint8_t numToF = sizeof(XSHUT_FLOOR)/sizeof(uint8_t);
  uint16_t FloorProtrusion;
};