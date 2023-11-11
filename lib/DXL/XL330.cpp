#include "XL330.h"

bool XL330::init(){
  updateEEPROM();
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

/* ================================================================================================================================ */
uint8_t XL330::torqueOnAll(){
  bool result = SUCCESS;
  for (uint8_t DXL_ID = 1; DXL_ID < 5; DXL_ID++){
    if (!dxl.torqueOn(DXL_ID)){
      result = FAIL_COMMUNICATION;
    }
  }
  return result;
}

/* ================================================================================================================================ */
uint8_t XL330::torqueOffAll(){
  bool result = SUCCESS;
  for (uint8_t DXL_ID = 1; DXL_ID < 5; DXL_ID++){
    if (!dxl.torqueOff(DXL_ID)){
      result = FAIL_COMMUNICATION;
    }
  }
  return result;
}

/* ================================================================================================================================ */
bool XL330::drive(float RPM_LEFT, float RPM_RIGHT){
  if (abs(RPM_LEFT) > MAXRPM) return false;
  if (abs(RPM_RIGHT) > MAXRPM) return false;
  /* すべてのサーボを同じ速度でかき回すよ */
  if (!dxl.setGoalVelocity(1, RPM_LEFT, UNIT_RPM)) return false;
  if (!dxl.setGoalVelocity(2, RPM_RIGHT, UNIT_RPM)) return false;
  if (!dxl.setGoalVelocity(3, RPM_LEFT, UNIT_RPM)) return false;
  if (!dxl.setGoalVelocity(4, RPM_RIGHT, UNIT_RPM)) return false;
  return true;
}

/* ================================================================================================================================ */
uint8_t XL330::drive_for_degrees(float RPM_LEFT, float RPM_RIGHT, float degrees, uint8_t stop, unsigned int timeout_ms, bool wait){
  if (abs(RPM_LEFT) > MAXRPM) return SPEED_SIZE_ERR;
  if (abs(RPM_RIGHT) > MAXRPM) return SPEED_SIZE_ERR;

  if (stop == NOT){ // not stop ------------------------------------------------------------
    if (degrees <= 0){ // ΔΘはunsigned floatに丸める
      RPM_LEFT *= -1;
      RPM_RIGHT *= -1;
      degrees *= -1;
    }
    float startPosition[4];
    for (uint8_t i=1; i < 5; i++){
      startPosition[i-1] = dxl.getPresentPosition(i, UNIT_DEGREE);
    }
    drive(RPM_LEFT, RPM_RIGHT);

    unsigned long int start_time_ms = millis();
    while (1){ // どれかが目標角にたどり着くまで走る
      for (uint8_t i=1; i < 5; i++){
        if (3 >= degrees - abs(startPosition[i-1] - dxl.getPresentPosition(i, UNIT_DEGREE))) return SUCCESS;
      }
      if ((millis()-start_time_ms) >= timeout_ms) return TIMEOUT;
    }
  } else if (stop == BRAKE){ // BRAKE ------------------------------------------------------------
    if (degrees <= 0){ // ΔΘはunsigned floatに丸める
      RPM_LEFT *= -1;
      RPM_RIGHT *= -1;
      degrees *= -1;
    }
    float start_position[4],goal_position[4];
    for (uint8_t i=1; i < 5; i++){
      start_position[i-1] = dxl.getPresentPosition(i, UNIT_DEGREE);
      goal_position[i-1] = start_position[i-1] + degrees;
    }

    float error, last_error[4] = {0,0,0,0}, u,present_position[4];
    unsigned long int start_time_ms = millis();
    while (1){
      // PIDで回す
      for (uint8_t i=1; i < 5; i++){
        present_position[i-1] = dxl.getPresentPosition(i, UNIT_DEGREE);
        error = goal_position[i-1] - present_position[i-1];
        u = Kp_position * error + Ki_position * (error + last_error[i-1]) + Kd_position * (error - last_error[i-1]);
        if (i % 2 == 1){
          if (u >= RPM_LEFT) u = RPM_LEFT; // 最大値をMAXRPMに抑える
          if (u <= -1*RPM_LEFT) u = -1*RPM_LEFT; // 最小値を-MAXRPMに抑える
        } else {
          if (u >= RPM_RIGHT) u = RPM_RIGHT; // 最大値をMAXRPMに抑える
          if (u <= -1*RPM_RIGHT) u = -1*RPM_RIGHT; // 最小値を-MAXRPMに抑える
        }
        dxl.setGoalVelocity(i, u, UNIT_RPM);
        last_error[i-1] = error;

        if (3 >= degrees - abs(start_position[i-1] - present_position[i-1])) return SUCCESS; // どれかが目標角にたどり着くまで走る
      }
      if ((millis()-start_time_ms) >= timeout_ms) return TIMEOUT;
    }
  } else { // POSITION_MODE ------------------------------------------------------------
    /* 4つ全部同じように動かすことしかできないよ
     * そもそもこれを作ること自体ががナンセンス 
    */

    // ここではRPMは0以上のfloatな? degreesで方向を指定
    if (RPM_LEFT <= 0 || RPM_RIGHT <= 0 || RPM_LEFT == RPM_RIGHT) return SPEED_SIZE_ERR; // 速度を0にすると最高速度で動いちゃう(仕様)

    float start_position[4];
    float rawSpeed_LEFT = RPM_LEFT * 445 /101.85;

    for (uint8_t i=1; i < 5; i++){
      changeMode(i, POSITION);
      start_position[i-1] = dxl.getPresentPosition(i, UNIT_DEGREE);
      dxl.writeControlTableItem(PROFILE_VELOCITY, i, rawSpeed_LEFT); // 渡す速度はRAWで
      dxl.setGoalPosition(1, degrees+start_position[i-1], UNIT_DEGREE);
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

/* ================================================================================================================================ */
bool XL330::run(uint8_t ID, float RPM){
  if (abs(RPM) > MAXRPM) return false;
  return dxl.setGoalVelocity(ID, RPM, UNIT_RPM);
}

/* ================================================================================================================================ */
bool XL330::run_for_degrees(uint8_t ID, float RPM, float degrees, uint8_t stop, unsigned int timeout_ms, bool wait){
  if (abs(RPM) > MAXRPM) return false;
  float start_position = dxl.getPresentPosition(ID, UNIT_DEGREE);

  if (stop == BRAKE){
    float error, last_error = 0, u;
    float goal_position = start_position + degrees, present_position;
    while (1){
      present_position = dxl.getPresentPosition(ID, UNIT_DEGREE);
      error = goal_position - present_position;
      u = Kp_position * error + Ki_position * (error + last_error) + Kd_position * (error - last_error);
      if (u >= MAXRPM) u = 100; // 最大値をMAXRPMに抑える
      if (u <= -1*MAXRPM) u = -100; // 最小値を-MAXRPMに抑える
      dxl.setGoalVelocity(ID, u, UNIT_RPM);
      last_error = error;
    }
  }else if (stop == NOT){ // ----------------------------------------------------------------
    if (degrees <= 0){ // ΔΘはunsigned floatに丸める
      RPM *= -1;
      degrees *= -1;
    }
    dxl.setGoalVelocity(ID, RPM, UNIT_RPM);

    unsigned long int start_time_ms = millis();
    while (1){
      if (degrees <= abs(start_position - dxl.getPresentPosition(ID, UNIT_DEGREE))) return true;
      if ((millis()-start_time_ms) >= timeout_ms) return false;
    }
  }else if (stop == POSITION_MODE){ // ----------------------------------------------------------------
    if (RPM < 0) RPM *= -1;
    float rawSpeed = RPM *445 /101.85;
    dxl.writeControlTableItem(PROFILE_VELOCITY, ID, rawSpeed);
    dxl.setGoalPosition(ID, degrees + start_position, UNIT_DEGREE);

    if (!wait) return true;

    unsigned long int start_time_ms = millis();
    while (1){
      if (degrees <= abs(start_position - dxl.getPresentPosition(ID, UNIT_DEGREE))) return true;
      if ((millis()-start_time_ms) >= timeout_ms) return false;
    }
  }
}

/* ================================================================================================================================ */
float XL330::position(uint8_t ID){
  return dxl.getPresentPosition(ID, UNIT_DEGREE);
}

/* ================================================================================================================================ */
bool XL330::changeMode(uint8_t ID, uint8_t new_mode){
  if (new_mode == mode){
    return true;
  }

  dxl.torqueOff(ID);
  if (new_mode == VELOCITY){
    dxl.setOperatingMode(ID, OP_VELOCITY);
    mode = VELOCITY;
  } else {
    dxl.setOperatingMode(ID, OP_EXTENDED_POSITION);
    mode = POSITION;
  }
  dxl.torqueOn(ID);

  return true;
}

void XL330::updateEEPROM(){
  EEPROM.get(EEP_Kp_position, Kp_position);
  EEPROM.get(EEP_Ki_position, Ki_position);
  EEPROM.get(EEP_Kd_position, Kd_position);
}