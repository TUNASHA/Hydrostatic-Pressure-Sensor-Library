#ifndef PSENSOR
#define PSENSOR
#include <Arduino.h>


class PressureSensor {
	// API functions:
public:
    PressureSensor     ( int  pin,int samples);    
   // ~PressureSensor     ( );                     //destructor
    void  setParam ( float minPress, float maxPress,int shuntResistor ); // minCurr,maxCurr,minPress,maxPress,ShuntR
    void  begin();
    float readVolt     ();
    float readCurr     ();
    float readPressure ();
    void  reset        ();

private:
    int   _pin;
    int   _samples;
    int   _adcResolution;
    float _adcRef;
    float _shuntResistor;
    float _minPress = 0;
    float _maxPress = 0;
    double result  = 0;
    float readadc  ( );
    float roundUp  (float value, int decimals );  
   
};

 //extern PressureSensor PSensor
#endif