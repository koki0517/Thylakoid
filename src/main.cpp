#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "BNO055.h"
#include "VL53L5CX.h"
#include "ILI9341.h"

BNO055 gyro;
SSD1306 display;
VL53L5CX lidar;

int8_t refrector_pin[] = {}; // 最低でも4個いるよね

static void task1(void*) {
  while (1) {
  }
}

static void task2(void*) {
  while (1) {
  }
}

FLASHMEM __attribute__((noinline)) void setup() {
  Serial.begin(115200);

  if (!gyro.init()){
    // BNO055が見つかんなかったYO 
  }

  if (!display.init()){
    // SSD1306が見つかんなかったYO
  }

  if (!lidar.init()){
    // xiaoが見つかんなかったYO
  }

  // RTOSの設定
  ::xTaskCreate(task1, "task1", 2048, nullptr, 2, nullptr);
  ::xTaskCreate(task2, "task2", 2048, nullptr, 2, nullptr);
  ::vTaskStartScheduler();
}

void loop(){} // nothing to do is here