#include <Arduino.h>
#include "display.h"
#include "rotary_encoder.h"
#include "motors.h"
#include "rtc.h"

#include "Wire.h"
#include "DFRobot_LCD.h"
#include <time.h>


// Avoid using delay(), use millis() instead.
// Avoid blocking loops.
// Limit globals


DFRobot_LCD lcd(16,2);  //16 characters and 2 lines of show
char mode = 0;
long epoch_time = 1650421475; // Placeholder

void setup() {
    // initialize
    lcd.init();
}

void loop() {

    //breath(REG_ONLY);

	switch(mode){
		case 0: {	// Demo mode
			long encoderVal = read_encoder_value();
			static int i = 0;
			if(i > 100){
				time_t rawtime = epoch_time + encoderVal;
				struct tm *info;
				char buffer[17];
				time( &rawtime );
				info = localtime( &rawtime );
				
				lcd.setCursor(0, 0);
				strftime(buffer, 17,"%H:%M:%S %b %d", info);
				lcd.print(buffer);
				lcd.setCursor(0, 1);
				lcd.print("Encoder:");
				lcd.setCursor(9, 1);
				lcd.print(encoderVal);
				i = 0;
			}

			i++;

		}
		case 1: { // User input mode

		}
		case 2: { // Underwater mode

		}
		case 3: { // Release idle mode

		}
		default: {
			mode = 3; // Release in case of error
		}
	}
}