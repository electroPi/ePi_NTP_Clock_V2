/***
 * This class is used to simulate a clock. It's main feature is that
 * the second duration can be configured, thus it helps during testing.
 */

#ifndef __TEST_TIME_H
#define __TEST_TIME_H

#include <Arduino.h>

class testTime {
	private:
		int seconds;
		int minutes;
		int hours;
		
		unsigned long seconds_speed;
		unsigned long minutes_speed;
		unsigned long hours_speed;
		unsigned long ttLastUpdate;

	public:
		testTime();
		~testTime();
		
		void setSecondSpeed(unsigned long p_speed);		
		void generateTime(int *hour, int *minute, int *second);
};

#endif /* __TEST_TIME_H */