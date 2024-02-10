#include <Wire.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // Se inicia como esclavo en la dirección 8
  Wire.onReceive(receiveData);  // Se llama a la función receiveData cuando se recibe un mensaje
}

void loop() {
  delay(100);
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    char receivedChar = Wire.read();  // Lee el byte recibido

    // Construir la cadena recibida
    String receivedData = "";
    receivedData += receivedChar;

    // Si la cadena recibida termina con un carácter de nueva línea, procesar el JSON
    if (receivedChar == '\n') {
      // Crear un nuevo objeto JSON
      StaticJsonDocument<200> jsonDoc;

      // Parsear la cadena JSON
      DeserializationError error = deserializeJson(jsonDoc, receivedData);

      // Verificar si ocurrió algún error al parsear el JSON
      if (!error) {
        // Extraer los valores del objeto JSON
        
        float altitude = jsonDoc["altitude"];
        bool estop_state = jsonDoc["estop_state"];

        // Mostrar los valores en el monitor serie
        Serial.println("Received Data:");
        Serial.print("Altitude: ");
        Serial.print(altitude);
        Serial.println(" meters");
        Serial.print("ESTOP State: ");
        Serial.println(estop_state ? "HIGH" : "LOW");
      } else {
        Serial.println("Error parsing JSON!");
      }
    }
  }
}
