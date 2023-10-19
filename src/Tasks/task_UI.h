#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../Tools/rtosTools.h"
#include "../Tools/devices.h"

Display display;

void task_UI(void*){
  /*UI系の処理
  所詮UIなので30Hzとかでも問題ない
  */
  while (1) {
  }
}