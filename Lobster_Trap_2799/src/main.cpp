#include <Arduino.h>
#include "display.h"
#include "encoder.h"
#include "motors.h"
#include "rtc.h"

// Avoid using delay(), use millis() instead.
// Avoid blocking loops.
// Limit globals


void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
	// Blink the LED using blocking delays
	digitalWrite(LED_BUILTIN, HIGH);   
	delay(250);                       
	digitalWrite(LED_BUILTIN, LOW);    
	delay(250);                   
}