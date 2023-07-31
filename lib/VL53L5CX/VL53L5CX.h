#pragma once

extern int16_t libar[64*3];

class VL53L5CX {
public:
  VL53L5CX();
  bool init();
  void get_lidar();
  bool stop();
};