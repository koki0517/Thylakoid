#include "task_Main.h"

void task_Main(void *pvParameters) {
  /*メインタスク
    制御周期短めで動いてほしい
  */
  while (1) {
    // ジャイロのデータを取得
    xQueueReceive(QueueGyro, &gyroXYZ_1, 0);
  }
}