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

void Gyro::get_raw(float* rawGyroX,float* rawGyroY,float* rawGyroZ, float* rawAccX,float* rawAccY,float* rawAccZ){
  *rawGyroX = gyrX;
  *rawGyroY = gyrY;
  *rawGyroZ = gyrZ;
  *rawAccX = accX;
  *rawAccY = accY;
  *rawAccZ = accZ;
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

void Gyro::UpdateGyro(filteredGyroXYZ *gyroXYZ){
  // 生の値を取得 内部である程度補正はされてるけどね
  gyrX = IMU.gyrX();
  gyrY = IMU.gyrY();
  gyrZ = IMU.gyrZ();
  accX = IMU.accX();
  accY = IMU.accY();
  accZ = IMU.accZ();

  MadgwickFilter.updateIMU(gyrX, gyrY, gyrZ, accX, accY, accZ);

  gyroXYZ->X = MadgwickFilter.getRoll();
  gyroXYZ->Y = MadgwickFilter.getPitch();
  gyroXYZ->Z = MadgwickFilter.getYaw();

  /* 普通にライントレースしてるときに使うよ
   * 0 -> 平坦
   * 1 -> 上り
   * 2 -> 下り
  */
  if (gyroXYZ->X > 5){
    gyroXYZ->hill = 1;
  } else if (gyroXYZ->X < -5){
    gyroXYZ->hill = 2;
  } else {
    gyroXYZ->hill = 0;
  }
  // Note: 基板で実装する向きが決まらんことにはXYを判断できへん 加速度を使えば「登り始め」「下り始め」をより早く感知できるかも?
  //       生の加速度がどのくらい信用できるかにもよるけど、
}