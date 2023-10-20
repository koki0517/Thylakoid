#pragma once

/* ジャイロセンサICM42688Pを使うためのクラスだよ
    ・角度はMadgwickフィルタで補正
    ・このクラス内では排他制御を実装していないのでタスク側で作る必要あり
    ・シーソーの傾き始める加速度はEEPROMのEEPADDR_STARTINGTILTに保存されている
    ・外部からEEPROMを変更した場合はupdateEEPROM()で内部の変数を更新する
*/

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <ICM42688.h>
#include <MadgwickAHRS.h>
#include "../EEPROM/EEPROM_Address.h"

typedef struct {
  // 補正された加速度が入る
  float X;
  float Y;
  float Z;
  uint8_t hill;
}filteredGyroXYZ;

filteredGyroXYZ gyroXYZ_1;
filteredGyroXYZ gyroXYZ_2;
filteredGyroXYZ gyroXYZ_3;

class Gyro {
public:
  int init(int Madgwickfrequency); // こいつだけintだから注意 初期化に成功すると正の値、失敗すると負の値が返ってくる
  void get_raw(float* rawGyroX,float* rawGyroY,float* rawGyroZ, float* rawAccX,float* rawAccY,float* rawAccZ);
  bool seesaw();
  void updateEEPROM();
  void UpdateGyro();
private:
  float accX, accY, accZ; // 生の加速度が入る
  float gyrX, gyrY,gyrZ; // 生の角度が入る
  float startingTilt; // EEPROM内に保存された値が入るZOY
};