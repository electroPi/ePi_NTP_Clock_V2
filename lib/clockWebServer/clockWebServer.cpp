#include <clockWebServer.h>

#include <ESP8266WebServer.h>
#include <PageBuilder.h>
#include <webPage.h>

// used to read out the clock config data
#include <clockConfig.h>
// used to handle the ntp time stuff
#include <ePiNtpTime.h>

#include "clockDebug.h"

unsigned long upMin = 0;
unsigned long upHour = 0;
unsigned long upDay = 0;

static ESP8266WebServer server;// = ESP8266WebServer();



/*******************************************************************************************/
/* Web page construction helpers */

// Get the hours strip enabled status
String getHourStripEnabled(PageArgument& args) {
  return (clockConfig::getInstance()->getEnableHours()) ? String("checked") : String("");
}
String getMinutesStripEnabled(PageArgument& args) {
  return (clockConfig::getInstance()->getEnableMinutes()) ? String("checked") : String("");
}
String getDisplayEnabled(PageArgument& args) {
  return (clockConfig::getInstance()->getEnableDisplay()) ? String("checked") : String("");
}
String getAutoNightEnabled(PageArgument& args) {
  return (clockConfig::getInstance()->getEnableAutoNightMode()) ? String("checked") : String("");
}

String getDisplayTimeEnabled(PageArgument& args) {
  return (clockConfig::getInstance()->getEnableTimeDisplay()) ? String("checked") : String("");
}
String getDisplayDateEnabled(PageArgument& args) {
  return (clockConfig::getInstance()->getEnableDateDisplay()) ? String("checked") : String("");
}
String getDisplayIntensityValue(PageArgument& args) {
  return String(clockConfig::getInstance()->getDisplayBrightness());
}
String getDisplayScrollSpeedValue(PageArgument& args) {
  return String(clockConfig::getInstance()->getDisplayScrollSpeed());
}
String getDisplayRefreshSpeedValue(PageArgument& args) {
  return String(clockConfig::getInstance()->getDisplayRefresh());
}


String getHourStripIntensityValue(PageArgument& args) {
  return String(clockConfig::getInstance()->getHoursBrightness());
}
String getMinutesStripIntensityValue(PageArgument& args) {
  return String(clockConfig::getInstance()->getMinutesBrightness());
}


String getUTCTimeOffset(PageArgument& args) {
  return String(clockConfig::getInstance()->getUTCTimeOffset());
}

String getAutoNightStartValue(PageArgument& args) {
  return String(clockConfig::getInstance()->getAutoNightStartTime());
}
String getAutoNightEndValue(PageArgument& args) {
  return String(clockConfig::getInstance()->getAutoNightEndTime());
}

String getUpDay(PageArgument& args) {
  return String(upDay);
}
String getUpHour(PageArgument& args) {
  return String(upHour);
}
String getUpMin(PageArgument& args) {
  return String(upMin);
}

// Page construction
PageElement MainPage(webPage, {
    {"CLOCK_ENABLE_CHECKED", getHourStripEnabled},
    {"MINUTES_ENABLED_CHECKED", getMinutesStripEnabled},   
    {"DISPLAY_ENABLED_CHECKED", getDisplayEnabled},
    {"AUTO_NIGHT_MODE_CHECKED", getAutoNightEnabled},

    {"DISPLAY_ENABLED_TIME_CHECKED", getDisplayTimeEnabled},
    {"DISPLAY_ENABLED_DATE_CHECKED", getDisplayDateEnabled},
    {"DISPLAY_INTENSITY_VALUE", getDisplayIntensityValue},
    {"SCROLL_SPEED_VALUE", getDisplayScrollSpeedValue},
    {"REFRESH_SPEED_VALUE", getDisplayRefreshSpeedValue},

    {"HOUR_STRIP_INTENSITY_VALUE", getHourStripIntensityValue},
    {"MINUTES_STRIP_INTENSITY_VALUE", getMinutesStripIntensityValue},

  
    {"UTC_TIME_OFFSET", getUTCTimeOffset},
  

    {"AUTO_TIME_START_TIME", getAutoNightStartValue},
    {"AUTO_TIME_END_TIME", getAutoNightEndValue},

    {"UP_DAY", getUpDay},
    {"UP_HOUR", getUpHour},
    {"UP_MIN", getUpMin}
    
});
PageBuilder NTPPage("/", {MainPage});



extern void minutesStripToggle(bool p_state);
extern void minutesStripSetBrightness(uint8 p_brightness);
extern void hoursStripToggle(bool p_state);
extern void hoursStripSetBrightness(uint8 p_brightness);
extern void displayEnabledToggle(bool p_state);
extern void miniDisplaySetBrightness(uint8 p_brightness);
extern void miniDisplaySetScrollSpeed(uint16 p_speed);


/*******************************************************************************************/

