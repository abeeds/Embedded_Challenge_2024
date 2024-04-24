#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_CircuitPlayground.h>

// pixel value cannot be greater than 9
// color options: 
// red - 2
// yellow - 1
// green - 0
void lightNeoPixel(uint8_t pixel, uint8_t color);


// number must not be greater than 10
void greenYellowPattern(uint8_t num_yellow);


// number must not be greater than 10
void yellowRedPattern(uint8_t num_red);


// will light a pattern that indicates the percentage
// each light represents 5%
// from 0-50% the lights will be from green to yellow
// from 51-100% the lights will be from yellow to red
// percent should be a value from 0 - 100
void displayPercent(uint8_t percent);

#endif