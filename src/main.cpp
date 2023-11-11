/* 
 * これはメインなプログラムだよ!
 * FreeRTOSで動くよ！
*/

#include "./Tasks/task_UI/task_UI.h"
#include "./Tasks/task_Sensor/task_Sensor.h"
#include "./Tasks/task_Main/task_Main.h"

FLASHMEM __attribute__((noinline)) void setup() {
  Serial.begin(115200); // これから始めなきゃArduinoって感じがしないよね こだわり

  initDevices(); // 数多のセンサーその他もろもろの初期化 初期化できないデバイスがあったらそれをシリアルに吐露し続けるよ。永遠にね、

  // RTOSの設定
  xCreateRTOStools(); // キューとかセマフォをつくるよ

  ::xTaskCreate(task_Main, "task_Main", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task_Sensor, "task_Sensor", 8192, nullptr, 2, nullptr);
  ::xTaskCreate(task_UI, "task_UI", 8192, nullptr, 2, nullptr);
  ::vTaskStartScheduler();
  // ここの下には何も書くな!!バグの元!!
}

void loop(){} // nothing to do is here