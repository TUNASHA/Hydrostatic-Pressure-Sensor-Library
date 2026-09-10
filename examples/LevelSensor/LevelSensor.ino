
/*
This is simple example to read all data from hydrostatic liquid pressure sensor and print them out to serial bus.
*/

#include <PressureSensor.h>

PressureSensor PSensor = PressureSensor(A4, 400);         //adcpin, no of samples

float alpha = 0.2;                                      // Smoothing factor (0-1). Higher = more smoothing.
float filtered_value = 0;                              // Variable to store the filtered result
unsigned long previousMillis=0,FpreviousMillis=0;

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

   // This part of the code is optional, it executes low pass filtering every 10ms, suitable for noisy readings
   if (millis() - FpreviousMillis >= 10) {   
     filtered_value = alpha * filtered_value + (1 - alpha) * pressure; //lowpass filter
     FpreviousMillis = millis();
   }

   // prints out readings every 1second
  if (millis() - previousMillis >= 1000) {
    Serial.println("volt "       + String(volt));
    Serial.println("curr "       + String(curr, 4));
    Serial.println("pressure "   + String(pressure, 2));
    Serial.println("Filteredpressure "   + String(filtered_value, 2));
    previousMillis = millis();
  }
}
