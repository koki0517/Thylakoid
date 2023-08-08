#include "ColorSensor.h"

const int8_t LED_pin = 0;
const int8_t SDA;
const int8_t SCL;

ColorSensor::init(uint8_t addr = 0x29, TwoWire *theWire = &Wire){
  pinMode(LED_pin,arduino::OUTPUT);
  digitalWrite(LED_pin,arduino::HIGH);
}

ColorSensor::rgb(int16_t* r,int16_t* g,int16_t* b){}

ColorSensor::hsv(int16_t* h,int16_t* s,int16_t* v){}

ColorSensor::color(int8_t color_code){}