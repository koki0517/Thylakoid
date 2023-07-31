#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h> // 何故か入れないとエラーになる

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "BNO055.h"

extern bool loss_display = false;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire1);

BNO055::BNO055();

bool BNO055::init(){
  bool isSuccess = bno.begin();
  bno.setExtCrystalUse(true);
  delay(500);
  return isSuccess;
}

void BNO055::get_xyz(float* bno_x,float* bno_y,float* bno_z){
  /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
  
  *bno_x = (float)event.orientation.x;
  *bno_y = (float)event.orientation.y;
  *bno_z = (float)event.orientation.z;

  // Serial.print("Orientation: ");
  // Serial.print(bno_x);
  // Serial.print(" ");
  // Serial.print(bno_y);
  // Serial.print(" ");
  // Serial.print(bno_z);
  // Serial.println();

  ::vTaskDelay(pdMS_TO_TICKS(20));
}