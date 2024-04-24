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
            Serial.println("Invalid color entered.");
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


void displayPercent(double percent){
    if(percent > 100) {
        Serial.println("Value is greater than 100%.");
        return;
    }
    uint8_t val = percent / 5;

    if(val < 11) {
        CircuitPlayground.setBrightness(20);
        greenYellowPattern(val);
    }
    else {
        CircuitPlayground.setBrightness(255);
        yellowRedPattern(val - 10);
    }
}
