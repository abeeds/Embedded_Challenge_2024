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

volatile int i = 0;  // counter for sampling acceleration data
const int n = 64;    // size of acceleration data array
double accelerometerData[n];
double imaginaryAccelerometerData[n];
const int sample_n = 400;   // number of samples recorded
int sample_counter = 0;
uint8_t samples_percentage[sample_n];
uint8_t samples_intensity[sample_n];
bool checkedForParkinsons = false;    // true if done collecting samples
double percentage_average = 0;
double intensity_average = 0;

// write function declarations here:
float calculateNetAcceleration(float X, float Y, float Z);
void setupTimer();


void setup() {
  sei();
  Serial.begin(9600);
  CircuitPlayground.begin();

  // Setup Timer 0 to generate an interrupt every 5 miliseconds
  // This will be used to sample the accelerometer data
  setupTimer();
}


ISR(TIMER0_COMPA_vect) {
  // This service routine will be called every 5 ms
  // We will use this to sample the accelerometer data
  float X = CircuitPlayground.motionX();
  float Y = CircuitPlayground.motionY();
  float Z = CircuitPlayground.motionZ();

  double netAcceleration = calculateNetAcceleration(X, Y, Z);
  accelerometerData[i] = netAcceleration;

  i++;
}

void loop() {
  // Calculates the final average
  if(sample_counter == sample_n && !checkedForParkinsons){
    //play sound when finished
    CircuitPlayground.playTone(440, 4000);
    TIMSK0 &= ~(1 << OCIE0A);  // Disable the Timer interrupt
    
    // record sum of all power and intensities recorded
    for(int j = 0; j < sample_n; j++){
      percentage_average += samples_percentage[j];
      intensity_average += samples_intensity[j];
    }

    // calculate average
    percentage_average /= (double)sample_n;
    intensity_average /= (double)sample_n;

    Serial.print("Final Power: ");
    Serial.println(percentage_average);
    Serial.print("Final Intensity: ");
    Serial.println(intensity_average);

    displayPercent(percentage_average, intensity_average, true);
    checkedForParkinsons = true;    // done collecting samples
  }

  // Calculates data for each sample
  if(i == n && sample_counter < sample_n && !checkedForParkinsons) {
    TIMSK0 &= ~(1 << OCIE0A);  // Disable the Timer interrupt

    ArduinoFFT<double> FFT(accelerometerData, imaginaryAccelerometerData, n, 200);
    FFT.windowing(FFTWindow::Rectangle, FFTDirection::Forward);	/* Weigh data */
    FFT.compute(FFTDirection::Forward); /* Compute FFT */
    FFT.complexToMagnitude();

    double totalPower = 0;
    double targetPower = 0;
    double targetIntensity = 0; 
    double frequencyResolution = 200.0 / n; // Sampling rate divided by number of samples

    // record frequency and power
    for(int j = 1; j < n / 2; j++) { // Only need to consider half the spectrum due to symmetry in real signals
      double frequency = j * frequencyResolution;
      double power = accelerometerData[j] * accelerometerData[j];

      totalPower += power;

      // record power if in Parkinson's frequency range
      if(frequency >= 3 && frequency <= 6) {
        targetPower += power;
        targetIntensity += power;
      }
    }

    // convert power and intensity to percents
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

    // light neopixels based on recorded data
    displayPercent(percentage, intensity, false);
    i = 0;
    sample_counter++;

    TIMSK0 |= (1 << OCIE0A);  // Enable Timer Interrupt
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
  // Use a clock prescaler of 256 -> CS02 = 1, CS01 = 0, CS00 = 0

  // Setup the OCR0A register
  OCR0A = 155; // 5 ms

  // Setup the TIMSK0 register
  TIMSK0 |= (1 << OCIE0A) | (1 << TOIE0);
}
