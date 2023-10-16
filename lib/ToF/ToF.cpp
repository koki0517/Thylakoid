#include <ToF.h>

/*壁を見るセンサーたち---------------------------------------------------*/
WallToF::WallToF(TwoWire *theWire){
  this->theWire = theWire;
}

bool WallToF::init(){
  /*どのセンサーがエラーを起こしたかを返せたらいいなー*/
  bool success = true;

  /*VL53L0X*/
  // すべてのVL53L0Xをオフにする
  for (uint8_t i = 0; i < numToF_VL0; i++) {
    pinMode(XSHUT_WALL_VL0[i],arduino::OUTPUT);
    digitalWrite(XSHUT_WALL_VL0[i], arduino::LOW);
  }

  // 全てのToFを初期化
  for (uint8_t i = 0; i < numToF_VL0; i++) {
    digitalWrite(XSHUT_WALL_VL0[i], arduino::HIGH);
    delay(1);
    VL53L0X[i].setBus(*theWire);
    VL53L0X[i].setAddress(FirstAddress_VL0 + i);
    VL53L0X[i].setTimeout(500);
    if (!VL53L0X[i].init()){
      success = false;
      Serial.printf("Failed to detect and initialize %u VL53L0XX\n",i);
    }

    // 射程を広げる
    VL53L0X[i].setSignalRateLimit(0.1);
    VL53L0X[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    VL53L0X[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

    // 高速モード
    VL53L0X[i].setMeasurementTimingBudget(20000);

    // start
    VL53L0X[i].startContinuous(10);
  }
  /*VL53L1X--------------------------------------------------------------------------*/
  // すべてのVL53L1Xをオフにする
  for (uint8_t i = 0; i < numToF_VL1; i++) {
    pinMode(XSHUT_WALL_VL1[i],arduino::OUTPUT);
    digitalWrite(XSHUT_WALL_VL1[i], arduino::LOW);
  }

  // 全てのToFを初期化
  for (uint8_t i = 0; i < numToF_VL0; i++) {
    pinMode(XSHUT_WALL_VL1[i], arduino::INPUT); // Pololu曰くhighにしたくないらしい
    delay(1);

    VL53L1X[i].setTimeout(500);
    if (!VL53L1X[i].init())
    {
      success = false;
      // Serial.printf("Failed to detect and initialize %u VL53L1X\n",i);
    }
    VL53L1X[i].setAddress(FirstAddress_VL1 + i);

    VL53L1X[i].setDistanceMode(VL53L1X::Long);
    VL53L1X[i].setMeasurementTimingBudget(50000); 

    VL53L1X[i].startContinuous(50);
  }

  return success;
}

uint16_t WallToF::read(uint8_t sensor_number){
  for (int i = 0; i < numToF_VL0; i++) {
    if (XSHUT_WALL_VL0[i] == sensor_number) {
      uint16_t vl_mm = VL53L0X[i].readRangeContinuousMillimeters();
      if (VL53L0X[1].timeoutOccurred()) init();
      return vl_mm;
    }
  }
  uint16_t vl_mm = VL53L1X[i].read();
  if (VL53L1X[i].timeoutOccurred()) init();
  return vl_mm;
}

/*床を見るセンサーたち------------------------------------------------------------------*/

FloorToF(TwoWire *theWire){
  VL53L0X VL53L0X[numToF];
  this->theWire = theWire;
  // すべてのToFをオフにする
  for (int i = 0; i < numToF; i++) {
    pinMode(XSHUT_FLOOOR[i],OUTPUT);
    digitalWrite(XSHUT_FLOOOR[i], LOW);
  }
}

bool FloorToF::init(){
  updateEEPROM();
  /*どのセンサーがエラーを起こしたかを返せたらいいなー*/
  bool success = true;
  // 全てのToFを初期化
  for (i = 0; i < numToF - 1; i++) {
    digitalWrite(XSHUT_FLOOR[i], HIGH);
    delay(1);
    VL53L0X[i].setTimeout(500);
    if (!VL53L0X[i].init()){
      success = false;
      // Serial.printf("Failed to detect and initialize %u VL6180X\n",i);
    }
    VL53L0X[i].setAddress(FirstAddress + i);

    // 射程を広げる
    VL53L0X[i].setSignalRateLimit(0.1);
    VL53L0X[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    VL53L0X[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

    // 高速モード
    VL53L0X[i].setMeasurementTimingBudget(20000);

    // start
    VL53L0X[i].startContinuous(10);
  }
  return success;
}

uint16_t FloorToF::read(uint8_t sensor_number){
  uint16_t vl_mm = VL53L0X[sensor_number].readRangeContinuousMillimeters();
  if (VL53L0X[sensor_number].timeoutOccurred()) init();
  return vl_mm;
}

bool FloorToF::findProtrusion(uint8_t sensor_number){
  bool success = FloorProtrusion <= read(sensor_number);
  return success;
}

void FloorToF::updateEEPROM(){
  EEPROM.get(EEP_FloorProtrusion, FloorProtrusion);
}