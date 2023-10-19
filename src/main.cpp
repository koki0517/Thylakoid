/* 
  これはメインなプログラムだよ!
  FreeRTOSで動くよ！
*/

#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "./Tasks/task_UI.h"
#include "./Tasks/task_Sensor.h"
#include "./Tasks/task_Main.h"
#include "./Tools/devices.h"

FLASHMEM __attribute__((noinline)) void setup() {
  Serial.begin(115200);

  initDevices(); // 数多のセンサーその他もろもろの初期化

  // RTOSの設定
  xCreateRTOStools(); // キューとかセマフォをつくるよ

  ::xTaskCreate(task1, "task1", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task2, "task2", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task_UI, "task_UI", 8192, nullptr, 2, nullptr);
  ::vTaskStartScheduler();
  // ここの下には何も書くな!!バグる!!
}

void loop(){} // nothing to do is here