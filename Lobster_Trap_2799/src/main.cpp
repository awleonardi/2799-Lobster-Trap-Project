#include <Arduino.h>
#include "display.h"
#include "rotary_encoder.h"
#include "motors.h"
// #include "rtc.h"

#include "Wire.h"
#include "DFRobot_LCD.h"
#include <TimeLib.h>
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
char modeState = 0;
char inputState = 0;
long epoch_time = 1650952832; // Placeholder
long release_time = 0;
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
	setTime(epoch_time);
	adjustTime((-4)*3600);
}

void loop() {
    //breath(REG_ONLY);

	switch(modeState){
		case 0: {	// User input modeState
			switch(inputState){
				static long millisDelay = 0;
				case 0: {	// Rotary input
					sw.update();
					long encoderVal = read_encoder_value();
					static int i = 0;
					static bool saveTime = true;
					static time_t sT;

					if(saveTime){
						sT = now();
						saveTime = false;
						reset_abs_pos();
					}

					if(i > 100){
						char buffer[24];
						char buffer2[24];
						
						lcd.setCursor(0, 0);
						time_t cT = now();
						sprintf(buffer,  "%02d:%02d:%02d %02d/%02d  ", hour(cT), minute(cT), second(cT), month(cT), day(cT));
						sprintf(buffer2, "%02d:%02d:%02d %02d/%02d  ", hour(sT + encoderVal), minute(sT + encoderVal), second(sT + encoderVal), month(sT + encoderVal), day(sT + encoderVal));
						lcd.print(buffer);
						lcd.setCursor(0, 1);
						lcd.print(buffer2);
						
						i = 0;
					}
					i++;
					if(sw.fell()){		// fell should be on release
						inputState = 1;
						millisDelay = millis();
						saveTime = true;
						release_time = sT + encoderVal;
					}
					break;
				}
				case 1: {	// Time confirmation
					sw.update();
					if((millis() - millisDelay) < 2000){
						lcd.setCursor(0, 0);
						lcd.print("Press to confirm");
					}
					else{
						lcd.setCursor(0, 0);
						lcd.print("Wait to cancel  ");
					}
					lcd.setCursor(0, 1);
					lcd.print("04:20:00 4/20");	// Programmed release time

					if((millis() - millisDelay) > 5000){
						inputState = 0;
					}

					if(sw.fell()){
						inputState = 2;
					}
					break;
				}
				case 2: {	// Proceed to underwater
					lcd.setCursor(0, 0);
					lcd.print("Release confirmd");
					lcd.setCursor(0, 1);
					lcd.print("Extending magnet");
					inputState = 0;
					modeState = 1;

					break;
				}

			}


			break;
		}
		case 1: { // Motor extend modeState
			delay(2000); // Delay for motor
			modeState = 2;
			lcd.noDisplay();
			lcd.setRGB(0, 0, 0);
			break;
		}
		
		case 2: { // Underwater modeState
			time_t currentTime = now();
			if(currentTime > release_time){
				lcd.display();
				lcd.setRGB(0, 0, 255);
				lcd.setCursor(0, 0);
				lcd.print("Releasing!      ");
				lcd.setCursor(0, 1);
				lcd.print("Retracting mag  ");
				modeState = 3;
			}
			else{
				delay(1000);
			}

			break;
		}

		case 3: { // Motor retract for release
			delay(2000);	// Delay for motor
			modeState = 0;
			break;
		}
		
		
		
		case 4: { // Underwater modeState
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
			break;
		}
		case 5: { // Release idle modeState
			sw.update();
			lcd.setCursor(0, 0);
			static int count = 0;
			if(sw.fell()){
				count++;
			}
			lcd.print(count);

			break;
		}
		
		default: {
			modeState = 0; // Release in case of error
			break;
		}
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