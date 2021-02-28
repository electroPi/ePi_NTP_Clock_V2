#include "clockScroller.h"
#include <clockDebug.h>
// used to read out the clock config data
#include <clockConfig.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
// Even if my HW type is FC16, I am using the ICSTATION because it seemed that 
// I installed it rotated with a 180 degrees.... using ICSATATION it will display
// the text as it is required
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define MAX_DEVICES 1

#define CLK_PIN   D5
#define DATA_PIN  D7
#define CS_PIN    D6



clockScroller::clockScroller() :
MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES) {
    scrollSpeed = clockConfig::getInstance()->getDisplayScrollSpeed();
    brightness = clockConfig::getInstance()->getDisplayBrightness();
    enabled = true;
}

clockScroller::~clockScroller() {
}

uint16 clockScroller::getScrollSpeed() {
    return scrollSpeed;
}

void clockScroller::setScrollSpeed(uint16 p_scrollSpeed) {
    scrollSpeed = p_scrollSpeed;
}


void clockScroller::scrollText(const char *p) {
  uint8_t charWidth;
  uint8_t cBuf[8];  // this should be ok for all built-in fonts

  if (enabled == true) {
    clear();

    while (*p != '\0')
    {
      charWidth = getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
      
      for (uint8_t i=0; i<=charWidth; i++)  // allow space between characters
      {
        transform(MD_MAX72XX::TSL);
        if (i < charWidth) {
          setColumn(0, cBuf[i]);
        }
      clkDelay(scrollSpeed);
      }
    }
    clear();
  } // end if enabled
}


void clockScroller::enable() {
  enabled = true;
}

void clockScroller::disable() {
  enabled = false;
  clear();
}

void clockScroller::setBrightness(uint8 p_brightness) {
  brightness = p_brightness;
  control(MD_MAX72XX::INTENSITY, brightness);
}

uint8 clockScroller::getBrightness() { 
  return brightness;
}