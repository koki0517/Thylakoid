#include "ColorSensor.h"

const int8_t LED_pin = 0;
const int8_t SDA;
const int8_t SCL;

// インスタンス関数でWireを定義しようね

ColorSensor::init(TwoWire *theWire = &Wire, uint8_t addr = 0x29){
  pinMode(LED_pin,arduino::OUTPUT);
  digitalWrite(LED_pin,arduino::HIGH);
}

ColorSensor::rgb(int16_t* r,int16_t* g,int16_t* b){}

ColorSensor::hsv(int16_t* h,int16_t* s,int16_t* v){}

ColorSensor::color(int8_t color_code){}