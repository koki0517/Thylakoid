#pragma once

class TCS34725 {
public:
  bool init(uint8_t addr = 0x29, TwoWire *theWire = &Wire);
  rgb(int16_t* r,int16_t* g,int16_t* b);
  color(int8_t color_code);
}
