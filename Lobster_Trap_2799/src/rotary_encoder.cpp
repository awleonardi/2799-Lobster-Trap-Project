#include "rotary_encoder.h"
#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability

	
Encoder myEnc(9, 10);
long positionAbsolute = 0;


long read_encoder_value() {	// returns value in epoch seconds
	static long oldPosition  = -999;
	long newPosition = myEnc.read();
	
	if (newPosition != oldPosition) {
		if(newPosition > oldPosition){
			positionAbsolute += (1 + ((newPosition-oldPosition) * 0.09));
		}
		else if(newPosition < oldPosition){
			positionAbsolute -= (1 + ((oldPosition-newPosition) * 0.09));
		}
		oldPosition = newPosition;
	}

	return positionAbsolute;
}

void reset_abs_pos(){
	positionAbsolute = 0;
	return;
}
