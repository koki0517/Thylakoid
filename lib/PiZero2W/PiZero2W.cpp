#include "PiZero2W.h"

PiZero2W::PiZero2W(){
  Serial7.begin(115200); // もっと上げてもいいかも?
  while (!Serial7); 
}

bool PiZero2W::init(){
  while (Serial7.available())Serial7.read(); // 受信バッファのお掃除
  Serial7.write(0x01); // スタート信号
  bool result = true;
  unsigned long start_time = millis();
  while (!Serial7.available()){ // データ第一弾が届くまで待つのだ
    ::vTaskDelay(pdMS_TO_TICKS(100)); // タイムアウト的な機能で100msだけ待ってあげる
    result = false;
    break;
  }
  return result;
}

bool PiZero2W::stop(){
  Serial7.write(0x04); // ストップ信号
  return true;
}

void PiZero2W::get_lidar(){
  Serial7.read();
}
