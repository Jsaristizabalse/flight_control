#include <Wire.h>


#define MPU_ADDRESS 0x68
 
// float accX, accY, accZ;
// float AngleRoll, AnglePitch




 long accelX, accelY, accelZ;
 float gForceX, gForceY,gForceZ;

 long gyroX, gyroY, gyroZ;
 float rotX, rotY,rotZ;


 int tempH, tempL, tempOut;
 float mpuTemp;

// float LoopTimer;

 
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
  
  delay(50);
}



void setupMPU(){
  Wire.beginTransmission(MPU_ADDRESS); //Start i2c communication with gyro
  Wire.write(0x1A);             //low pass filter
  Wire.write(0x5);              //
  Wire.endTransmission();

  Wire.write(0x6B); //PwrMgmt
  Wire.write(0x0); //Sleep mode register to 0
  Wire.endTransmission();
  
  Wire.beginTransmission(MPU_ADDRESS); //I2C address MPU
  Wire.write(0x1B); //Giroscope reg
  Wire.write(0x8); //Giroscope scale (1) +/- 500 deg/sec
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS); //I2C address MPU
  Wire.write(0x1C); //Accelerometer reg
  Wire.write(0x24); //Accelerometer full scale +/- 16g
  Wire.endTransmission();

  Wire.beginTransmission(MPU_ADDRESS); //I2C address MPU
  Wire.write(0x1C); //Accelerometer reg
  Wire.write(0x24); //Accelerometer full scale +/- 16g
  Wire.endTransmission();
}




  void recordAccelRegisters(){
    Wire.beginTransmission(MPU_ADDRESS); // MPU
    
    Wire.write(0x3B); //Starting register for accel readings
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADDRESS,6); //request accel registers 3B - 40
    while(Wire.available() < 6);
    
    accelX = Wire.read()<<8| Wire.read(); //first two bytes
    accelY = Wire.read()<<8| Wire.read(); //middle two bytes
    accelZ = Wire.read()<<8| Wire.read(); //last two bytes

    proccessAccelData();
  }

  void proccessAccelData(){
    gForceX = accelX / 2048.0;
    gForceY = accelY / 2048.0;
    gForceZ = accelZ / 2048.0;
  }

  void recordGyroRegisters(){
    Wire.beginTransmission(MPU_ADDRESS); // MPU
    Wire.write(0x43); //Starting register for gyro readings
    Wire.endTransmission();
    Wire.requestFrom(MPU_ADDRESS,6); //request gyro registers 43 - 48
    while(Wire.available() < 6);
    
    gyroX = Wire.read()<<8| Wire.read(); //first two bytes
    gyroY = Wire.read()<<8| Wire.read(); //middle two bytes
    gyroZ = Wire.read()<<8| Wire.read(); //last two bytes

    proccessGyroData();
  }


  void proccessGyroData(){
    rotX = gyroX / 65.5;
    rotY = gyroY / 65.5;
    rotZ = gyroZ / 65.5;
  }

  void recordTempRegisters(){
    Wire.beginTransmission(MPU_ADDRESS); // MPU
    Wire.write(0x41); //Starting register for Temp readings
    Wire.endTransmission();

    Wire.requestFrom(MPU_ADDRESS,2); //request gyro registers 41 - 42
    while(Wire.available() < 2);
    
    tempH = Wire.read(); //first two bytes
    tempL = Wire.read(); //middle two bytes

    proccessTempData();
  }

    void proccessTempData(){
    tempOut = (tempH << 8 | tempL);
    mpuTemp = (tempOut/340.0) + 36.53;
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