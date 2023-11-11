#include <FloorToF.h>
/*床を見るセンサーたち------------------------------------------------------------------*/

void FloorToF::init(){
  updateEEPROM();
  // 全てのToFを初期化
  for (uint8_t i = 0; i < numToF; i++) {
    digitalWrite(XSHUT_FLOOR[i], arduino::HIGH);
    delay(1);
    VL6[i].init();
    VL6[i].configureDefault();
    VL6[i].setAddress(FirstAddress + i);
    VL6[i].setTimeout(500);
  }
}

uint16_t FloorToF::read(uint8_t sensor_number){
  uint16_t vl_mm = VL6[sensor_number].readRangeSingleMillimeters();
  if (VL6[sensor_number].timeoutOccurred()) init();
  return vl_mm;
}

bool FloorToF::findProtrusion(uint8_t sensor_number){
  if (FloorProtrusion <= read(sensor_number)){
    return true;
  } else {
    return false;
  }
}

void FloorToF::updateEEPROM(){
  EEPROM.get(EEP_FloorProtrusion, FloorProtrusion);
}