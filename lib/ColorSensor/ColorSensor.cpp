#include "ColorSensor.h"

ColorSensor::ColorSensor(TwoWire *theWire) : theWire(theWire), color(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X) {}

bool ColorSensor::init(){
  updateEEPROM();
  bool success = color.begin(0x29,theWire);
  return success;
}

void ColorSensor::rgb(uint16_t* red,uint16_t* green,uint16_t* blue){
  uint16_t r, g, b, c;
  color.getRawData(&r, &g, &b, &c);
  *red = r;
  *green = g;
  *blue = b;
}

void ColorSensor::hsv(HSV *hsv){
  uint16_t r, g, b, c;
  color.getRawData(&r, &g, &b, &c);
  
  uint16_t maxRGB = max(r,max(g,b));
  uint16_t minRGB = min(r,min(g,b));
  uint16_t diff = maxRGB - minRGB;

  // Hue
  if (maxRGB == minRGB){
    hsv->Hue = 0;
  } else if (maxRGB == r){
    hsv->Hue = 60 * ((g - b) / diff);
  } else if (maxRGB == g){
    hsv->Hue = 60 * ((b - r) / diff);
  } else if (maxRGB == b){
    hsv->Hue = 60 * ((r - g) / diff);
  }
  if (hsv->Hue < 0) hsv->Hue += 360;

  // Sqturation
  if (maxRGB == 0){
    hsv->Saturation = 0;
  } else {
    hsv->Saturation = diff / maxRGB * 100;
  }

  // Value(Brightness)
  hsv->Value = maxRGB;
}

uint8_t ColorSensor::colorHSV(){
  HSV tmphsv; 
  hsv(&tmphsv);
  if (HueGreenMin < tmphsv.Hue && tmphsv.Hue < HueGreenMax && tmphsv.Saturation > SqturationGreenMin && tmphsv.Value > ValueGreenMin){
    return GREEN;
  } else if (HueRedMin < tmphsv.Hue && tmphsv.Hue < HueRedMax && tmphsv.Saturation > SqturationRedMin && tmphsv.Value > ValueRedMin){
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
