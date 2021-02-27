#ifndef __EPI_NTP_TIME_H
#define __EPI_NTP_TIME_H

/**
 * This class encapsulates the NTP clock client functionality.
 * 
 * It is implemented as a singleton design pattern, one clock has only
 * one NTP client...
 */

#include <WiFiUdp.h>
#include <NTPClient.h>



class ePiNtpTime {
    private:
        static ePiNtpTime * inst;
        ePiNtpTime();
        NTPClient *timeClient;
    public:
        static ePiNtpTime *getInstance();
        ~ePiNtpTime();

        void updateTimeOffset(int p_timeOffset);

        int getHours();
        int getMinutes();
        int getSeconds();

        void getTime(int *h, int *m, int *s);
        String getDate();

};

#endif /* __EPI_NTP_TIME_H */