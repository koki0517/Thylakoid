#include "task_Main.h"

void task_Main(void *pvParameters) {
  /*メインタスク
    制御周期短めで動いてほしい
  */

  filteredGyroXYZ gyroXYZ_1;

  while (1) {
    // ジャイロのデータを取得
    xQueueReceive(queueGyro, &gyroXYZ_1, 0);
  }
}