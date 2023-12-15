#include "task_UI.h"
#include "../task_Main/task_Main.h"

void task_UI(void *pvParameters){
  /* 走行中のUI系の処理
   * 所詮UIなので30Hzとかでも問題ない と言うとでも思ったか 30Hzも出んわ
   * UIの画像についてはUI-Imagesブランチを参照
   * MainTaskによって生えてきたり消えたりする
   */

  filteredGyroXYZ gyroXYZ_3;
  TickType_t xLastWakeTime;
  uint8_t LOP_pin = 0; // ToDo: ピン番号を確認する
  pinMode(LOP_pin, arduino::INPUT_PULLUP);

  while (1) {
    // ジャイロのデータを取得 あくまでもデータは消さない
    xQueuePeek(queueGyro, &gyroXYZ_3, 0);

    /* LOPの確認 */
    if (digitalRead(LOP_pin) == arduino::HIGH){
      /* ★☆★☆ 進行停止確定 乙乙乙 ☆★☆★☆ */
      xLastWakeTime = xTaskGetTickCount(); // 現在の時刻を取得
      xSemaphoreTake(SemaphoreI2C, portMAX_DELAY); // I2Cのセマフォをぶん取る
      ::vTaskDelete(taskMain); // MAINタスクを削除

      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500)); // 500ms待つ
      if (digitalRead(LOP_pin) == arduino::HIGH){ // まだボタンが押されているか確認
        // リタイヤ
        while (digitalRead(LOP_pin) == arduino::HIGH); // ボタンが離れるまで待つ
        ::xTaskCreate(task_Main, "task_Main", 8192, nullptr, 2, &taskMain);
        ::vTaskSuspend(NULL);
      } else {
        // 進行停止
        display.drawPhoto(&Photo.LOP, 0, 0);
      }
    }
  }
}