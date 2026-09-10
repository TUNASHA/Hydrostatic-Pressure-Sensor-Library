#include <PressureSensor.h>

PressureSensor::PressureSensor(int pin, int samples) {
  _pin = pin;
  _samples = samples;
}

void PressureSensor::begin() {

#if defined(ARDUINO_ARCH_AVR)
  // ---- 5V AVR boards: Uno, Mega, Leonardo, Pro Mini (5V) ----
  analogReference(DEFAULT);          // Vcc as reference (5.0V on a 5V board)
  pinMode(_pin, INPUT);
  _adcRef        = 5.0f;
  _adcResolution = 1023.0f;          // 10-bit

#elif defined(ARDUINO_ARCH_SAM)
  // ---- Arduino Due (3.3V, 12-bit) ----
  analogReference(AR_DEFAULT);
  pinMode(_pin, INPUT);
  _adcRef        = 3.3f;
  _adcResolution = 4095.0f;

#elif defined(ARDUINO_ARCH_SAMD)
  // ---- Arduino Zero / MKR (3.3V, 12-bit) ----
  analogReference(AR_DEFAULT);
  pinMode(_pin, INPUT);
  analogReadResolution(12);
  _adcRef        = 3.3f;
  _adcResolution = 4095.0f;

#elif defined(ARDUINO_ARCH_STM32)
  // ---- STM32 cores (Blue Pill, Black Pill, etc.) ----
  pinMode(_pin, INPUT_ANALOG);
  analogReadResolution(12);
  _adcRef        = 3.3f;
  _adcResolution = 4095.0f;

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




