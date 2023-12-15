#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"

#include "../../task_Main/task_Main.h"

#include "../../../Tools/rtosTools.h"
#include "../../../Tools/devices.h"

#include "identifyButton.h"

void LOP(){
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount(); // 現在の時刻を取得
  xSemaphoreTake(SemaphoreI2C, portMAX_DELAY); // I2Cのセマフォをぶん取る
  ::vTaskDelete(taskMain); // MAINタスクを削除

  vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000)); // 1000ms待つ
  if (digitalRead(LOP_pin) == arduino::HIGH){ // まだボタンが押されているか確認
    // リタイヤ
    while (digitalRead(LOP_pin) == arduino::HIGH); // ボタンが離れるまで待つ
    ::xTaskCreate(task_Main, "task_Main", 8192, nullptr, 2, &taskMain);
    ::vTaskSuspend(NULL);
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
        ::vTaskSuspend(NULL);
      } else {
        TS_Point tsPoint = display.getTSPoint();
        if (tsPoint.x != -1){
          uint8_t numButton = identifyButton(&tsPoint, ButtonLOP);
          switch (numButton){
          case 1:
            // リタイヤ
            while (display.isTouch()); // ボタンが離れるまで待つ
            ::xTaskCreate(task_Main, "task_Main", 8192, nullptr, 2, &taskMain);
            ::vTaskSuspend(NULL);
            break;
          case 0:
            /* ボタンではない場所 */
            break;
          }
        } // タッチされてない
      }
    }
  }
}