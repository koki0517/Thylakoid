#include "task_UI.h"

extern QueueHandle_t queueGyro, queueColor;

void task_UI(void*){
  /* 走行中のUI系の処理
   * 所詮UIなので30Hzとかでも問題ない と言うとでも思ったか 30Hzも出んわ
   * UIの画像についてはUI-Imagesブランチを参照
   * MainTaskによって生えてきたり消えたりする
   */

  filteredGyroXYZ gyroXYZ_3;
  Color color;

  while (1) {
    xQueuePeek(queueGyro, &gyroXYZ_3, 0); // ジャイロのデータを取得 あくまでもデータは消さない
    xQueuePeek(queueColor, &color, 0); // カラーセンサのデータの取得

    /* 物理ボタンの確認 */
    if (digitalRead(display.LOP_pin) == arduino::HIGH){
      /* ★☆★☆ 進行停止確定 乙乙乙 ☆★☆★☆ */
      LOP();
    }
  }
}