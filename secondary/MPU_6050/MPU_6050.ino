#include "MPU_6050.h"


void setup() {
  Serial.begin(115200);

  Wire.setClock(400000);
  Wire.begin();
  setupMPU();
  delay(250);
}

void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  recordTempRegisters();
  
  printData();
  
  delay(100);
}



void setupMPU() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(CONFIG_REG);
  Wire.write(MPU6050_BAND_10_HZ);// Low pass filter of 10 hz
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(POWR_MGMT);
  Wire.write(0x00);  //sleep register to 0
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(GYRO_CONFIG);
  Wire.write(MPU6050_RANGE_500_DEG);
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(ACCEL_CONFIG);
  Wire.write(MPU6050_RANGE_16_G);
  Wire.endTransmission();

  // Wire.beginTransmission(MPU_ADDRESS);
  // Wire.write(0x1D);
  // Wire.write(0x24);
  // Wire.endTransmission();
}

void recordAccelRegisters() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU_ADDRESS, 6);
  while (Wire.available() < 6);

  accelX = Wire.read() << 8 | Wire.read();
  accelY = Wire.read() << 8 | Wire.read();
  accelZ = Wire.read() << 8 | Wire.read();

  proccessAccelData();
}

void proccessAccelData() {
  gForceX = accelX / 2048.0;
  gForceY = accelY / 2048.0;
  gForceZ = accelZ / 2048.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPU_ADDRESS, 6);
  while (Wire.available() < 6);

  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();

  proccessGyroData();
}

void proccessGyroData() {
  rotX = gyroX / 65.5;
  rotY = gyroY / 65.5;
  rotZ = gyroZ / 65.5;
}

void recordTempRegisters() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x41);
  Wire.endTransmission();

  Wire.requestFrom(MPU_ADDRESS, 2);
  while (Wire.available() < 2);

  tempH = Wire.read();
  tempL = Wire.read();

  proccessTempData();
}

void proccessTempData() {
  tempOut = (tempH << 8 | tempL);
  mpuTemp = (tempOut / 340.0) + 36.53;
}

void printAcc(){
    Serial.print(" Accel (g)");
    Serial.print(" X = ");
    Serial.print(gForceX);
    Serial.print(" Y = ");
    Serial.print(gForceY);
    Serial.print(" Z = ");
    Serial.print(gForceZ);

}
void printGyro(){
    Serial.print("      Gyro (deg) ");
    Serial.print("X = ");
    Serial.print(rotX);
    Serial.print(" Y = ");
    Serial.print(rotY);
    Serial.print(" Z = ");
    Serial.print(rotZ);

}
void printTemp(){
    Serial.print("      Temp (C)");
    Serial.print(" A = ");
    Serial.println(mpuTemp);

}
void printData(){
  printAcc();
  printGyro();
  printTemp();
}
