#ifndef MPU6050_h
#define MPU6050_h

#include <Wire.h>

#define MPU_ADDRESS 0x68

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

int tempH, tempL, tempOut;
float mpuTemp;

void setupMPU();
void recordAccelRegisters();
void proccessAccelData();
void recordGyroRegisters();
void proccessGyroData();
void recordTempRegisters();
void proccessTempData();
void printAcc();
void printGyro();
void printTemp();
void printData();

#endif
