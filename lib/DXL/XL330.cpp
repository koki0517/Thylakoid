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
uint8_t XL330::drive_for_degrees(float RPM_LEFT, float RPM_RIGHT, float degrees, uint8_t stop, unsigned int timeout_ms, bool wait){
  // 最大値を超えていないか
  if (abs(RPM_LEFT) > MAXRPM) return SPEED_SIZE_ERR;
  if (abs(RPM_RIGHT) > MAXRPM) return SPEED_SIZE_ERR;
  // 0を除外
  if (degrees == 0) return DEGREES_ZERO;
  if (RPM_LEFT == 0 || RPM_RIGHT == 0) return SPEED_SIZE_ERR;

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
        if (2 >= degrees - abs(startPosition[i-1] - dxl.getPresentPosition(i, UNIT_DEGREE))) return SUCCESS;
      }
      if ((millis()-start_time_ms) >= timeout_ms) return TIMEOUT;
    }
  } else if (stop == BRAKE){ // BRAKE ------------------------------------------------------------
    int left_direction, right_direction;
    float start_position[4],goal_position[4];
    for (uint8_t i=1; i < 5; i++){
      start_position[i-1] = dxl.getPresentPosition(i, UNIT_DEGREE); // 初期位置を記録
    }

    // 進む方向の指定
    if (degrees > 0){
      if (RPM_LEFT > 0){
        left_direction = forward;
      } else left_direction = back;
      if (RPM_RIGHT > 0){
        right_direction = forward;
      } else right_direction = back;
    } else {
      if (RPM_LEFT > 0){
        left_direction = back;
      } else left_direction = forward;
      if (RPM_RIGHT > 0){
        right_direction = back;
      } else right_direction = forward;
    }

    degrees = abs(degrees); // 絶対値にする

    // 目標角度の指定
    for (int i = 1; i < 5; i++){
      if (i == 1 || i == 3){
        if (left_direction == forward){
          goal_position[i-1] = start_position[i-1] + degrees;
        } else goal_position[i-1] = start_position[i-1] - degrees;
      } else {
        if (right_direction == forward){
          goal_position[i-1] = start_position[i-1] - degrees;
        } else goal_position[i-1] = start_position[i-1] + degrees;
      }
    }

    // PID制御
    float error, last_error[4] = {0,0,0,0}, u, present_position;
    unsigned long int start_time_ms = millis();
    uint8_t j;
    float absLeftRPM = abs(RPM_LEFT), absRightRPM = abs(RPM_RIGHT);
    unsigned int timelimit = millis() + timeout_ms;
    uint8_t returnMessage = (uint8_t)TIMEOUT;
    bool reachedGoal = false;
    while (1){
      for (uint8_t i = 1; i < 5; i++){
        j = i-1;
        present_position = dxl.getPresentPosition(i, UNIT_DEGREE);
        error = goal_position[j] - present_position;
        u = Kp_position * error + Ki_position * (error + last_error[j]) + Kd_position * (error - last_error[j]);
        if (i == 1 || i == 3){
          if (u >= absLeftRPM) u = absLeftRPM;
          if (u <= -1*absLeftRPM) u = -1*absLeftRPM;
        } else {
          if (u >= absRightRPM) u = absRightRPM;
          if (u <= -1*absRightRPM) u = -1*absRightRPM;
        }
        dxl.setGoalVelocity(i, u, UNIT_RPM);
        last_error[j] = error;

        if (2 > abs(goal_position[j]- present_position) && !reachedGoal){ // 到達後・収束させるためにタイムリミットを変更する
          timelimit = millis() + 200;
          returnMessage = (uint8_t)SUCCESS;
          reachedGoal = true;
        }
      }
      
      if (millis() >= timelimit){
        drive(0,0);
        return returnMessage;
      }
    }
  } else { // POSITION_MODE ------------------------------------------------------------
    /* 4つ全部同じように動かすことしかできないよ
     * モードをVelocityに固定したいという思想から上のを作ったんだから、そもそもこれを作ること自体ががナンセンス
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
      if ((millis()-start_time_ms) <= timeout_ms) return TIMEOUT;
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
    unsigned long int start_time_ms = millis();
    float error, last_error = 0, u;
    float goal_position = start_position + degrees;
    float present_position;
    unsigned int timelimit = millis() + timeout_ms;
    uint8_t returnMessage = false;
    bool reachedGoal = false;
    while (1){
      present_position = dxl.getPresentPosition(ID, UNIT_DEGREE);
      error = goal_position - present_position;
      u = Kp_position * error + Ki_position * (error + last_error) + Kd_position * (error - last_error);
      if (u >= MAXRPM) u = 100; // 最大値をMAXRPMに抑える
      if (u <= -1*MAXRPM) u = -100; // 最小値を-MAXRPMに抑える
      dxl.setGoalVelocity(ID, u, UNIT_RPM);
      last_error = error;
      
      if (2 > abs(goal_position - present_position) && !reachedGoal){
        returnMessage = true;
        timelimit = millis() + 200;
        reachedGoal = true;
      }
      if (millis() >= timelimit){
        dxl.setGoalVelocity(ID, 0, UNIT_RPM);
        return returnMessage;
      }
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