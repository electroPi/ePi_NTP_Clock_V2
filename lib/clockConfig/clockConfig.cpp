#include <clockConfig.h>
#include "clockDebug.h"


clockConfig *clockConfig::inst = NULL;

clockConfig* clockConfig::getInstance() {
    if (! inst) {
        inst = new clockConfig();
    }

    return inst;
}


clockConfig::clockConfig() {
    eepromConfig.hasValidData = true;
    eepromConfig.hoursEnabled = true;
    eepromConfig.minutesEnabled = true;
    eepromConfig.displayEnabled = true;
    eepromConfig.autoNightModeEnabled = true;
    eepromConfig.displayTime = true;
    eepromConfig.displayDate = true;

    eepromConfig.hoursBrightness = 15;
    eepromConfig.minutesBrightness = 15;
    eepromConfig.displayBrightness = 15;

    eepromConfig.utcTimeOffset = 7200; // UTC time offset in seconds
    eepromConfig.displayScrollSpeed = 40;
    eepromConfig.displayRefresh = 5000; // 5 seconds

    eepromConfig.autoNightStartTime = "20:00";
    eepromConfig.autoNightEndTime = "07:00";
    EEPROM.begin(512);
}

clockConfig::~clockConfig() {
    eepromConfig.hasValidData = false;
    eepromConfig.autoNightStartTime.~String();
    eepromConfig.autoNightEndTime.~String();
}


void clockConfig::getValuesFromEEPROM() {
    EEPROM.get(CC_EEPROM_ADDR, eepromConfig);
}

void clockConfig::setValuesToEEPROM() {
    EEPROM.put(CC_EEPROM_ADDR, eepromConfig);
    EEPROM.commit();
}

void clockConfig::eraseEEPROM() {
    for (uint8 i = 0; i < sizeof(eepromConfig); i++)
        EEPROM.write(CC_EEPROM_ADDR+i, 0); 
    EEPROM.commit(); // only to force eeprom rewrite "reset"
}

void clockConfig::clearAllData() {
    memset(&eepromConfig, 0, sizeof(eepromConfig));
}


bool clockConfig::getValidData() {
    return eepromConfig.hasValidData;
}

void clockConfig::setValidData(bool p_valid) {
    eepromConfig.hasValidData = p_valid;
}


bool clockConfig::getEnableHours() {
    return eepromConfig.hoursEnabled;
}

void clockConfig::setEnableHours(bool p_enabled) {
    eepromConfig.hoursEnabled = p_enabled;
}



bool clockConfig::getEnableMinutes() {
    return eepromConfig.minutesEnabled;
}

void clockConfig::setEnableMinutes(bool p_enabled) {
    eepromConfig.minutesEnabled = p_enabled;
}



bool clockConfig::getEnableDisplay() {
    return eepromConfig.displayEnabled;
}

void clockConfig::setEnableDisplay(bool p_enabled) {
    eepromConfig.displayEnabled = p_enabled;
}



bool clockConfig::getEnableAutoNightMode() {
    return eepromConfig.autoNightModeEnabled;
}

void clockConfig::SetEnableAutoNightMode(bool p_enabled) {
    eepromConfig.autoNightModeEnabled = p_enabled;
}


bool clockConfig::getEnableTimeDisplay() {
    return eepromConfig.displayTime;
}

void clockConfig::setEnableTimeDisplay(bool p_enabled) {
    eepromConfig.displayTime = p_enabled;
}


bool clockConfig::getEnableDateDisplay() {
    return eepromConfig.displayDate;
}

void clockConfig::setEnableDateDisplay(bool p_enable) {
    eepromConfig.displayDate = p_enable;
}

uint8_t clockConfig::getHoursBrightness() {
    return eepromConfig.hoursBrightness;
}

void clockConfig::setHoursBrightness(uint8_t p_brightness) {
    eepromConfig.hoursBrightness = p_brightness;
}



uint8_t clockConfig::getMinutesBrightness() {
    return eepromConfig.minutesBrightness;
}

void clockConfig::setMinutesBrightness(uint8_t p_brightness) {
    eepromConfig.minutesBrightness = p_brightness;
}



uint8_t clockConfig::getDisplayBrightness() {
    return eepromConfig.displayBrightness;
}

void clockConfig::setDisplayBrightness(uint8_t p_brightness) {
    eepromConfig.displayBrightness = p_brightness;
}



int clockConfig::getUTCTimeOffset() {
    return eepromConfig.utcTimeOffset;
}

void clockConfig::setUTCTimeOffset(int p_utcOffset) {
    eepromConfig.utcTimeOffset = p_utcOffset;
}

uint16_t clockConfig::getDisplayScrollSpeed() {
    return eepromConfig.displayScrollSpeed;
}

void clockConfig::setDisplayScrollSpeed(uint16_t p_scrollSpeed) {
    eepromConfig.displayScrollSpeed = p_scrollSpeed;
}

uint16 clockConfig::getDisplayRefresh() {
    return eepromConfig.displayRefresh;
}

void clockConfig::setDisplayRefresh(uint16 p_refresh){
    eepromConfig.displayRefresh = p_refresh;
}

String clockConfig::getAutoNightStartTime() {
    return eepromConfig.autoNightStartTime;
}

void clockConfig::setAutoNightStartTime(String p_time) {
    eepromConfig.autoNightStartTime = p_time;
}


String clockConfig::getAutoNightEndTime() {
    return eepromConfig.autoNightEndTime;
}

void clockConfig::setAutoNightEndTime(String p_time) {
    eepromConfig.autoNightEndTime = p_time;
}
