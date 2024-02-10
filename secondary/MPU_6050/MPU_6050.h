#ifndef MPU6050_h
#define MPU6050_h

#include <Wire.h>

#define MPU_ADDRESS   0x68
#define CONFIG_REG    0x1A
#define POWR_MGMT     0x6B

#define GYRO_CONFIG   0x1B
#define ACCEL_CONFIG  0x1C

#define MPU6050_RANGE_500_DEG   0x8 //00100100


#define MPU6050_RANGE_16_G      0x18

#define MPU6050_BAND_10_HZ      0x05







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
