#include <Arduino.h>
#include "display.h"
#include "rotary_encoder.h"
#include "motors.h"
// #include "rtc.h"

#include "Wire.h"
#include "DFRobot_LCD.h"
#include <time.h>
#include <Bounce2.h>


// Avoid using delay(), use millis() instead.
// Avoid blocking loops.
// Limit globals


const int STEP = 2;
const int DIR = 3;

void stop();
void setEfforts_man(bool speed, bool clockwise);
int val = 1;
bool on = true;

DFRobot_LCD lcd(16,2);  //16 characters and 2 lines of show
char modeState = 2;
long epoch_time = 1650421475; // Placeholder
Bounce sw = Bounce();

void setup() {
    // initialize
	lcd.init();
	pinMode(8, INPUT);
	pinMode(7, INPUT_PULLDOWN);
	pinMode(1, INPUT_PULLDOWN);
	sw.attach(8);		// Attach rotary encoder switch to pin 8
	sw.interval(25); 	// Set 25ms debounce interval
	pinMode(STEP, OUTPUT);
	pinMode(DIR, OUTPUT);
	digitalWrite(STEP, LOW);
	digitalWrite(DIR, LOW);
}

void loop() {
    //breath(REG_ONLY);

	switch(modeState){
		case 0: {	// Demo modeState
			
			sw.update();
			long encoderVal = read_encoder_value();
			static int i = 0;
			if(i > 100){
				time_t rawtime = epoch_time + encoderVal;
				struct tm *info;
				char buffer[17];
				char buffer2[24];
				time( &rawtime );
				info = localtime( &rawtime );
				
				lcd.setCursor(0, 0);
				strftime(buffer, 17,"%H:%M:%S %b %d", info);
				lcd.print(buffer);
				lcd.setCursor(0, 1);
				sprintf(buffer2, "R_Encoder: %ld      ", encoderVal);
				lcd.print(buffer2);
				i = 0;
			}


			i++;
		}
		case 1: { // User input modeState
			bool switchState1 = digitalRead(7);
			bool switchState2 = digitalRead(1);
			
			if(switchState1 == true){
				lcd.setCursor(0, 0);
				lcd.print("1");
			}
			if(switchState1 == false){
				lcd.setCursor(0, 0);
				lcd.print(" ");
			}
			if(switchState2 == true){
				lcd.setCursor(0, 2);
				lcd.print("2");
			}
			if(switchState2 == false){
				lcd.setCursor(0, 2);
				lcd.print(" ");
			}
		}
		case 2: { // Underwater modeState
			bool switchState1 = digitalRead(7);
			bool switchState2 = digitalRead(1);
			if(switchState1 == true){
				unsigned int startTime = micros();
				unsigned int timeInterval = 1000;
				while((micros() - startTime) < timeInterval){
					setEfforts_man(on, true);
				}
				on = !on;
				
			}
			else if(switchState2 == true){
				unsigned int startTime = micros();
				unsigned int timeInterval = 1000;
				while((micros() - startTime) < timeInterval){
					setEfforts_man(on, false);
				}
				on = !on;
			}
			else{
				on = false;
				setEfforts_man(on, false);
			}
		}
		case 3: { // Release idle modeState

		}
		// default: {
		// 	modeState = 0; // Release in case of error
		// }
	}
}




void setEfforts_man(bool speed, bool clockwise) {

  if (speed == true){

    digitalWrite(STEP, HIGH);

    if (clockwise == true) {

      digitalWrite(DIR, HIGH);

    }

    if (clockwise == false) {

      digitalWrite(DIR, LOW);

    }

  }

  else{
	  
	digitalWrite(STEP, LOW);

    if (clockwise == true) {

      digitalWrite(DIR, HIGH);

    }

    if (clockwise == false) {

      digitalWrite(DIR, LOW);

    }
  }

}