#include <Arduino.h>
#include "./display/display.h"


void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {
  // this is all just test code.
  // feel free to delete.
  // to try it, type a number into the serial monitor
  int x = 0;
  int y;
  if (Serial.available() > 0) {
    x = Serial.read();
  }

  if(x >= 48 && x <= 57){
    y = (x - 47) * 10;
    Serial.print("Displaying ");
    Serial.print(y);
    Serial.println("%");

    displayPercent(y);
  }
}
