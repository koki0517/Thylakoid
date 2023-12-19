#include "task_Sensor.h"

void task_Sensor(void*) {
  /* サブなタスク
   * 一定の制御周期を保証したいジャイロのためにある
   * ここで処理したデータはキューに入れる
   * I2C系のセンサも処理する
   * 基本的に常時実行すると思う知らんけど
   */

  TickType_t xLastWakeTime;
  BaseType_t hasGotmutexI2C;
  // BaseType_t hasGotmutexGyro;
  filteredGyroXYZ gyroXYZ_2;
  Color color;

  while (1) {
    xLastWakeTime = xTaskGetTickCount(); // 現在の時刻を取得

    // ジャイロセンサ(ICM42688)の更新
    // xSemaphoreTake(mutexGyro, portMAX_DELAY );
    gyro.UpdateGyro(&gyroXYZ_2); // 更新‼
    // キューで送り付ける
    xQueueOverwrite(queueGyro, &gyroXYZ_2);
    // xSemaphoreGive(mutexGyro);

    // I2C系のセンサを更新 セマフォをゲットでない場合もあり
    hasGotmutexI2C = xSemaphoreTake(mutexI2C, 0);
    if (hasGotmutexI2C == pdPASS){
      // カラーセンサ
      color.LEFT = colorLeft.colorHSV();
      color.RIGHT = colorRight.colorHSV();
      xQueueOverwrite(queueColor, &color);

      // 床のToFセンサ
      if (floortof.findProtrusion(ToF_FLOOR_LEFT)) {
      }
      if (floortof.findProtrusion(ToF_FLOOR_RIGHT)) {
      }
      xSemaphoreGive(mutexI2C);
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); // 制御周期調整弁君
  }
}