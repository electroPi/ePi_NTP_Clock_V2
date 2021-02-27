/* Singleton implementation of NTP client */

#include <ePiNtpTime.h>

#include <clockConfig.h>




ePiNtpTime *ePiNtpTime::inst = NULL;

ePiNtpTime* ePiNtpTime::getInstance() {
    if (! inst) {
        inst = new ePiNtpTime();
    }

    return inst;
}

    
static WiFiUDP ntpUdp;

ePiNtpTime::ePiNtpTime() {

    // You can specify the time server pool and the offset (in seconds, can be
    // changed later with setTimeOffset() ). Additionaly you can specify the
    // update interval (in milliseconds, can be changed using setUpdateInterval() ).
    // ro - time - 30 minutes ntp query
    timeClient = new NTPClient(ntpUdp, "europe.pool.ntp.org", 3*3600, 30*60000);
    timeClient->begin();
    timeClient->setTimeOffset(clockConfig::getInstance()->getUTCTimeOffset());
}

ePiNtpTime::~ePiNtpTime() {
    delete timeClient;
    timeClient = NULL;
}

void ePiNtpTime::updateTimeOffset(int p_timeOffset) {
    timeClient->setTimeOffset(p_timeOffset);
    timeClient->forceUpdate();
}



int ePiNtpTime::getHours() {
    // 12 is reported as 0!!!
    timeClient->update();
    int h = timeClient->getHours();
    return h % 12;
}

int ePiNtpTime::getMinutes() {
    timeClient->update();
    return timeClient->getMinutes();
}

int ePiNtpTime::getSeconds() {
    timeClient->update();
    return timeClient->getSeconds();
}

void ePiNtpTime::getTime(int *p_h, int *p_m, int *p_s){
    timeClient->update();
    int h = timeClient->getHours();
    
    *p_s = timeClient->getSeconds();
    *p_m = timeClient->getMinutes();
    *p_h = h ;
}



#define LEAP_YEAR(Y)     ( (Y>0) && !(Y%4) && ( (Y%100) || !(Y%400) ) )

String ePiNtpTime::getDate() {
  unsigned long rawTime = timeClient->getEpochTime() / 86400L;  // in days
  unsigned long days = 0, year = 1970;
  uint8_t month;
  static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};

  while((days += (LEAP_YEAR(year) ? 366 : 365)) <= rawTime)
    year++;
  rawTime -= days - (LEAP_YEAR(year) ? 366 : 365); // now it is days in this year, starting at 0
  days=0;
  for (month=0; month<12; month++) {
    uint8_t monthLength;
    if (month==1) { // february
      monthLength = LEAP_YEAR(year) ? 29 : 28;
    } else {
      monthLength = monthDays[month];
    }
    if (rawTime < monthLength) break;
    rawTime -= monthLength;
  }
  String monthStr = ++month < 10 ? "0" + String(month) : String(month); // jan is month 1  
  String dayStr = ++rawTime < 10 ? "0" + String(rawTime) : String(rawTime); // day of month  
  return String(year) + "-" + monthStr + "-" + dayStr ;

}