#include <ESP8266WiFi.h>
#include <CapacitiveSensor.h>

CapacitiveSensor capSensor = CapacitiveSensor(2, 4);


void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = capSensor.capacitiveSensor(30);
  int remappedValue = map(sensorValue, -2, 62000, 0, 1000);

  
  Serial.println("");
  Serial.print("Raw: ");
  Serial.print(sensorValue);
  Serial.print("   Remapped: ");
  Serial.print(remappedValue);

  delay(1000);
  
}
