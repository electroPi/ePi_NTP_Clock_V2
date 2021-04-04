#include <Arduino.h>


#include <clockDebug.h>
#include <clockStrip.h>
#include <clockConfig.h>
#include <clockWebServer.h>
#include <ePiNtpTime.h>
#include <clockOTA.h>
#include <clockScroller.h>


#include <WiFiManager.h>              // https://github.com/tzapu/WiFiManager


#include "testTime.h"



#define HOUR_PIN D2
#define MINUTES_PIN D3

#define CLOCK_WIFI  "ePi_NTP_clk"
#define CLOCK_WIFI_PASSWD "electroPi"


void hstripRollover();
void mstripRollover();

clockWebServer clk_ws = clockWebServer();
clockOTA clk_ota = clockOTA();
clockScroller clk_scrl = clockScroller();

clockStrip *minutesStrip = new clockStrip(60, MINUTES_PIN, &mstripRollover);
clockStrip *hoursStrip = new clockStrip(24, HOUR_PIN, &hstripRollover);

WiFiManager wm;
ePiNtpTime *timeClient = ePiNtpTime::getInstance();
clockConfig *clkConf = clockConfig::getInstance();
testTime tt;


// used to get the time from the ntp
int seconds ;
int minutes ;
int hours ;

// used for millis timing mechanism
unsigned long lastDisplayUpdate;
unsigned long dbgprintperiod = 1000;
unsigned long lastminute;





void hstripRollover() {
  hoursStrip->clear();
  // hoursStrip->show();
}

void mstripRollover() {
  minutesStrip->clear();
  // minutesStrip->show();
}

void daylightBrightness() {
  minutesStrip->setBrightness(clkConf->getMinutesBrightness());
  hoursStrip->setBrightness(clkConf->getHoursBrightness());
  clk_scrl.setBrightness(clkConf->getDisplayBrightness());
}

void nigthBrightness() {
  minutesStrip->setBrightness(1);
  hoursStrip->setBrightness(1);
  clk_scrl.setBrightness(1);
}


void setup() {
  Serial.begin(115200);  
  delay(2000);


  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);

  // cleanup everything before start!
  clk_scrl.begin();
  minutesStrip->begin();
  minutesStrip->clear();
  minutesStrip->show();
  hoursStrip->begin();
  hoursStrip->clear();
  hoursStrip->show();

  clk_scrl.scrollText("electroPi NTP clock V2.0");

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
  //WiFi.begin(ssid, password);
  WiFiManager wm;


  // wipe settings for testing purposes!
  //wm.resetSettings();
  
  bool res = wm.autoConnect(CLOCK_WIFI, CLOCK_WIFI_PASSWD); // password protected ap
  if(!res) {
    DEBUG_PRINTLN("Failed to connect to network! Resetting... ");
    clk_scrl.scrollText("Failed to connect to network! Resetting... ");
    delay(2000);
    ESP.reset();
  } 
  else {
        //if you get here you have connected to the WiFi    
      DEBUG_PRINTLN("connected to the network.");
  }


  while ( WiFi.status() != WL_CONNECTED ) {
    clk_scrl.scrollText("Connecting to WiFi...");
    DEBUG_PRINTLN("Connecting to WiFi...");
  }  
  String ip = "IP: " + WiFi.localIP().toString(); 
  DEBUG_PRINTLN(ip.c_str()); 
  clk_scrl.scrollText(ip.c_str());

  clk_ws.begin();

  clk_ota.setHostName(CLOCK_WIFI);
  clk_ota.begin();

  minutesStrip->begin();
  minutesStrip->RainbowCycle(5);
  minutesStrip->ActivePattern = RAINBOW_CYCLE;
  minutesStrip->TotalSteps = 255;
  minutesStrip->Interval = 200;
  minutesStrip->setBrightness( clkConf->getMinutesBrightness() );
  minutesStrip->setOffset(30);
  minutesStrip->setMaxInterval(60);
  minutesStrip->setLedsPerStep(1);

  hoursStrip->begin();
  hoursStrip->RainbowCycle(7);
  hoursStrip->ActivePattern = RAINBOW_CYCLE;
  hoursStrip->TotalSteps = 255;
  hoursStrip->Interval = 330;
  hoursStrip->setBrightness( clkConf->getHoursBrightness() );
  hoursStrip->setOffset(12);
  hoursStrip->setMaxInterval(24);
  hoursStrip->setLedsPerStep(1);

  lastDisplayUpdate = millis();
  lastminute = millis();

  timeClient->getTime(&hours, &minutes, &seconds);
  timeClient->getTime(&hours, &minutes, &seconds);
  timeClient->getTime(&hours, &minutes, &seconds);


  minutesStrip->setTime(minutes);
  minutesStrip->Update();
  hoursStrip->setTime(hours);
  hoursStrip->Update();
  clkDelay(2000);

  tt.setSecondSpeed(25);
  tt.setSecondSpeed(1);
}




void loop() {  
  clk_ws.loop();
  clk_ota.loop();

  timeClient->getTime(&hours, &minutes, &seconds);
  // tt.generateTime(&hours, &minutes, &seconds);



  if (millis() > lastDisplayUpdate + clockConfig::getInstance()->getDisplayRefresh()) {
    char tstr[8];
    sprintf(tstr, "%02d:%02d:%02d", hours, minutes, seconds);
    DEBUG_PRINTLN(tstr);
    if (clockConfig::getInstance()->getEnableTimeDisplay() == true) {
      clk_scrl.scrollText(tstr);
    }
    if (clockConfig::getInstance()->getEnableDateDisplay() == true) {
      clk_scrl.scrollText(timeClient->getDate().c_str());
    }
    lastDisplayUpdate = millis();
    
    // verify auto-night mode
    sprintf(tstr, "%02d:%02d", hours, minutes);    
    if (clkConf->getAutoNightStartTime().equals(tstr)){
      nigthBrightness();
    }
    if (clkConf->getAutoNightEndTime().equals(tstr)){
      daylightBrightness();
    }

    
  }

  if (millis() > lastminute + dbgprintperiod) {
    minutesStrip->setTime(minutes);
    hoursStrip->setTime(hours);
    lastminute = millis();
  }
  minutesStrip->Update();
  hoursStrip->Update();

  //clkDelay(25);

}


/********************************************************************************************************/
/* Functions used to connect the clock web server to the strip objects
I was too lazy to do with callbacks I guess...
 */




void minutesStripToggle(bool p_state) {
  if (p_state) {
    minutesStrip->enable();
  }
  else {
    minutesStrip->disable();
  }
}

void minutesStripSetBrightness(uint8 p_brightness) {
  minutesStrip->setBrightness(p_brightness);
}


void hoursStripToggle(bool p_state) {
  if (p_state) {
    hoursStrip->enable();
  }
  else {
    hoursStrip->disable();
  }
}

void hoursStripSetBrightness(uint8 p_brightness) {
  hoursStrip->setBrightness(p_brightness);
}

void displayEnabledToggle(bool p_state) {
  (p_state == true) ? clk_scrl.enable() : clk_scrl.disable();
}

void miniDisplaySetBrightness(uint8 p_brightness) {
  clk_scrl.setBrightness(p_brightness);
}

void miniDisplaySetScrollSpeed(uint16 p_speed) {
  clk_scrl.setScrollSpeed(p_speed);
}