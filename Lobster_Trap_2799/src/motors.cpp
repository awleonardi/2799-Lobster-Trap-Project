#include <Arduino.h>
#include "motors.h"


const int STEP = 0;
const int DIR = 1;
bool boolin = true;

// void setup() {
//   Serial.begin(9600);
//   pinMode(STEP, OUTPUT);
//   pinMode(DIR, OUTPUT);
//   digitalWrite(STEP, LOW);
//   digitalWrite(DIR, LOW);
// }

// void loop() {
//   setEfforts();
//   delay(1);
//   stop();
//   delay(1);
//

void setEffort(bool clockwise) {
  if (clockwise) {
    digitalWrite(STEP, HIGH);
    digitalWrite(DIR, HIGH);
  } 
  else {
    digitalWrite(STEP, HIGH);
    digitalWrite(DIR, LOW);
  }
}

void stop(){
    digitalWrite(STEP, LOW);
    digitalWrite(DIR, LOW);
}


void setEfforts() {
  digitalWrite(STEP, HIGH);
  digitalWrite(DIR, HIGH);
}