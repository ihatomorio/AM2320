#ifndef WIREAM2320_H
#define WIREAM2320_H

#include <Arduino.h>

enum AM2320_Register  : word {
  HUMI_MSB = 0x00,
  HUMI_LSB = 0x01,
  TEMP_MSB = 0x02,
  TEMP_LSB = 0x03,
  MODEL_MSB = 0x08,
  MODEL_LSB = 0x09,
  VER_NUM = 0x0A,
  STATUS_AM2320 = 0x0F,
  USER_REG_MSB = 0x10,
  USER_REG_LSB = 0x11,
  USER_REG2_MSB = 0x12,
  USER_REG2_LSB = 0x13
};

enum AM2320_Function : word {
  READ = 0x03, //Read multiple registers
  WRITE = 0x10 //Write multiple registers
};

class CWireAM2320
{
  public:
    CWireAM2320();
    
    void init();
    void init(int pinSDA, int pinSCL);

    int fetch(); //refresh sensor value

    float humid; //Humidity
    float temp; //Temperature

  private:
    static const int AM2320_ADDR = 0x5C;
    unsigned long lastFetchTime = 0;
};

#endif

