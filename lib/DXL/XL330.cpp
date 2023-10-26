#include "XL330.h"

bool XL330::init(){
  dxl.begin(1000000); // 1MHz!! 速い!!
  dxl.setPortProtocolVersion(2.0);
  for (uint8_t DXL_ID = 1; DXL_ID < 5; DXL_ID++){
    dxl.ping(DXL_ID);
    dxl.torqueOff(DXL_ID);
    dxl.setOperatingMode(DXL_ID, OP_VELOCITY);
    dxl.torqueOn(DXL_ID);
  }
  return true;
}

bool XL330::ledOn(uint8_t ID){
  return dxl.ledOn(ID);
}

bool XL330::ledOff(uint8_t ID){
  return dxl.ledOff(ID);
}

bool XL330::torqueOn(uint8_t ID){
  return dxl.torqueOn(ID);
}

bool XL330::torqueOff(uint8_t ID){
  return dxl.torqueOff(ID);
}

uint8_t XL330::torqueOnAll(){
  bool result = SUCCESS;
  for (uint8_t DXL_ID = 1; DXL_ID < 5; DXL_ID++){
    if (!dxl.torqueOn(DXL_ID)){
      result = FAIL_COMMUNICATION;
    }
  }
  return result;
}

uint8_t XL330::torqueOffAll(){
  bool result = SUCCESS;
  for (uint8_t DXL_ID = 1; DXL_ID < 5; DXL_ID++){
    if (!dxl.torqueOff(DXL_ID)){
      result = FAIL_COMMUNICATION;
    }
  }
  return result;
}

bool XL330::drive(float RPM){
  if (abs(RPM) > MAXRPM) return false;
  /* すべてのサーボを同じ速度でかき回すよ */
  if (!dxl.setGoalVelocity(1, RPM, UNIT_RPM)) return false;
  if (!dxl.setGoalVelocity(2, RPM, UNIT_RPM)) return false;
  if (!dxl.setGoalVelocity(3, RPM, UNIT_RPM)) return false;
  if (!dxl.setGoalVelocity(4, RPM, UNIT_RPM)) return false;
  return true;
}

uint8_t XL330::drive_for_degrees(float RPM, float degrees, uint8_t stop, unsigned int timeout_ms, bool wait){
  if (abs(RPM) > MAXRPM) return SUCCESS;

  if (stop == NOT){ // not stop ------------------------------------------------------------
    if (degrees <= 0){ // ΔΘはunsigned floatに丸める
      RPM *= -1;
      degrees *= -1;
    }
    float startPosition[4];
    for (uint8_t i=0; i < 5; i++){
      startPosition[i] = dxl.getPresentPosition(i, UNIT_DEGREE);
    }
    drive(RPM);

    unsigned long int start_time_ms = millis();
    while (1){
      for (uint8_t i=1; i < 5; i++){
        if (degrees <= abs(startPosition[i] - dxl.getPresentPosition(i, UNIT_DEGREE))) return SUCCESS;
      }
      if ((millis()-start_time_ms) >= timeout_ms) return TIMEOUT;
    }
  } else if (stop == BRAKE){ // BRAKE ------------------------------------------------------------

  } else { // POSITION_MODE ------------------------------------------------------------
    /* 速度を0にすると最高速度で動いちゃう(仕様) */
    if (RPM <= 0){ // RPMはunsigned floatに丸める
      RPM *= -1;
      degrees *= -1;
    }
    float start_position[4];
    for (uint8_t i=1; i < 5; i++){
      changeMode(i, POSITION);
      dxl.writeControlTableItem(PROFILE_VELOCITY, i, 100); // 速度はRAW
      start_position[i] = dxl.getPresentPosition(i, UNIT_DEGREE);
    }
    for (uint8_t i=1; i < 5; i++){
      dxl.setGoalPosition(i, degrees+start_position[i], UNIT_DEGREE);
    }

    if (!wait) return SUCCESS;

    unsigned long int start_time_ms = millis();
    while (1){
      for (uint8_t i=1; i < 5; i++){
        if (degrees <= abs(start_position[i] - dxl.getPresentPosition(i, UNIT_DEGREE))) return SUCCESS;
      }
      if ((millis()-start_time_ms) >= timeout_ms) return TIMEOUT;
    }
  }
}

bool XL330::run(uint8_t ID, float RPM){
  if (abs(RPM) > MAXRPM) return false;
  return dxl.setGoalVelocity(ID, RPM, UNIT_RPM);
}

bool XL330::run_for_degrees(uint8_t ID, float rawSpeed, float degrees, unsigned int timeout_ms, bool wait){
  // ToDo: 最大値を定義する
  if (rawSpeed < 0) rawSpeed *= -1;
  dxl.writeControlTableItem(PROFILE_VELOCITY, ID, rawSpeed);
  float start_position = dxl.getPresentPosition(ID, UNIT_DEGREE);
  dxl.setGoalPosition(ID, degrees + start_position, UNIT_DEGREE);

  if (!wait) return true;

  unsigned long int start_time_ms = millis();
  while (1){
    if (degrees <= abs(start_position - dxl.getPresentPosition(ID, UNIT_DEGREE))) return true;
    if ((millis()-start_time_ms) >= timeout_ms) return false;
  }
}

float XL330::position(uint8_t ID){
  return dxl.getPresentPosition(ID, UNIT_DEGREE);
}

bool XL330::changeMode(uint8_t ID, uint8_t new_mode){
  if (new_mode == mode){
    return true;
  }

  dxl.torqueOff(ID);
  if (new_mode == VELOCITY){
    dxl.setOperatingMode(ID, OP_VELOCITY);
  } else {
    dxl.setOperatingMode(ID, OP_EXTENDED_POSITION);
  }
  dxl.torqueOn(ID);

  return true;
}