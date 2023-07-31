#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>

#include <Adafruit_GFX.h>
#include "ILI9341.h"

ILI9341::ILI9341(){
  // EEPROMの読み出し 4284バイトもあるんだってすごいね
  for (int8_t i=0;i<=n;i++){
    // nにいい感じの数字を入れよう
  }
}

bool ILI9341::init(){
}

void ILI9341::clear(){
}

void ILI9341::home(){
  while (1){
    // なんか書けやbreak;すんぞ
  }
}