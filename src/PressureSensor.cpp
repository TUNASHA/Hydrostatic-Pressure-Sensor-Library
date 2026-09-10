
#include <PressureSensor.h>

PressureSensor::PressureSensor(int pin, int samples) {
  _pin = pin;
  _samples = samples;
}

void PressureSensor::begin() {
  pinMode(_pin, INPUT);
   #if defined(ARDUINO_ARCH_AVR) ||( ARDUINO_ARCH_SAM )||(ARDUINO_ARCH_SAMD)
  _adcRef = 5.0;
  _adcResolution = 1023;
  #elif
  pinMode(_pin, INPUT_ANALOG);
   analogReadResolution(12);
   _adcRef = 3.3;
   _adcResolution = 4095;
  #endif
}

void PressureSensor::setParam(float minPress, float maxPress, int shuntResistor) {
  _minPress = minPress;
  _maxPress = maxPress;
  _shuntResistor = shuntResistor;
}

float PressureSensor::readadc() {
  float avg = 0;
  for (int i = 0; i < _samples; i++) {
    avg = avg + analogRead(_pin) * _adcRef / _adcResolution;
    delay(1);
  }
  return avg / _samples;
}

float PressureSensor::readVolt() {
  return readadc();
}

float PressureSensor::readCurr() {
  return readadc() / _shuntResistor;
}

float PressureSensor::roundUp(float value, int decimals) {
  double multiplier = pow(10, decimals);
  return ceil(value * multiplier) / multiplier;
}

float PressureSensor::readPressure() {
  double in_current = roundUp(readCurr(), 3);
  float in_min = 0.004;
  float in_max = 0.016;
  result = _minPress + (_maxPress - _minPress) * ((in_current - in_min) / in_max);
  if (result < 0) { result = 0.000; }
  return result;
}




