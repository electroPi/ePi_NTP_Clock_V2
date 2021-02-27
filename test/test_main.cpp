
// >> pio test -e esp12e --verbose --test-port=/dev/ttyUSB0

// compile only if in correct env/testing situation
#if 1 // defined(ARDUINO) && defined(UNIT_TEST)

#include <Arduino.h>
#include "unity.h"

#include <clockStrip.h>
#include <clockConfig.h>

uint16 period = 200;


inline void wait() {
    unsigned long tNow = millis();
    while(millis() < tNow+period) {
        ESP.wdtFeed();
    }
}

void clockStripTest(void) {  
    // clockStrip *strp = new clockStrip(60, D3);
    clockStrip *strp = new clockStrip(24, D2);

    strp->begin();
    strp->show();

    Serial.println(">>> CLOCK STRIP TEST <<<");

    Serial.print(" >> Setting brightness to 10...");
    strp->setBrightness(10);
    TEST_ASSERT_EQUAL(10, strp->getBrightness());
    Serial.println("OK");
    // calls to tests will go here

    strp->setMaxInterval(12);
    TEST_ASSERT_EQUAL(12, strp->getMaxInterval());

    strp->setMaxInterval(24);
    TEST_ASSERT_EQUAL(24, strp->getMaxInterval());


    Serial.print(" >> Setting brightness to 250...");
    strp->setBrightness(250);
    TEST_ASSERT_EQUAL(250, strp->getBrightness());
    Serial.println("OK");

    // calling set time for each stuff
    int i = 0;
    for (i=0; i<60; i++) {
        Serial.print(" >>> Sending time value: ");
        Serial.println(i);
        strp->setTime(i);
        wait();
        //delay(250);
    }

    Serial.print(" >> Setting brightness to 10");
    strp->setBrightness(10);
    TEST_ASSERT_EQUAL(10, strp->getBrightness());
    Serial.println("OK");

    strp->clear();
    strp->show();
    TEST_ASSERT_EQUAL(true, strp->isEnabled());
    strp->disable();
    TEST_ASSERT_EQUAL(false, strp->isEnabled());

    Serial.println(" >> NO LED's shall light for now!");
    for (i=0; i<60; i++) {
        Serial.print(" >>> Sending time value: ");
        Serial.println(i);
        strp->setTime(i);
        wait();
        //delay(50);
    }
    strp->enable();
    TEST_ASSERT_EQUAL(true, strp->isEnabled());
    Serial.println("OK");

    TEST_ASSERT_EQUAL(1, strp->getLedsPerStep());

    Serial.println(" >> Setting 2 LED's per step");
    strp->setLedsPerStep(2);
    TEST_ASSERT_EQUAL(2, strp->getLedsPerStep());
    for (i=0; i<60; i++) {
        Serial.print(" >>> Sending time value: ");
        Serial.println(i);
        strp->setTime(i);
        wait();
        //delay(300);
    }
    Serial.println("OK");

    Serial.println(" >> Setting 3 LED's per step");
    strp->setLedsPerStep(3);
    TEST_ASSERT_EQUAL(3, strp->getLedsPerStep());
    for (i=0; i<60; i++) {
        Serial.print(" >>> Sending time value: ");
        Serial.println(i);
        strp->setTime(i);
        wait();
        //delay(300);
    }
    // tear down
    strp->clear();
    Serial.println("OK");

    Serial.println(">>> CLOCK STRIP TEST DONE <<<");
}

