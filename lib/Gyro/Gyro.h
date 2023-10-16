#pragma once

/* ジャイロセンサICM42688Pを使うためのクラスだよ
    角度はMadgwickフィルタで補正する
    UpdateGyro()で常に値を更新しながら他のメソッドで値を読むことを想定している
    このクラス内では排他制御を実装していないのでタスク側で作る必要あり
    シーソーの傾き始める加速度はEEPROMのEEPADDR_STARTINGTILTに保存されている
    外部から変更した場合はupdateEEPROM()で更新する
*/

#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <ICM42688.h>
#include <MadgwickAHRS.h>
#include "../EEPROM/EEPROM_Address.h"

class Gyro {
public:
  int init(); // こいつだけintだから注意 初期化に成功すると正の値、失敗すると負の値が返ってくる
  void get_xyz(float* gyroX,float* gyroY,float* gyroZ);
  void get_raw(float* rawGyroX,float* rawGyroY,float* rawGyroZ, float* rawAccX,float* rawAccY,float* rawAccZ);
  int8_t hill();
  bool seesaw();
  void updateEEPROM();
  void UpdateGyro();
private:
  float accX, accY, accZ; // 生の加速度が入る
  float gyrX, gyroY,gyrZ; // 生の角度が入る
  float filteredGyroX, filteredGyroY, filteredGyroZ; // 補正された加速度が入る
  float startingTilt; // EEPROM内に保存された値が入るZOY
};