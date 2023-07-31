#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <Wire.h>
#include "TCS34725.h"

const int8_t LED_pin = 0;
const int8_t SDA;
const int8_t SCL;

TCS34725::init(uint8_t addr = 0x29, TwoWire *theWire = &Wire){
  pinMode(LED_pin,arduino::OUTPUT);
  digitalWrite(LED_pin,arduino::HIGH);
}

TCS34725::rgb(int16_t* r,int16_t* g,int16_t* b){}

TCS34725::color(int8_t color_code){}