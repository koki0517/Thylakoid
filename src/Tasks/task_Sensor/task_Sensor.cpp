#include "task_Sensor.h"

void task_Sensor(void *pvParameters) {
  /* サブなタスク
   * 一定の制御周期を保証したいジャイロのためにある
   * ここで処理したデータはキューに入れる
   * I2C系のセンサも処理する
   * 基本的に常時実行すると思う知らんけど
   */

  TickType_t xLastWakeTime;
  BaseType_t hasGotGyroMutex, hasGotI2C0Mutex, hasGotI2C1Mutex;
  filteredGyroXYZ gyroXYZ_2;
  Color color;

  while (1) {
    xLastWakeTime = xTaskGetTickCount(); // 現在の時刻を取得

    // ジャイロセンサ(ICM42688)の更新
    // xSemaphoreTake(mutexGyro, portMAX_DELAY );
    gyro.UpdateGyro(&gyroXYZ_2); // 更新‼
    // キューで送り付ける
    xQueueOverwrite(QueueGyro, &gyroXYZ_2);
    // xSemaphoreGive(mutexGyro);

    // I2C系のセンサを更新 ミューテックスをゲットでない場合もあり
    hasGotI2C0Mutex = xSemaphoreTake(mutexI2C0, 0);
    hasGotI2C1Mutex = xSemaphoreTake(mutexI2C1, 0);
    if (hasGotI2C0Mutex == pdPASS && hasGotI2C1Mutex == pdPASS){
      // カラーセンサ
      color.LEFT = colorLeft.colorHSV();
      color.RIGHT = colorRight.colorHSV();
      xQueueOverwrite(queueColor, &color);

      // 床のToFセンサ
      if (floortof.findProtrusion(ToF_FLOOR_LEFT)) {
      }
      if (floortof.findProtrusion(ToF_FLOOR_RIGHT)) {
      }
      xSemaphoreGive(mutexI2C0);
      xSemaphoreGive(mutexI2C1);
    } else {
      // どちらか片方のミューテックスがゲットできなかった場合 もう片方を開放する
      if (hasGotI2C0Mutex == pdPASS) {
        xSemaphoreGive(mutexI2C0);
      }
      if (hasGotI2C1Mutex == pdPASS) {
        xSemaphoreGive(mutexI2C1);
      }
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); // 制御周期調整弁君
  }
}