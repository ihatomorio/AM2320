#include "WireAM2320.h"

#include <Arduino.h>
#include <Wire.h>

CWireAM2320::CWireAM2320() {
  humid = 0.0;
  temp = 0.0;
}

void CWireAM2320::init() {
  Wire.begin();
}

void CWireAM2320::init(int pinSDA, int pinSCL) {
  Wire.begin(pinSDA, pinSCL);
}

int CWireAM2320::fetch() {
  //minimal interval 2s 
  unsigned long elapsed = millis() - lastFetchTime;
  if (elapsed < 2000) delay(2000 - elapsed); //wait for 2s max
  lastFetchTime = millis(); //get last fetch time

  //Wake Sensor
  Wire.beginTransmission(AM2320_ADDR); //START + addr (+ wait 800us-3ms (auto?))
  Wire.endTransmission(); //STOP

  //Send the read command or send written instructions
  Wire.beginTransmission(AM2320_ADDR);//START + addr + W
  Wire.write(READ); //function code
  Wire.write(HUMI_MSB); //start address
  Wire.write(4); //number of registers
  int nEnd = Wire.endTransmission(); //STOP

  if (nEnd != 0) {
    return -1;
  }

  //To return the data read or confirmation signal
  delayMicroseconds(1500); //delay at least 1.5ms
  int ans = Wire.requestFrom(AM2320_ADDR, 6); //START + addr + R
  delayMicroseconds(30);

  if ( ans != 6) {
    return -1;
  }

  int H1, H2, T1, T2 = 0;
  for (int i = 1; i <  ans + 1; i++) {
    int c = Wire.read();
    switch (i) {
      case 3:
        H1 = c;
        break;
      case 4:
        H2 = c;
        break;
      case 5:
        T1 = c;
        break;
      case 6:
        T2 = c;
        break;
      default:
        break;
    }
  }
  humid = (H1 * 256 + H2) / 10.0;
  temp = (T1 * 256 + T2) / 10.0;

  return 0;
}

