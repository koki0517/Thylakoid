/* 競技中にボタンが押されたときの処理
 * LoPの場合とリタイヤの場合がある
 */

#pragma once
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include "semphr.h"
#include "queue.h"

#include "../../task_Main/task_Main.h"

extern TaskHandle_t taskMain;
extern QueueHandle_t queueMainTaskStartOptions;
extern SemaphoreHandle_t mutexI2C;
extern uint8_t LOP_pin;
extern Display display;

void LOP();