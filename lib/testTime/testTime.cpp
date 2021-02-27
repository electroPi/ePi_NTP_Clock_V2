#include "testTime.h"

testTime::testTime() {
	seconds = minutes = hours = 0;
	setSecondSpeed(1000); // 1000 uSecond = 1 second
	ttLastUpdate = millis();
}

testTime::~testTime() {
}

void testTime::setSecondSpeed(unsigned long p_speed) {
	seconds_speed = p_speed;
	minutes_speed = 60*seconds_speed;
	hours_speed = 60*minutes_speed;
}

void testTime::generateTime(int *hour, int *minute, int *second) {
	
	if (millis() > ttLastUpdate + seconds_speed) {
		seconds = seconds + 1;
		seconds = seconds % 60;
			
		if (seconds == 0 ) { // rounded over, means a new minute!
			minutes = minutes + 1;
			minutes = minutes % 60;
				
			if (minutes == 0) { // minutes rounded over, a new hour!
				hours = hours + 1;
				hours = hours % 24;
			}
		}
			
		ttLastUpdate = millis();
	}
	
	*hour = hours;
	*minute = minutes;
	*second = seconds;
}
