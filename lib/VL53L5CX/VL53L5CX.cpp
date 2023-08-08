/* 基本的にRP2040が処理したVL53L5CXの値をUARTで受け取るだけ
 * 受信バッファのサイズは変えないつもり
 * おなかすいた
 */

#include "VL53L5CX.h"

extern int16_t libar[64*3];

VL53L5CX::VL53L5CX(){
  Serial1.begin(115200); // もっと上げてもいいかも?
  while (!Serial1); // シリアルの名前はちゃんと変えようね!Teensy君いっぱいシリアル持ってんだから
}

bool VL53L5CX::init(){
  while (Serial1.available())Serial1.read(); // 受信バッファのお掃除
  Serial1.write(0x01); // スタート信号
  bool result = true;
  unsigned long start_time = millis();
  while (!Serial1.available()){ // データ第一弾が届くまで待つのだ
    ::vTaskDelay(pdMS_TO_TICKS(100)); // タイムアウト的な機能で100msだけ待ってあげる
    result = false;
    break;
  }
  return result;
}

bool VL53L5CX::stop(){
  Serial1.write(0x04); // ストップ信号
}

void VL53L5CX::get_lidar(){
  Serial1.read();
}
