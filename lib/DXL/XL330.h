#pragma once

/*
  < 関数の命名規則 >
  "drive"がつくものはすべてのサーボに関わる関数
  "run"がつくものは個別のサーボの制御

  実行速度のほしい関数ではforをあえて使っていないことも気分でたまにある 効果のほどは不明

  速度の単位はすべてRPMで統一している
*/

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "../EEPROM/EEPROM_Address.h"
#include <Dynamixel2Arduino.h>

using namespace ControlTableItem;

/* 停止方法のオプション */
enum Stop{
  NOT,
  BRAKE,
  POSITION_MODE,
};

enum XL330ErrorCode{
  SUCCESS,
  TIMEOUT,
  SPEED_SIZE_ERR,
  SPEED_UNSIGNED,
  DEGREES_ZERO,
  FAIL_COMMUNICATION,
  FAIL_COMMUNICATION_1,
  FAIL_COMMUNICATION_2,
  FAIL_COMMUNICATION_3,
  FAIL_COMMUNICATION_4,
};

enum DXL_ID{
  /* サーボのID
    前 ^
    1￣￣￣2
    |     |
    3_____4
    後ろ
  */
  dxl_front_left = 1,
  dxl_front_right = 2,
  dxl_back_left = 3,
  dxl_back_right = 4,
};

enum DXL_MODE{
  VELOCITY,
  POSITION,
};

class XL330 {
public:
  bool init();
  bool ledOn(uint8_t ID);
  bool ledOff(uint8_t ID);
  bool torqueOn(uint8_t ID);
  bool torqueOff(uint8_t ID);
  uint8_t torqueOnAll();
  uint8_t torqueOffAll();

  // とっとこ はしるよハム太郎～♪
  bool drive(float RPM_LEFT, float RPM_RIGHT){
    if (abs(RPM_LEFT) > MAXRPM) return false;
    if (abs(RPM_RIGHT) > MAXRPM) return false;
    /* すべてのサーボを同じ速度でかき回すよ */
    if (!dxl.setGoalVelocity(1, RPM_LEFT, UNIT_RPM)) return false;
    if (!dxl.setGoalVelocity(2, -1*RPM_RIGHT, UNIT_RPM)) return false;
    if (!dxl.setGoalVelocity(3, RPM_LEFT, UNIT_RPM)) return false;
    if (!dxl.setGoalVelocity(4, -1*RPM_RIGHT, UNIT_RPM)) return false;
    return true;
  }
  uint8_t drive_for_degrees(float RPM_LEFT, float RPM_RIGHT, float degrees, uint8_t stop, unsigned int timeout_ms = 10000, bool wait = true);

  // すみっこ はしるよハム太郎～♪
  bool run(uint8_t ID, float RPM);
  bool run_for_degrees(uint8_t ID, float RPM, float degrees, uint8_t stop, unsigned int timeout_ms = 10000, bool wait = true);

  // 大すきなのは～ ヒマワリのタネ～♪
  float position(uint8_t ID);
  bool changeMode(uint8_t ID, uint8_t mode);
  void updateEEPROM();
private:
  Dynamixel2Arduino dxl = Dynamixel2Arduino(Serial8, 33);
  const float MAXRPM = 110;
  uint8_t mode = VELOCITY;
  float Kp_position = 1.0, Ki_position = 0, Kd_position = 5;
  enum direction{
    forward,
    back,
  };
};
