#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>

extern int16_t libar[64*3];

VL53L5CX::VL53L5CX(){
  Serial1.begin(115200);
  while (!Serial1); // シリアルの名前はちゃんと変えようね!Teensy君いっぱいシリアル持ってんだから
}

bool VL53L5CX::init(){
  while (Serial.available())Serial1.read(); // 受信バッファのお掃除
  Serial1.write(0x01);
  bool result = true;
  unsigned long start_time = millis();
  while (!Serial.available()){ // データ第一弾が届くまで待つのだ
    ::vTaskDelay(pdMS_TO_TICKS(100)); // タイムアウト的な機能で100msだけ待ってあげる
    result = false;
    break;
  } 
  return result;
}

bool VL53L5CX::stop(){
  Serial1.write(0x04);
}

void VL53L5CX::get_lidar(){
  Serial1.read();
}