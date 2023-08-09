#include "Gyro.h"

ICM42688 IMU(SPI, 13); // SPI1 27

void Gyro::updateEEPROM(){
  EEPROM.get(EEPADDR_STARTINGTILT, startingTilt);
}

int Gyro::init(){
  updateEEPROM();
  // start communication with IMU
  return IMU.begin();
}

void Gyro::get_xyz(float* gyroX,float* gyroY,float* gyroZ){
  *gyroX = IMU.gyrX();
  *gyroY = IMU.gyrY();
  *gyroZ = IMU.gyrZ();
}

int8_t Gyro::hill(){
  /* 普通にライントレースしてるときに使うよ
   * 0 -> 平坦
   * 1 -> 上り
   * 2 -> 下り
  */

  float gyroX = IMU.gyrX();
  if (gyroX >= 5){
    return 1;
  } else if (gyroX <= 5){
    return 2;
  } else return 0;
  // Note: 基板で実装する向きが決まらんことにはXYを判断できへん 加速度を使えば「登り始め」「下り始め」をより早く感知できるかも?
}

bool Gyro::seasaw(){
  /* シーソーの傾き始める瞬間を加速度を用いて感知するよ
   * true -> シーソーが傾いた(加速度が大きい)
   * false -> まだ登り坂(加速度が小さい)
  */

  float accX = IMU.accX();
  if (abs(accX) > startingTilt){
    return true;
  } else return false;
  // Note: 上前半に同じく
}