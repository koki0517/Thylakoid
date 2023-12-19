#include "LOP.h"

void LOP(){
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount(); // 現在の時刻を取得
  xSemaphoreTake(mutexI2C, portMAX_DELAY); // I2Cのセマフォをぶん取る
  ::vTaskDelete(taskMain); // MAINタスクを削除

  vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000)); // 1000ms待つ
  if (digitalRead(LOP_pin) == arduino::HIGH){ // まだボタンが押されているか確認
    // リタイヤ
    while (digitalRead(LOP_pin) == arduino::HIGH); // ボタンが離れるまで待つ
    ::xTaskCreate(task_Main, "task_Main", 8192, nullptr, 2, &taskMain);
  } else {
    // 進行停止
    display.drawPhoto(&Photo.LOP, 0, 0);
    while (1){
      if (digitalRead(LOP_pin) == arduino::HIGH){
        // 再開
        while (digitalRead(LOP_pin) == arduino::HIGH); // ボタンが離れるまで待つ
        uint8_t option = 1;
        xQueueOverwrite(queueMainTaskStartOptions, &option);
        ::xTaskCreate(task_Main, "task_Main", 8192, nullptr, 2, &taskMain);
        break;
      } else {
        TS_Point tsPoint = display.getTSPoint();
        if (tsPoint.x != -1){
          uint8_t numButton = display.identifyButton(&tsPoint, display.ButtonLOP, sizeof(display.ButtonLOP)/sizeof(display.ButtonLOP[0]));
          if (numButton == (uint8_t)numButtonLOP::LOP_FINISH) {
            // リタイヤ
            while (display.isTouch()); // ボタンが離れるまで待つ
            ::xTaskCreate(task_Main, "task_Main", 8192, nullptr, 2, &taskMain);
            break;
          } // その他のボタン
        } // タッチされてない
      }
    }
  }
}