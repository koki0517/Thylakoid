#include "task_Sensor.h"

void task_Sensor(void*) {
  /*サブなタスク
    一定の制御周期を保証したいジャイロのためにある
    I2C系のセンサも処理する
  */
  while (1) {
    // ジャイロセンサ(ICM42688)の更新 補正されたジャイロの値はgyroXYZ_2構造体に入ってる
    xSemaphoreTake(mutexGyro, portMAX_DELAY );
    gyro.UpdateGyro(); // 更新‼
    // キューで送り付ける
    xQueueOverwrite(QueueGyro,&gyroXYZ_2);
    xSemaphoreGive(mutexGyro);
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}