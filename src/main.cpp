#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>
#include <arduinoFFT.h>
#include "./display/display.h"

/*
Group Members:
  Kunal Gupta - kg3163
  Shahria Abeed - sa5680
  Shubh Savani - sms9977
*/

volatile int i = 0;
const int n = 128;
double accelerometerData[n];
double imaginaryAccelerometerData[n];
const int sample_n = 300;
int sample_counter = 0;
int samples_percentage[sample_n];
int samples_intensity[sample_n];
bool checkedForParkinsons = false;
double percentage_average = 0;
double intensity_average = 0;

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
  // This service routine will be called every 50 ms
  // We will use this to sample the accelerometer data
  float X = CircuitPlayground.motionX();
  float Y = CircuitPlayground.motionY();
  float Z = CircuitPlayground.motionZ();

  double netAcceleration = calculateNetAcceleration(X, Y, Z);
  accelerometerData[i] = netAcceleration;

  i++;
}

void loop() {

  if(sample_counter == sample_n && !checkedForParkinsons){
    CircuitPlayground.playTone(440, 250);
    TIMSK0 &= ~(1 << OCIE0A);
    for(int j = 0; j < sample_n; j++){
      percentage_average += samples_percentage[j];
      intensity_average += samples_intensity[j];
    }
    percentage_average /= (double)sample_n;
    intensity_average /= (double)sample_n;
    Serial.println(percentage_average);
    Serial.println(intensity_average);
    displayPercent(percentage_average, intensity_average, true);
    checkedForParkinsons = true;
  }

  if(i == n && sample_counter < sample_n && !checkedForParkinsons) {
    // Disable the Timer interrupt
    TIMSK0 &= ~(1 << OCIE0A);

    ArduinoFFT<double> FFT(accelerometerData, imaginaryAccelerometerData, n, 200);
    FFT.windowing(FFTWindow::Rectangle, FFTDirection::Forward);	/* Weigh data */
    FFT.compute(FFTDirection::Forward); /* Compute FFT */
    FFT.complexToMagnitude();

    double totalPower = 0;
    double targetPower = 0;
    double targetIntensity = 0; 
    double frequencyResolution = 200.0 / n; // Sampling rate divided by number of samples

    for(int j = 1; j < n / 2; j++) { // Only need to consider half the spectrum due to symmetry in real signals
      double frequency = j * frequencyResolution;
      double power = accelerometerData[j] * accelerometerData[j];

      totalPower += power;

      if(frequency >= 3 && frequency <= 8) {
        targetPower += power;
        targetIntensity += power;
      }
    }

    double percentage = (targetPower / totalPower) * 100;
    double intensity = (min(targetIntensity / 1000, 100));

    Serial.print(sample_counter);
    Serial.print(" Percentage of power in 3-8 Hz range: ");
    Serial.print(percentage);
    Serial.print("%");
    Serial.print(" and intensity of that 3-8Hz range:");
    Serial.println(intensity);

    samples_percentage[sample_counter] = percentage;
    samples_intensity[sample_counter] = intensity;

    for(int j = 0; j < n; j++) {
      accelerometerData[j] = 0;
      imaginaryAccelerometerData[j] = 0;
    }
    displayPercent(percentage, intensity, false);
    i = 0;
    sample_counter++;

    // Enable Timer Interrupt
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
  TCCR0B = (1 << CS02);
  // FOC0A = 0, FOC0B = 0
  // WGM02 = 0
  // Use a clock prescaler of 1024 -> CS02 = 1, CS01 = 0, CS00 = 1

  // Setup the OCR0A register
  OCR0A = 155; // 5 ms

  // Setup the TIMSK0 register
  TIMSK0 |= (1 << OCIE0A) | (1 << TOIE0);
}
