#include "task_UI.h"

void task_UI(void*){
  /*UI系の処理
    所詮UIなので30Hzとかでも問題ない
    UIの画像についてはUI-Imagesブランチを参照
  */

  while (1) {
    // ジャイロのデータを取得
    xQueueReceive(QueueGyro, &gyroXYZ_3, 0);
  }
}