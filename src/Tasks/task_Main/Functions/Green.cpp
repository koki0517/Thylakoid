#include "Green.h"

static void uTurn();

uint8_t foundGreen(whichColorSensor whichColor){
  
  // ここに緑が見つかったかどうかの処理を書く
  if (whichColor == whichColorSensor::BOTH){
    /* Uターン */
    uTurn();
  }
  return 0;
}

static void uTurn(){
  /* 柱にぶつかるかな?ドキドキ?! Uターン */
  Events uTurn = Events::GREEN_U_TURN;
  xQueueOverwrite(queueEventsHappened, &uTurn);
}

/* 反対側に緑マーカーがないかの確認 */