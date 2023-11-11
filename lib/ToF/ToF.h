#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include <Wire.h>
#include <VL53L0X.h>
#include <VL53L1X.h>
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
enum ToF_Sensor {
  ToF_FRONT_LEFT = 5,
  ToF_FRONT_RIGHT = 6,
  ToF_BACK_LEFT = 3,
  ToF_BACK_RIGHT = 4
};

#include <vector>

class WallToF {
public:
  WallToF() {
    numToF_VL1 = sizeof(XSHUT_WALL_VL1)/sizeof(uint8_t);
    numToF_VL0 = sizeof(XSHUT_WALL_VL0)/sizeof(uint8_t);
    VL0.resize(numToF_VL0);
    VL1.resize(numToF_VL1);
  }
  bool init(); // タイムアウトでセンサを再起動するときもこれを使う
  uint16_t read(uint8_t sensor_number);
private:
  const char FirstAddress_VL1 = 0x30; 
  uint8_t XSHUT_WALL_VL1[2] = {5,6};
  uint8_t numToF_VL1;
  const char FirstAddress_VL0 = 0x50; 
  uint8_t XSHUT_WALL_VL0[2] = {3,4};
  uint8_t numToF_VL0;
  std::vector<VL53L0X> VL0;
  std::vector<VL53L1X> VL1;
};
