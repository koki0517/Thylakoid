#include "ColorSensor.h"

ColorSensor::ColorSensor(TwoWire *theWire) : theWire(theWire), color(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X){}

bool ColorSensor::init(){
  updateEEPROM();
  return color.begin(0x29,theWire);
}

void ColorSensor::rgb(uint16_t* red,uint16_t* green,uint16_t* blue){
  uint16_t r, g, b, c;
  color.getRawData(&r, &g, &b, &c);
  *red = r;
  *green = g;
  *blue = b;
}

void ColorSensor::hsv(uint16_t *h,uint16_t *s,uint16_t *v){
  uint16_t r, g, b, c;
  color.getRawData(&r, &g, &b, &c);
  
  uint16_t maxRGB = max(r,max(g,b));
  uint16_t minRGB = min(r,min(g,b));
  diff = maxRGB - minRGB;

  // Hue
  if (maxRGB == minRGB){
    *h = 0;
  } else if (maxRGB == r){
    *h = 60 * ((g - b) / diff)
  } else if (maxRGB == g){
    *h = 60 * ((b - r) / diff)
  } else if (maxRGB == b){
    *h = 60 * ((r - g) / diff)
  }
  if (*h < 0) *h += 360;

  // Sqturation
  if (maxRGB == 0){
    *s = 0;
  } else {
    *s = diff / maxRGB * 100;
  }

  // Value(Brightness)
  *v = maxRGB;
}

uint8_t ColorSensor::color(){
  uint16_t h, s, v;
  hsv(&h, &s, &v);
  if (HueGreenMin < h && h < HueGreenMax && s > SqturationGreenMin && v > ValueGreenMin){
    return GREEN;
  } else if (HueRedMin < h && h < HueRedMax && s > SqturationRedMin && v > ValueRedMin){
    return RED;
  } else return OTHERS;
}

void ColorSensor::updateEEPROM(){
  EEPROM.get(EEP_HueGreenMin, HueGreenMin);
  EEPROM.get(EEP_HueGreenMax, HueGreenMax);
  EEPROM.get(EEP_SqturationGreenMin, SqturationGreenMin);
  EEPROM.get(EEP_ValueGreenMin, ValueGreenMin);

  EEPROM.get(EEP_HueRedMin, HueRedMin);
  EEPROM.get(EEP_HueRedMax, HueRedMax);
  EEPROM.get(EEP_SqturationRedMin, SqturationRedMin);
  EEPROM.get(EEP_SqturationRedMin, ValueRedMin);
}
