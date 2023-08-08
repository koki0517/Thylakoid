#include "Gyro.h"

Gyro::Gyro();

bool Gyro::init(){
  bool isSuccess = bno.begin();

  return isSuccess;
}

void Gyro::get_xyz(float* bno_x,float* bno_y,float* bno_z){

}