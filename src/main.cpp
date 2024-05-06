#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>
#include "./display/display.h"
#include "./DFT/dft.h"

volatile int i = 0;
const int windowSize = 3; // Have a window of 3 seconds to collect data
const int SR = 100; // Sample rate of 100 Hz
const int n = windowSize * SR;
volatile double accelerometerData[windowSize * SR];


// write function declarations here:
float calculateNetAcceleration(float X, float Y, float Z);
void setupTimer();


void setup() {
  sei();
  Serial.begin(9600);
  // while (!Serial);
  Serial.println("Hello, world!");
  CircuitPlayground.begin();

  // Setup Timer 0 to generate an interrupt every 10 miliseconds
  // This will be used to sample the accelerometer data
  setupTimer();
}


ISR(TIMER0_COMPA_vect) {
  // Serial.println("HI");
  // This service routine will be called every 50 ms
  // We will use this to sample the accelerometer data
  float X = CircuitPlayground.motionX();
  float Y = CircuitPlayground.motionY();
  float Z = CircuitPlayground.motionZ();

  // float X = 1.0;
  // float Y = 2.0;
  // float Z = 3.0;

  // Serial.print(X);
  // Serial.print(",");
  // Serial.print(Y);
  // Serial.print(",");
  // Serial.println(Z);
  // delay(1000);

  double netAcceleration = calculateNetAcceleration(X, Y, Z);

  accelerometerData[i] = netAcceleration;

  i++;
  // Serial.print(",");
  // Serial.print(Y);
  // Serial.print(",");
  // Serial.println(Z);

  //also isnt SR should be 1 / SR * 1000
}

void loop() {
  if(i == n) {
    // Disable the Timer interrupt
    TIMSK0 &= ~(1 << OCIE0A);

    //Serial.println(i);
    // DFT dft(n, accelerometerData, SR);
    // double frequency_range = dft.percentageInFrequencyRange();
    // double intensity_range = dft.getIntensityRange();
    // Serial.print(" ");
   // Serial.println(frequency_range);
    //displayPercent(frequency_range, intensity_range);

    // Graph the FFT data using teleplot
    // dft.plotData();
    i = 0;

    // Resetting the Timer interrupt
    TIMSK0 |= (1 << OCIE0A);
  }
}


float calculateNetAcceleration(float X, float Y, float Z) {
  return sqrt(X * X + Y * Y + Z * Z);
}


void setupTimer() {
  TCCR0A = 0;
  // Setup TCCR0A register
  TCCR0A = (1 << WGM01);
  // Normal Port Operation -> COM0A1 = 0, COM0A0 = 0
  // Disconnect OC0B - Normal Port operation -> COM0B1 = 0, COM0B0 = 0
  // CTC mode with OCRA as TOP -> WGM02 = 0, WGM01 = 1, WGM00 = 0
  
  TCCR0B = 0;
  // Setup TCCR0B register
  TCCR0B = (1 << CS02) | (1 << CS00);
  // FOC0A = 0, FOC0B = 0
  // WGM02 = 0
  // Use a clock prescaler of 1024 -> CS02 = 1, CS01 = 0, CS00 = 1

  // Setup the OCR0A register
  OCR0A = 77; // 10 ms

  // Setup the TIMSK0 register
  TIMSK0 |= (1 << OCIE0A) | (1 << TOIE0);
}
