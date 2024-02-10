#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

#define DATA_FRQ 1000


//define PINS
const int ESTOPIN   = 9;
const int UP_PIN    = 11;
const int DOWN_PIN  = 10;
const int COMM      = 12;


//define variables
float previousAlt;
float initAlt = 0;
float maxAlt = 0;
float meanAlt;

float altitude;
float realAlt;
float temp;
float pressure;
float pressureSea;



bool estopState = LOW;
bool apogeeLCK = LOW;
  
void setup() {


  //Configuramos el pin del ESTOPIN
  pinMode(UP_PIN, OUTPUT);
  pinMode(DOWN_PIN, OUTPUT);
  pinMode(ESTOPIN, OUTPUT);
  pinMode(COMM, OUTPUT);




  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}
  
void loop() {
  readData();
  verifyAlt();


}


void readData(){

  realAlt = bmp.readAltitude(101500);
  pressure = bmp.readPressure();
  altitude = bmp.readAltitude();
  pressureSea = bmp.readSealevelPressure();
  temp = bmp.readTemperature();
  meanAlt = (previousAlt+realAlt)/2;
  
  
  if (initAlt == 0.0){
    initAlt = realAlt;
  }

  Serial.print("Init Altitude = ");
  Serial.print(initAlt);
  Serial.println(" meters");

  Serial.print("real Altitude = ");
  Serial.print(realAlt);
  Serial.println(" meters");
  
  Serial.print("mean Altitude = ");
  Serial.print(meanAlt);
  Serial.println(" meters");
  //printData();

}

void printData(){
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" Pa");
  
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(altitude);
  Serial.println(" meters");

  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(pressure);
  Serial.println(" Pa");

// you can get a more precise measurement of altitude
// if you know the current sea level pressure which will
// vary with weather and such. If it is 1015 millibars
// that is equal to 101500 Pascals.
  Serial.print("Real altitude = ");
  Serial.print(realAlt);
  Serial.println(" meters");
  
  Serial.println();
  delay(DATA_FRQ);

}


void verifyAlt(){
  //actualizamos la altura anterior
  previousAlt = realAlt;


  //condicion para desbloquear el estopin
  //la altitud media debe superar 400mts
  float apogee = initAlt + 2;
  if (realAlt > apogee ){
    apogeeLCK = HIGH;
  }


  //SUBIENDO
  if(realAlt > (meanAlt)){
    digitalWrite(UP_PIN,HIGH);
    digitalWrite(DOWN_PIN,LOW);
    }//BAJANDO
  else if (realAlt < meanAlt){
    digitalWrite(UP_PIN,LOW);
    digitalWrite(DOWN_PIN,HIGH);
    //condicion para lanzar el paracaidas
    //si la altura empieza a disminuir
    if(realAlt < meanAlt && apogeeLCK){
      fireParachute();
    }
  }


  //guardamos la altura maxima
  if(realAlt > maxAlt){
    maxAlt = realAlt;
  }

  delay(1000);
}





void fireParachute(){
  digitalWrite(ESTOPIN, HIGH);
  delay(5000);
  digitalWrite(ESTOPIN, LOW);
}






void sendESTOPIN(){
  estopState = digitalRead(ESTOPIN);

  // Crear un objeto JSON
  StaticJsonDocument<200> jsonDoc;


  jsonDoc["altitude"] = realAlt;
  jsonDoc["estop_state"] = estopState;

  // Serializar el objeto JSON
  char jsonBuffer[200];
  serializeJson(jsonDoc, jsonBuffer);

  // Enviar el JSON a través de I2C
  Wire.beginTransmission(8); // Dirección del segundo Arduino (receptor)
  Wire.write(jsonBuffer);
  Wire.endTransmission();

  Serial.println();
  delay(500);




}

