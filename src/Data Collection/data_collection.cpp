#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

int i = 0;
const int windowSize = 3; // Have a window of 3 seconds to collect data
const int SR = 20; // Sample rate of 50 Hz
float accelerometerData[windowSize * SR];

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

  double netAccel = calculateNetAcceleration(X, Y, Z);

  accelerometerData[i] = netAccel;

  i++;
  if(i == windowSize * SR) {
    //do computations here
    i = 0;
  }

  Serial.print(X);
  Serial.print(",");
  Serial.print(Y);
  Serial.print(",");
  Serial.println(Z);

  delay(SR);
}
