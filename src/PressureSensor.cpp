
#include <PressureSensor.h>

PressureSensor::PressureSensor(int pin, int samples) {
  _pin = pin;
  _samples = samples;
}

#if defined(ARDUINO_ARCH_AVR)
  // Arduino Uno, Mega, Leonardo, etc. — 5V, 10-bit ADC
  pinMode(_pin, INPUT);
  _adcRef        = 5.0;
  _adcResolution = 1023;

#elif defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_SAMD)
  // STM32 / SAMD — 3.3V, configurable 12-bit ADC
  pinMode(_pin, INPUT_ANALOG);
  analogReadResolution(12);
  _adcRef        = 3.3;
  _adcResolution = 4095;

#elif defined(ARDUINO_ARCH_SAM)
  // Arduino Due — 3.3V, 12-bit ADC
  pinMode(_pin, INPUT);
  analogReadResolution(12);
  _adcRef        = 3.3;
  _adcResolution = 4095;

#else
  #error "PressureSensor: unsupported architecture"
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




