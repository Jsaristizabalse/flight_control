 #include <Wire.h>
#ifndef IMUGY89_h
#define IMUGY89_h

#include <Wire.h>

#define IMU_ADDRESS 0x1D




#define CTRL1_REG       0x20
#define CTRL2_REG       0x21
#define CTRL5_REG       0x24





#define ACCEL_400hz   0x87
#define ACCEL_16G   0x32



long accelX, accelY, accelZ;
float gForceX, gForceY,gForceZ;


int tempH, tempL, tempOut;
float imuTemp;

void setupIMU();
void recordAccelRegisters();
void proccessAccelData();

void recordTempRegisters();
void proccessTempData();

void printAcc();
void printTemp();
void printData();





#endif
