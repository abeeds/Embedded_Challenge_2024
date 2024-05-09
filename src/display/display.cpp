#include "display.h"


void lightNeoPixel(uint8_t pixel, uint8_t color) {
    switch (color) {
        case 0:
            // Green
            CircuitPlayground.setPixelColor(pixel, 0, 255, 0);
        break;

        case 1:
            // Yellow
            CircuitPlayground.setPixelColor(pixel, 255, 222, 0);
        break;

        case 2:
            // Red
            CircuitPlayground.setPixelColor(pixel, 255, 0, 0);
        break;

        default:
            CircuitPlayground.setPixelColor(pixel, 255, 0, 0);
            break;
            // Serial.println("Invalid color entered.");
    }
}


void greenYellowPattern(uint8_t num_yellow) {
    if(num_yellow > 10)
        return;

    for(uint8_t i = 0; i < 10; i++) {
        if(i <= (num_yellow - 1)) 
            lightNeoPixel(i, 1);
        else 
            lightNeoPixel(i, 0);
    }
}


void yellowRedPattern(uint8_t num_red) {
    if(num_red > 10)
        return;

    for(uint8_t i = 0; i < 10; i++) {
        if(i <= (num_red - 1)) 
            lightNeoPixel(i, 2);
        else 
            lightNeoPixel(i, 1);
    }
}


void displayPercent(double frequency, double intensity, bool final = false){
    if(frequency > 100) {
        // Serial.println("Frequency is greater than 100%.");
        return;
    }
    int brightness = 1 + (int)(254 * (intensity / 100.0));
    if(final)brightness = 10 + (int)(120 * (intensity / 100.0));
    CircuitPlayground.setBrightness(brightness);

    uint8_t val = frequency / 5;
    if(final){
        if(val < 5) {
            greenYellowPattern(val);
        }
        else {
            yellowRedPattern(val - 10);
        }
    }
    else{ 
        if(val < 11) {
            greenYellowPattern(val);
        }
        else {
            yellowRedPattern(val - 10);
        }
    }
}
