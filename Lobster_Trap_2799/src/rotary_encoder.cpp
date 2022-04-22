#include "rotary_encoder.h"
#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability

	
Encoder myEnc(9, 10);


long read_encoder_value() {	// returns value in epoch seconds
	static long oldPosition  = -999;
	long newPosition = myEnc.read();
	
	if (newPosition != oldPosition) {
		oldPosition = newPosition;
	}

	return oldPosition;
}

