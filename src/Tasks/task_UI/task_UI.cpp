#include "task_UI.h"

void task_UI(void *pvParameters){
  /* 走行中のUI系の処理
   * 所詮UIなので30Hzとかでも問題ない と言うとでも思ったか 30Hzも出んわ
   * UIの画像についてはUI-Imagesブランチを参照
   * MainTaskによって生えてきたり消えたりする
   */

  filteredGyroXYZ gyroXYZ_3;

  while (1) {
    // ジャイロのデータを取得 あくまでもデータは消さない
    xQueuePeek(queueGyro, &gyroXYZ_3, 0);

    /* LOPの確認 */
    if (digitalRead(LOP_pin) == arduino::HIGH){
      /* ★☆★☆ 進行停止確定 乙乙乙 ☆★☆★☆ */
      LOP();
    }
  }
}