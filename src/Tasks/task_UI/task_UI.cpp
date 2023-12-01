#include "task_UI.h"

void task_UI(void *pvParameters){
  /* UI系の処理
   * 所詮UIなので30Hzとかでも問題ない と言うとでも思ったか 30Hzも出んわ
   * UIの画像についてはUI-Imagesブランチを参照
   * MainTaskによって生えてきたり消えたりする
   */

  while (1) {
    // ジャイロのデータを取得
    xQueueReceive(QueueGyro, &gyroXYZ_3, 0);
  }
}