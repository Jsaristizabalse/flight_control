#include "IMU_GY89.h"

 
// float accX, accY, accZ;
// float AngleRoll, AnglePitch

// float LoopTimer;

 
 void setup() {
  Serial.begin(115200);

  Wire.setClock(400000);
  Wire.begin();
  setupIMU();
  delay(250);
}

void loop() {
  recordAccelRegisters();
  // recordTempRegisters();
  printData();
  delay(100);
}



void setupIMU(){
  Wire.beginTransmission(IMU_ADDRESS); //Start i2c communication with gyro

  Wire.beginTransmission(IMU_ADDRESS); //I2C address MPU
  Wire.write(CTRL1_REG); //Accelerometer reg
  Wire.write(ACCEL_400hz); //Accelerometer full scale +/- 16g
  Wire.endTransmission();



  Wire.beginTransmission(IMU_ADDRESS); //I2C address MPU
  Wire.write(CTRL2_REG); //Accelerometer reg
  Wire.write(ACCEL_16G); //Accelerometer full scale +/- 16g
  Wire.endTransmission();

  Wire.beginTransmission(IMU_ADDRESS); //I2C address MPU
  Wire.write(CTRL5_REG); //Temp reg
  Wire.write(0b10011000); //Accelerometer full scale +/- 16g
  Wire.endTransmission();

}
void recordAccelRegisters(){
  Wire.beginTransmission(IMU_ADDRESS); // MPU
  
  Wire.write(0x28); //Starting register for accel readings
  Wire.endTransmission();
  Wire.requestFrom(IMU_ADDRESS,6); //request accel registers 3B - 40
  while(Wire.available() < 6);
  
  accelX = Wire.read()<<8| Wire.read(); //first two bytes
  accelY = Wire.read()<<8| Wire.read(); //middle two bytes
  accelZ = Wire.read()<<8| Wire.read(); //last two bytes

  proccessAccelData();
  }

void proccessAccelData(){
  gForceX = accelX * 0.000732;
  gForceY = accelY * 0.000732;
  gForceZ = accelZ * 0.000732;
}



void recordTempRegisters(){
  Wire.beginTransmission(IMU_ADDRESS); // MPU
  
  Wire.write(0x5); //Starting register for accel readings
  Wire.endTransmission();
  Wire.requestFrom(IMU_ADDRESS,2); //request accel registers 3B - 40
  while(Wire.available() < 2);
  
  tempH = Wire.read(); //first two bytes
  tempL = Wire.read(); //middle two bytes
  

  proccessTempData();
  }

void proccessTempData(){
  tempOut = (tempH << 8 | tempL);
  imuTemp = (tempL);


}


void printData(){
  printAcc();
  printTemp();
}

void printAcc(){
  Serial.print(" Accel (g)");
  Serial.print(" X = ");
  Serial.print(gForceX);
  Serial.print(" Y = ");
  Serial.print(gForceY);
  Serial.print(" Z = ");
  Serial.println(gForceZ);

}

void printTemp(){

    Serial.print("      Temp (C)");
    Serial.print(" A = ");
    Serial.println(imuTemp);

}



  