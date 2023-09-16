#pragma once

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

/*数多のToFセンサをまとめるクラス
現時点での構成は
・VL6180X x2 ...床監視用
・VL53L0X x2 ...壁との距離を測定
・VL53L1X x2 ...壁との距離を測定

全てVL53L0XにまとめないのはVL53L1Xの方がなんかカッコいいから ※それによってコードが複雑になってるのは間違いない
VL6180Xは近距離用

他のセンサとのアドレスの干渉に注意！！
*/

class WallToF {
public:
  bool init();
  void get_lidar();
  bool stop();
private:
  const char FirstAddress_VL1 = 0x30; 
  uint8_t XSHUT_WALL_VL1[] = {5,6};
  uint8_t numToF_VL1 = sizeof(XSHUT_WALL)/sizeof(uint8_t);
  const char FirstAddress_VL0 = 0x50; 
  uint8_t XSHUT_WALL_VL0[] = {3,4};
  uint8_t numToF_VL0 = sizeof(XSHUT_WALL)/sizeof(uint8_t);
};

class FloorToF {
public:
  bool init();
  void get_lidar();
  bool stop();
private:
  const char FirstAddress = 0x01;
  uint8_t XSHUT_FLOOR[] = {9,10};
  uint8_t numToF = sizeof(XSHUT_FLOOR)/sizeof(uint8_t);
};