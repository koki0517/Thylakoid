#include "LoadCell.h"

LoadCell::LoadCell(){
  // もしかしたらいらんのかも
  for (int8_t i=0; i < 3; i++){
    pinMode(pins[i],arduino::INPUT);
  }
  updateEEPROM();
}

bool LoadCell::isPressed(){
  int left = analogRead(LEFT_LoadCell);
  int right = analogRead(RIGHT_LoadCell);
  if (left + right >= isPressedvalue){
    return true;
  } else {
    return false;
  }
}

int16_t LoadCell::position_pressed(int *resultant_force,int *position){
  /*
  力のモーメントでバンパーのどこを押されたのか計算 まだ机上の空論しか実装してない
  座標        中心
  -1000-------0---------1000->x
    |￣￣￣￣￣￣￣￣￣￣￣￣|
    |____________________|
      ^               ^
      | LEFT          | RIGHT
  */
  int left = analogRead(LEFT_LoadCell);
  int right = analogRead(RIGHT_LoadCell);
  *resultant_force = left + right;
  // 押された中心が2つのロードセルの間のとき
  *position = (left / (left + right)) * 1000 - 500;
  // ロードセルの外側を押されたとき、つまり外分の式が適用されるときにロードセルの値がどうなるかわからない
}

void LoadCell::updateEEPROM(){
  EEPROM.get(EEP_LoadCell_isPressed,isPressedvalue);
}
