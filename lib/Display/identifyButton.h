#pragma once
#include <XPT2046_Touchscreen.h>

typedef struct{
  uint8_t number; // ボタンの通し番号
  int16_t xStart; // ボタンの原点
  int16_t yStart;
  int16_t width; // ボタンの幅、高さ
  int16_t height;
} Button;

Button ButtonHome[3] = {
  {1,30,70,75,105}, // RUN
  {2,120,70,80,105}, // SENSOR
  {3,220,70,75,105} // MOTER
};

enum class numButtonHome{
  OTHERS,
  RUN,
  SENSOR,
  MOTER
};

Button ButtonBAR[7] = {
  {1,0,200,45,40}, // HOME
  {2,45,200,45,40}, // RUN
  {3,90,200,50,40}, // SENSOR
  {4,140,200,45,40}, // MOTER
  {5,185,200,45,40}, // GYRO
  {6,230,200,45,40}, // REFRECTOR
  {7,275,200,45,40} // COLOR
};

enum class numButtonBAR{
  OTHERS,
  HOME,
  RUN,
  SENSOR,
  MOTER,
  GYRO,
  REFRECTOR,
  COLOR
};

Button ButtonSensor[8] = {
  {1,0,0,45,35}, // RETURN
  {2,20,50,70,70}, // GYRO
  {3,90,50,70,70}, // REFRECTOR
  {4,160,50,70,70}, // COLOR
  {5,230,50,75,70}, // LOADCELL
  {6,20,125,70,70}, // FLOOR_TOF
  {7,90,125,70,70}, // WALL_TOF
  {8,160,125,70,70} // LIDAR
};

enum class numButtonSensor{
  OTHERS,
  RETURN,
  GYRO,
  REFRECTOR,
  COLOR,
  LOADCELL,
  FLOOR_TOF,
  WALL_TOF,
  LIDAR
};

Button ButtonLOP[1] = {
  {1,145,120,65,40} // FINISH
};

enum class numButtonLOP{
  OTHERS,
  LOP_FINISH
};

uint8_t identifyButton(TS_Point *tsPoint, Button *button[]){
  uint8_t buttonSize = sizeof(button) / sizeof(button[0]);
  for (uint8_t i = 0; i < buttonSize; i++){
    if (tsPoint->x >= button[i]->xStart && tsPoint->x < button[i]->xStart + button[i]->width - 1){ // X軸方向の判定
      if (tsPoint->y > button[i]->yStart && tsPoint->y < button[i]->yStart + button[i]->height - 1){ // Y軸方向の判定
        return button[i]->number;
      }
    }
  }
  return 0; // どれにも当てはまらない場合
}