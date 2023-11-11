#include <ToF.h>

/*壁を見るセンサーたち---------------------------------------------------*/

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
    VL0[i].setBus(&Wire1);
    VL0[i].setAddress(FirstAddress_VL0 + i);
    VL0[i].setTimeout(500);
    if (!VL0[i].init()){
      success = false;
      Serial.printf("Failed to detect and initialize %u VL53L0XX\n",i);
    }

    // 射程を広げる
    VL0[i].setSignalRateLimit(0.1);
    VL0[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    VL0[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

    // 高速モード
    VL0[i].setMeasurementTimingBudget(20000);

    // start
    VL0[i].startContinuous(10);
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

    VL1[i].setTimeout(500);
    if (!VL1[i].init())
    {
      success = false;
      // Serial.printf("Failed to detect and initialize %u VL53L1X\n",i);
    }
    VL1[i].setAddress(FirstAddress_VL1 + i);

    VL1[i].setDistanceMode(VL53L1X::Long);
    VL1[i].setMeasurementTimingBudget(50000); 

    VL1[i].startContinuous(50);
  }

  return success;
}

uint16_t WallToF::read(uint8_t sensor_number){
  for (int i = 0; i < numToF_VL0; i++) {
    if (XSHUT_WALL_VL0[i] == sensor_number) {
      uint16_t vl_mm = VL0[i].readRangeContinuousMillimeters();
      if (VL0[1].timeoutOccurred()) init();
      return vl_mm;
    }
  }

  for (int i = 0; i < numToF_VL1; i++) {
    if (XSHUT_WALL_VL1[i] == sensor_number) {
      uint16_t vl_mm = VL1[i].read();
      if (VL1[i].timeoutOccurred()) init();
      return vl_mm;
    }
  }
}