void eepromTest() {
    clockConfig *cc = clockConfig::getInstance();
    
    Serial.println(">>> EEPROM  TEST <<<");
    Serial.print(" >> Testing default values from constructor...");
    TEST_ASSERT_EQUAL(true, cc->getValidData());
    TEST_ASSERT_EQUAL(true, cc->getEnableHours());
    TEST_ASSERT_EQUAL(true, cc->getEnableMinutes());
    // TEST_ASSERT_EQUAL(true, cc->getEnableSeconds());
    TEST_ASSERT_EQUAL(true, cc->getEnableAutoNightMode());
    TEST_ASSERT_EQUAL(15, cc->getHoursBrightness());
    TEST_ASSERT_EQUAL(15, cc->getMinutesBrightness());
    // TEST_ASSERT_EQUAL(15, cc->getSecondsBrightness());
    TEST_ASSERT_EQUAL(7200, cc->getUTCTimeOffset());    
    TEST_ASSERT_EQUAL_STRING("20:00", cc->getAutoNightStartTime().c_str());
    TEST_ASSERT_EQUAL_STRING("07:00", cc->getAutoNightEndTime().c_str());
    Serial.println("OK.");

    Serial.print(" >> Erasing EEPROM...");
    cc->eraseEEPROM();
    cc->getValuesFromEEPROM();
    TEST_ASSERT_EQUAL(0, cc->getValidData());
    TEST_ASSERT_EQUAL(0, cc->getEnableHours());
    TEST_ASSERT_EQUAL(0, cc->getEnableMinutes());
    // TEST_ASSERT_EQUAL(0, cc->getEnableSeconds());
    TEST_ASSERT_EQUAL(0, cc->getEnableAutoNightMode());
    TEST_ASSERT_EQUAL(0, cc->getHoursBrightness());
    TEST_ASSERT_EQUAL(0, cc->getMinutesBrightness());
    // TEST_ASSERT_EQUAL(0, cc->getSecondsBrightness());
    TEST_ASSERT_EQUAL(0, cc->getUTCTimeOffset());    
    TEST_ASSERT_EQUAL_STRING("", cc->getAutoNightStartTime().c_str());
    TEST_ASSERT_EQUAL_STRING("", cc->getAutoNightEndTime().c_str());
    Serial.println("OK.");


    Serial.print(" >> Populating new values into internal structure...");
    cc->setValidData(true);
    cc->setEnableHours(true);
    cc->setEnableMinutes(true);
    // cc->setEnableSeconds(true);
    cc->SetEnableAutoNightMode(true);
    cc->setHoursBrightness(10);
    cc->setMinutesBrightness(10);
    // cc->setSecondsBrightness(10);
    cc->setUTCTimeOffset(3600);
    cc->setAutoNightStartTime("20:30");
    cc->setAutoNightEndTime("06:30");
    TEST_ASSERT_EQUAL(true, cc->getValidData());
    TEST_ASSERT_EQUAL(true, cc->getEnableHours());
    TEST_ASSERT_EQUAL(true, cc->getEnableMinutes());
    // TEST_ASSERT_EQUAL(true, cc->getEnableSeconds());
    TEST_ASSERT_EQUAL(true, cc->getEnableAutoNightMode());
    TEST_ASSERT_EQUAL(10, cc->getHoursBrightness());
    TEST_ASSERT_EQUAL(10, cc->getMinutesBrightness());
    // TEST_ASSERT_EQUAL(10, cc->getSecondsBrightness());
    TEST_ASSERT_EQUAL(3600, cc->getUTCTimeOffset());    
    TEST_ASSERT_EQUAL_STRING("20:30", cc->getAutoNightStartTime().c_str());
    TEST_ASSERT_EQUAL_STRING("06:30", cc->getAutoNightEndTime().c_str());   
    Serial.println("OK.");


    Serial.print(" >> Saving new values to EEPROM...");
    cc->setValuesToEEPROM();
    Serial.println("OK");

    // cc->setEnableSeconds(false);
    // TEST_ASSERT_EQUAL(false, cc->getEnableSeconds());


    Serial.print(" >> Zero out internal data...");
    cc->clearAllData();
    TEST_ASSERT_EQUAL(0, cc->getValidData());
    TEST_ASSERT_EQUAL(0, cc->getEnableHours());
    TEST_ASSERT_EQUAL(0, cc->getEnableMinutes());
    // TEST_ASSERT_EQUAL(0, cc->getEnableSeconds());
    TEST_ASSERT_EQUAL(0, cc->getEnableAutoNightMode());
    TEST_ASSERT_EQUAL(0, cc->getHoursBrightness());
    TEST_ASSERT_EQUAL(0, cc->getMinutesBrightness());
    // TEST_ASSERT_EQUAL(0, cc->getSecondsBrightness());
    TEST_ASSERT_EQUAL(0, cc->getUTCTimeOffset());    
    TEST_ASSERT_EQUAL_STRING("", cc->getAutoNightStartTime().c_str());
    TEST_ASSERT_EQUAL_STRING("", cc->getAutoNightEndTime().c_str());
    Serial.println("OK");

    Serial.print(" >> Read back data from EEPROM...");
    cc->getValuesFromEEPROM();
    TEST_ASSERT_EQUAL(true, cc->getValidData());
    TEST_ASSERT_EQUAL(true, cc->getEnableHours());
    TEST_ASSERT_EQUAL(true, cc->getEnableMinutes());
    // TEST_ASSERT_EQUAL(true, cc->getEnableSeconds());
    TEST_ASSERT_EQUAL(true, cc->getEnableAutoNightMode());
    TEST_ASSERT_EQUAL(10, cc->getHoursBrightness());
    TEST_ASSERT_EQUAL(10, cc->getMinutesBrightness());
    // TEST_ASSERT_EQUAL(10, cc->getSecondsBrightness());
    TEST_ASSERT_EQUAL(3600, cc->getUTCTimeOffset());    
    TEST_ASSERT_EQUAL_STRING("20:30", cc->getAutoNightStartTime().c_str());
    TEST_ASSERT_EQUAL_STRING("06:30", cc->getAutoNightEndTime().c_str());
    Serial.println("OK");

    Serial.println(">>> EEPROM  TEST DONE <<<");
}

// setup connects serial, runs test cases (upcoming)
void setup() {
    Serial.begin(115200);
    delay(2000);
    //
    UNITY_BEGIN();

    //RUN_TEST(clockStripTest);
    RUN_TEST(eepromTest);

    //delay(1000);

    UNITY_END();
}



void loop() {
  // nothing to be done here.
}

#endif

