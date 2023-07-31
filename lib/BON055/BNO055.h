#pragma once

class BNO055 {
public:
  BNO055();
  bool init();
  void get_xyz(float* bno_x,float* bno_y,float* bno_z);
};