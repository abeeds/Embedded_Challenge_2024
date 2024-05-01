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
float calculateNetAcceleration(float X, float Y, float Z);
void setupTimer();

float calculateNetAcceleration(float X, float Y, float Z) {
  return sqrt(X * X + Y * Y + Z * Z);
}

void setupTimer() {
  // Setup TCCR0A register
  TCCR0A = (1 << COM0A1) | (1 << WGM01);
  // Clear OC0A on compare match -> COM0A1 = 1, COM0A0 = 0
  // Disconnect OC0B - Normal Port operation -> COM0B1 = 0, COM0B0 = 0
  // CTC mode with OCRA as TOP -> WGM02 = 0, WGM01 = 1, WGM00 = 0
  
  // Setup TCCR0B register
  TCCR0B = 0;
  // FOC0A = 0, FOC0B = 0
  // WGM02 = 0

  // Setup the OCR0A register
  OCR0A = 249; // 50 ms

  // Setup the TIMSK0 register
  TIMSK0 = (1 << OCIE0A);
  // Enable Compare Match A Interrupt -> OCIE0A = 1
  
}


void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();

  // Setup Timer 0 to generate an interrupt every 50 miliseconds
  // This will be used to sample the accelerometer data
  setupTimer();
}


ISR(TIMER0_COMPA_vect) {
  // This service routine will be called every 50 ms
  // We will use this to sample the accelerometer data
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

void loop() {
  //pass
}
