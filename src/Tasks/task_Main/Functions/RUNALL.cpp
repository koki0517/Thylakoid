#include "RUNALL.h"

void runAll(bool startTaskUI){
  filteredGyroXYZ gyroXYZ_1;
  LineSensorData lineData;
  Color color;

  // UIタスクを起動
  if (startTaskUI){
    vTaskResume(taskUI);
  }

  while (1){
    line.readAll(&lineData); // ラインセンサのデータを取得
    xQueueReceive(queueGyro, &gyroXYZ_1, pdMS_TO_TICKS(5)); // ジャイロのデータを取得
    xQueueReceive(queueColor, &color, pdMS_TO_TICKS(5)); // カラーセンサのデータの取得

    /* 緑色マーカー */
    if (color.LEFT == GREEN){
      if (color.RIGHT == GREEN){
        foundGreen(whichColorSensor::BOTH);
      } else foundGreen(whichColorSensor::LEFT);
    } else if (color.RIGHT == GREEN){
      if (color.LEFT == GREEN){
        foundGreen(whichColorSensor::BOTH);
      } else foundGreen(whichColorSensor::RIGHT);
    }
  }
}

/* ToDo: イベントが発生したことをUIに通知するキューを作る */