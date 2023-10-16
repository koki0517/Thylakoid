#include "Gyro.h"

ICM42688 IMU(SPI1, 13); // SPI1 27

Madgwick MadgwickFilter;

void Gyro::updateEEPROM(){
  EEPROM.get(EEP_STARTINGTILT, startingTilt);
}

int Gyro::init(int Madgwickfrequency){
  updateEEPROM();
  MadgwickFilter.begin(Madgwickfrequency);
  // start communication with IMU
  return IMU.begin();
}

void Gyro::get_xyz(float* gyroX,float* gyroY,float* gyroZ){
  *gyroX = filteredGyroX;
  *gyroY = filteredGyroY;
  *gyroZ = filteredGyroZ;
}

void Gyro::get_raw(float* rawGyroX,float* rawGyroY,float* rawGyroZ, float* rawAccX,float* rawAccY,float* rawAccZ){
  *rawGyroX = gyrX;
  *rawGyroY = gyrY;
  *rawGyroZ = gyrZ;
  *rawAccX = accX;
  *rawAccY = accY;
  *rawAccZ = accZ;
}

int8_t Gyro::hill(){
  /* 普通にライントレースしてるときに使うよ
   * 0 -> 平坦
   * 1 -> 上り
   * 2 -> 下り
  */

  float filteredGyroX = IMU.gyrX();
  if (filteredGyroX >= 5){
    return 1;
  } else if (filteredGyroX <= 5){
    return 2;
  } else return 0;
  // Note: 基板で実装する向きが決まらんことにはXYを判断できへん 加速度を使えば「登り始め」「下り始め」をより早く感知できるかも?
}

bool Gyro::seesaw(){
  /* シーソーの傾き始める瞬間を加速度を用いて感知するよ
   * true -> シーソーが傾いた(加速度が大きい)
   * false -> まだ登り坂(加速度が小さい)
  */

  if (abs(accX) > startingTilt){
    return true;
  } else return false;
  // Note: 上前半に同じく
}

void Gyro::UpdateGyro(){
  // 生の値を取得 内部である程度補正はされてるけどね
  gyrX = IMU.gyrX();
  gyrY = IMU.gyrY();
  gyrZ = IMU.gyrZ();
  accX = IMU.accX();
  accY = IMU.accY();
  accZ = IMU.accZ();

  MadgwickFilter.updateIMU(gyrX, gyrY, gyrZ, accX, accY, accZ);

  filteredGyroX = MadgwickFilter.getRoll();
  filteredGyroY = MadgwickFilter.getPitch();
  filteredGyroZ = MadgwickFilter.getYaw();
}