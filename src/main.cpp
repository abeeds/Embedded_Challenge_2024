#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>
#include "./display/display.h"
#include "./DFT/dft.h"

int i = 0;
const int windowSize = 3; // Have a window of 3 seconds to collect data
const int SR = 20; // Sample rate of 20 Hz
int n = windowSize * SR;
double accelerometerData[windowSize * SR];

// write function declarations here:
void readAccelerometerData();
float calculateNetAcceleration();

float calculateNetAcceleration(float X, float Y, float Z) {
  return sqrt(X * X + Y * Y + Z * Z);
}

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {

  float X = CircuitPlayground.motionX();
  float Y = CircuitPlayground.motionY();
  float Z = CircuitPlayground.motionZ();

  // Serial.print(X);
  // Serial.print(",");
  // Serial.print(Y);
  // Serial.print(",");
  // Serial.println(Z);
  // delay(1000);

  double netAcceleration = calculateNetAcceleration(X, Y, Z);

  accelerometerData[i] = netAcceleration;

  i++;
  if(i == n) {
    DFT dft(n, accelerometerData, SR);
    double frequency_range = dft.percentageInFrequencyRange();
    double intensity_range = dft.getIntensityRange();
    // Serial.print(intensity_range);
    // Serial.print(" ");
    // Serial.println(frequency_range);
    displayPercent(frequency_range, intensity_range);

    // Graph the FFT data using teleplot
    dft.plotData();
    i = 0;
  }

  // Serial.print(X);
  // Serial.print(",");
  // Serial.print(Y);
  // Serial.print(",");
  // Serial.println(Z);

  //also isnt SR should be 1 / SR * 1000
  delay(SR);
}
