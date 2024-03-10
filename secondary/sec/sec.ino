#include <Adafruit_BMP085.h>
#include <Wire.h>
#define SLAVE_ADDR 9
#define ANSWERSIZE 5

#define DATA_FRQ 1000

const int ESTOPIN   = 13;
const int UP_PIN    = 11;
const int DOWN_PIN  = 10;
const int COMM      = 9;


const int builtinLED   = 13;

//define variables
// altura a partir de la cual es seguro abrir el paracaidas
float lockAlt = 20.0;


float previousAlt;
float initAlt = 0;
float maxAlt = 0;
float meanAlt;

float altitude;
float realAlt;
float temp;
float pressure;
float pressureSea;


int estopinState = 0;
int estopinFlag = 1;

bool apogeeLCK = LOW;




Adafruit_BMP085 bmp;
  
void setup() {

  //Configuramos el pin del ESTOPIN
  pinMode(UP_PIN, OUTPUT);
  pinMode(DOWN_PIN, OUTPUT);
  pinMode(ESTOPIN, OUTPUT);
  pinMode(COMM, OUTPUT);
  pinMode(builtinLED, OUTPUT);

  Wire.begin(SLAVE_ADDR);  // Iniciar como esclavo con la dirección 9
  Wire.onReceive(receiveEvent);  // Configurar función para recibir datos


  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}



  
void loop() {

  readData();
  verifyAlt();
  stopinState();
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
  
  // Serial.print("mean Altitude = ");
  // Serial.print(meanAlt);
  // Serial.println(" meters");
  //printData();

}



void verifyAlt(){
  //actualizamos la altura anterior
  previousAlt = realAlt;


  //condicion para desbloquear el estopin
  //la altitud media debe superar 400mts
  float apogee = initAlt + lockAlt*0.5;

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
    if(realAlt < apogee && apogeeLCK && estopinFlag){
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
  digitalWrite(builtinLED, HIGH);
  digitalWrite(ESTOPIN, HIGH);
  delay(5000);
  digitalWrite(builtinLED, LOW);
  digitalWrite(ESTOPIN, HIGH);
}


void receiveEvent(int bytes){
  estopinState = Wire.read();
  Serial.print("Estado del pin: ");
  Serial.println(estopinState);

}

void stopinState(){

  if (estopinState == 1){
    estopinFlag = 0;
  }

  delay(250);
}
