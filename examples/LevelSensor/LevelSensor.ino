
/*
This is simple example to read all data from hydrostatic liquid pressure sensor and print them out to serial bus.
*/

#include <PressureSensor.h>

PressureSensor PSensor = PressureSensor(A4, 400);         //adcpin, no of samples
unsigned long previousMillis=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  PSensor.begin();
  PSensor.setParam(0, 500, 165);// min pressure = 0psi, max pressure =500psi, shunt resistor =165ohm
}

void loop() {
  // put your main code here, to run repeatedly:
  float pressure  = PSensor.readPressure();      // read pressure
  float volt      = PSensor.readVolt();         // read voltage 
  float curr      = PSensor.readCurr();        // read current

   // prints out readings every 1second
  if (millis() - previousMillis >= 1000) {
    Serial.println("volt "       + String(volt));          // print voltage across the  shunt resistor
    Serial.println("curr "       + String(curr, 4));      // in four decimal places,print the current flowing through the shunt resistor
    Serial.println("pressure "   + String(pressure, 2)); // in two decimal places,print the pressure readings
    previousMillis = millis();
  }
}
