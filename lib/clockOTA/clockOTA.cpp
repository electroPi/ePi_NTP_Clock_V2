#include <clockOTA.h>

#include <ArduinoOTA.h>
#include <clockDebug.h>

clockOTA::clockOTA() {
  /* OTA Setup  */

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    // stop everything else!
    ///TODO Maybe here I need to disable the LED strips!
    
    //String s = "Starting firmware update " + type;
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    // do not waste the time with scrolling! it is critical due to communication timeouts!
    //scrollText(s.c_str());
  });
  ArduinoOTA.onEnd([]() {
    DEBUG_PRINTLN("FW Updated!");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    char msg[10];
    sprintf(msg, "%u%%", (progress / (total / 100)) );
    DEBUG_PRINTLN(msg);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR) {
      DEBUG_PRINTLN("FW Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      DEBUG_PRINTLN("FW Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      DEBUG_PRINTLN("FW Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      DEBUG_PRINTLN("FW Receive Failed");
    } else if (error == OTA_END_ERROR) {
      DEBUG_PRINTLN("FW End Failed");
    }
  });
  
}

clockOTA::~clockOTA() {
    
}

void clockOTA::begin() {
  ArduinoOTA.begin();
}

void clockOTA::setHostName(String p_hostName) {
  ArduinoOTA.setHostname(p_hostName.c_str());
  //ArduinoOTA.setPassword("electroPi");
}

void clockOTA::loop() {
  ArduinoOTA.handle();
}