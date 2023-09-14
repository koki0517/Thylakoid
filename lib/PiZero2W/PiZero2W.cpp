#include "VL53L5CX.h"

PiZero2W::PiZero2W(){
  Serial1.begin(115200); // もっと上げてもいいかも?
  while (!Serial1); // シリアルの名前はちゃんと変えようね!Teensy君いっぱいシリアル持ってんだから
}

bool PiZero2W::init(){
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

bool PiZero2W::stop(){
  Serial1.write(0x04); // ストップ信号
}

void PiZero2W::get_lidar(){
  Serial1.read();
}
