/**
 * Encapsulation of the Arduino OTA (Over The Air) firmware update mechanisms...
 * It is used just to simplify the main file...
 */ 

#ifndef _CLOCK_OTA_H
#define _CLOCK_OTA_H

#include <WString.h>

class clockOTA {
    private:

    public:
        clockOTA();
        ~clockOTA();

        void begin();
        void loop();
        void setHostName(String p_hostName);
};

#endif /* _CLOCK_OTA_H */