#include "task_Main.h"

void task_Main(void *pvParameters) {
  /*メインタスク
    制御周期短めで動いてほしい
  */

  UBaseType_t uxNumberOfItems;
  uint8_t startOptions;

  // LOPを挟んだ場合の敗戦処理
  uxNumberOfItems = uxQueueMessagesWaiting(queueMainTaskStartOptions);
  if (uxNumberOfItems != 0) {
    xQueueReceive(queueMainTaskStartOptions, &startOptions, 0); // 取得には成功するはず

    switch (startOptions){
    case 1:
      /* 競技再開 */
      runAll();
      break;
    default:
      break;
    }
  }

  while (1) {
    display.drawPhoto(&Photo.HOME, 0, 0); // ホーム画面を表示
    while (!display.isTouch()) {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
    TS_Point tsPoint = display.getTSPoint();
  }
}