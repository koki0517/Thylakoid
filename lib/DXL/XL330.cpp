#include "XL330.h"

bool XL330:init(){
  dxl.begin(1000000); // 1MHz!! 速い!!
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  for (uint8_t DXL_ID = 1; DXL_ID < 5; DXL_ID++){
    dxl.ping(DXL_ID);
    dxl.torqueOff(DXL_ID);
    dxl.setOperatingMode(DXL_ID, OP_VELOCITY);
    dxl.torqueOn(DXL_ID);
  }
  return true;
}