clockWebServer::clockWebServer() {

    // before constructing the HTML page, read out the values
    // from the EEPROM.
    clockConfig::getInstance()->getValuesFromEEPROM();

    NTPPage.insert(server);


    server.on("/reboot", []() {

      minutesStripToggle(false);
      hoursStripToggle(false);
      server.send(200, "text/html", "OK! Restarting now.... <br> <button onclick='window.history.back();'>Back</button>");
      delay(1000);
      ESP.restart();
    });

    server.on("/poweroff", []() {
      server.send(200, "text/html", "OK! System will be powered off! <br> <button onclick='window.history.back();'>Back</button>");
      // force to true, then call the toggle functions to clear up the displays

      minutesStripToggle(false);
      hoursStripToggle(false);
      ESP.deepSleep(0);
    });

    server.on("/postUTCTimeOffset/", []() {
      clockConfig::getInstance()->setUTCTimeOffset(server.arg(0).toInt());
      ePiNtpTime::getInstance()->updateTimeOffset(server.arg(0).toInt());
      server.send(200, "text/html", backPage);
    });


    server.on("/saveEEPROM", []() {    
      clockConfig::getInstance()->setValuesToEEPROM();
      server.send(200, "text/html", backPage);
    });

    server.on("/toggleHours", []() {
      bool crtStatus = clockConfig::getInstance()->getEnableHours();
      clockConfig::getInstance()->setEnableHours(!crtStatus);
      hoursStripToggle(!crtStatus);
      server.send(200, "text/html", backPage);    
    });

    server.on("/toggleMinutes", []() {
      bool crtStatus = clockConfig::getInstance()->getEnableMinutes();
      clockConfig::getInstance()->setEnableMinutes(!crtStatus);
      minutesStripToggle(!crtStatus);
      server.send(200, "text/html", backPage);
    });

    server.on("/toggleDisplay", []() {
      bool crtStatus = clockConfig::getInstance()->getEnableDisplay();
      clockConfig::getInstance()->setEnableDisplay(!crtStatus);
      displayEnabledToggle(!crtStatus);
      server.send(200, "text/html", backPage);
    });


    server.on("/postMinutesIntensity/", []() {    
      String msg = server.arg(0);
      clockConfig::getInstance()->setMinutesBrightness(msg.toInt());
      minutesStripSetBrightness(clockConfig::getInstance()->getMinutesBrightness());
      server.send(200, "text/html", backPage);
    });

    server.on("/postHoursIntensity/", []() {
      String msg = server.arg(0);
      clockConfig::getInstance()->setHoursBrightness(msg.toInt());
      hoursStripSetBrightness(clockConfig::getInstance()->getHoursBrightness());
      server.send(200, "text/html", backPage);    
    });

    server.on("/toggleDisplayEnableTime", []() {
      bool crtStatus = clockConfig::getInstance()->getEnableTimeDisplay();
      clockConfig::getInstance()->setEnableTimeDisplay(!crtStatus);
      server.send(200, "text/html", backPage);
    });

    server.on("/toggleDisplayEnableDate", []() {
      bool crtStatus = clockConfig::getInstance()->getEnableDateDisplay();
      clockConfig::getInstance()->setEnableDateDisplay(!crtStatus);
      server.send(200, "text/html", backPage);      
    });


    server.on("/postDisplayIntensity/", []() {
      String msg = server.arg(0);
      clockConfig::getInstance()->setDisplayBrightness(msg.toInt());
      miniDisplaySetBrightness(clockConfig::getInstance()->getDisplayBrightness());
      server.send(200, "text/html", backPage);
    });

    server.on("/postDisplayScrollSpeed/", []() {
      String msg = server.arg(0);
      clockConfig::getInstance()->setDisplayScrollSpeed(msg.toInt());
      miniDisplaySetScrollSpeed(msg.toInt());
      server.send(200, "text/html", backPage);
    });

    server.on("/postDisplayRefreshSpeed/", []() {
      String msg = server.arg(0);
      clockConfig::getInstance()->setDisplayRefresh(msg.toInt());
      server.send(200, "text/html", backPage);
    });
    


    server.on("/postAutoDim/", []() {
        String startTime = server.arg(0);
        String endTime = server.arg(1);
        String responseString ;

        clockConfig::getInstance()->setAutoNightStartTime(startTime);
        clockConfig::getInstance()->setAutoNightEndTime(endTime);

        if (startTime.equals(endTime) ) {
            clockConfig::getInstance()->SetEnableAutoNightMode(false);
            responseString = "OK! Auto Night Mode is DISABLED!";
        } else {
            clockConfig::getInstance()->SetEnableAutoNightMode(true);
            responseString = "OK! Display will be dimmed from: " + startTime + " until: "+ endTime;
        }

        responseString += "<br> <button onclick='window.history.back();'>Back</button>";
        server.send(200, "text/html",  responseString);
    });

    //
}

clockWebServer::~clockWebServer() {
    
}


void clockWebServer::begin() {
  server.begin();
}



void clockWebServer::loop() {
    server.handleClient();
    getUptime();
}  


void clockWebServer::getUptime() {
  unsigned long now = millis();

  upMin = (now / (1000 * 60)) % 60;
  upHour = (now / (1000 * 60 * 60)) % 24;
  upDay = (now / (1000 * 60 * 60 * 24)) % 365;
}