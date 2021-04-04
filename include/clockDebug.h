#ifndef __CLOCK_DEBUG_H
#define __CLOCK_DEBUG_H

#include <Arduino.h>

#undef DEBUG
#ifdef DEBUG
  #define DEBUG_PRINTLN Serial.println
  #define DEBUG_PRINT Serial.print
  #define DEBUG_START Serial.begin(115200);
  
#else
  #define DEBUG_PRINTLN 
  #define DEBUG_PRINT
  #define DEBUG_START 
#endif



/**
 * This function implements a busy waiting with WDG feeding.
 * 
 * This function replaces Arduino's delay function. It is based
 * on the millis() function, and also feeds the system watchdog
 * during the waiting period. When the period expires, it will return.
 */
inline void clkDelay(unsigned long period) {
    unsigned long tNow = millis();
    do {
        ESP.wdtFeed();
        yield();        
    } while(millis() < tNow+period) ;
}



#endif /* __CLOCK_DEBUG_H */