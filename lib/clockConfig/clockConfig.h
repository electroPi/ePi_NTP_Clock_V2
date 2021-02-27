/**
 * This class encapsulates the clock configuration structure, and 
 * handles the EEPROM save/load stuff associated with this structure. 
 */

#ifndef __CLOCK_CONFIG_H
#define __CLOCK_CONFIG_H


#include <EEPROM.h>
#include <WString.h>



#define CC_EEPROM_ADDR (0x0F)



/// helper structure to work with EEPROM methods
struct config_t {
    volatile bool hasValidData;
    volatile bool hoursEnabled;    // hours stripe enabled?
    volatile bool minutesEnabled;  // minutes stripe enabled?
    volatile bool displayEnabled;  // central display enabled?
    volatile bool autoNightModeEnabled;  // automatic brightness dimming on the specified hours
    volatile bool displayTime;     // used to decide if time is displayed
    volatile bool displayDate;     // used to decide if date is displayed

    volatile uint8_t hoursBrightness;  // hour strip brightness
    volatile uint8_t minutesBrightness; // minutes strip brightness
    volatile uint8_t displayBrightness; // display brightness
    volatile int16_t utcTimeOffset; // UTC time offset in minutes
    volatile uint16_t displayScrollSpeed; // scrolling speed
    volatile uint16 displayRefresh; // in how many milliseconds the central display will show the time/date (how many millis will sleep)

    String autoNightStartTime;
    String autoNightEndTime;
};




class clockConfig {
    private:
        static clockConfig * inst;
        config_t eepromConfig;
        clockConfig();
    public:
        static clockConfig *getInstance();
        ~clockConfig();
        
        void getValuesFromEEPROM();
        void setValuesToEEPROM();
        void eraseEEPROM();

        void clearAllData();

        bool getValidData();
        void setValidData(bool p_valid);

        bool getEnableHours();
        void setEnableHours(bool p_enabled);

        bool getEnableMinutes();
        void setEnableMinutes(bool p_enabled);

        bool getEnableDisplay();
        void setEnableDisplay(bool p_enabled);

        bool getEnableAutoNightMode();
        void SetEnableAutoNightMode(bool p_enabled);

        bool getEnableTimeDisplay();
        void setEnableTimeDisplay(bool p_enabled);

        bool getEnableDateDisplay();
        void setEnableDateDisplay(bool p_enabled);

        uint8_t getHoursBrightness();
        void setHoursBrightness(uint8_t p_brightness);

        uint8_t getMinutesBrightness();
        void setMinutesBrightness(uint8_t p_brightness);

        uint8_t getDisplayBrightness();
        void setDisplayBrightness(uint8_t p_brightness);

        int getUTCTimeOffset();
        void setUTCTimeOffset(int p_utcOffset);

        uint16_t getDisplayScrollSpeed();
        void setDisplayScrollSpeed(uint16_t p_scrollSpeed);

        uint16 getDisplayRefresh();
        void setDisplayRefresh(uint16 p_refresh);

        String getAutoNightStartTime();
        void setAutoNightStartTime(String p_time);

        String getAutoNightEndTime();
        void setAutoNightEndTime(String p_time);

};

#endif /* __CLOCK_CONFIG_H */
