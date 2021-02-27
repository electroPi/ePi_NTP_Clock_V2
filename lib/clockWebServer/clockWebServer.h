/** 
 * This file contains the class that is responsible for 
 * the web-server implementation for the NTP clock.
 * 
 * The web server is basically the clock configuration interface.
 * It shows an HTML file, where the end-user can select from the 
 * following features:
 *  - enable/disable - seconds, minutes, hours strip
 *  - set the brightness level for: seconds, minutes, hours strips
 *  - save the configured values to the EEPROM (when poweroff-poweron the data will loaded from here)
 *  - restart the clock
 *  - poweroff the clock (ATTENTION: only the uController will be powered off. 
 *                        The LED strips will be powered, but switched off)
*/

#ifndef __CLOCK_WEB_SERVER_H
#define __CLOCK_WEB_SERVER_H


class clockWebServer {
    private:
        
    public:
        clockWebServer();
        ~clockWebServer();

        void begin();
        void loop();
};

#endif /* __CLOCK_WEB_SERVER_H */