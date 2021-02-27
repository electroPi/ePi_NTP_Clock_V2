/**
 * This class extends the Adafruit Neopixel library, and implements special
 * functionalities used by the NTP clock.
 */ 

#ifndef __CLOCKSTRIP_H
#define __CLOCKSTRIP_H

#include <Adafruit_NeoPixel.h>

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };


class clockStrip: public Adafruit_NeoPixel {
    private:
        /// Number of LED's in the current strip.
        int numLeds;
        /// The uController pin which drives the strip
        uint8 hw_pin;
        /// the strip brightness
        uint8 brightness;

        // how many leds are offset from absolute 0
        // in case of the 0'th led mounted down, then we
        // need to go with 50% forward....
        uint8 offset;

        /// if the strip is enabled or not
        bool enabled;

        /// ledsPerStep. number of LED's used for one time step
        uint8 ledsPerStep;
        /// maxInterval. 60-for minutes and seconds, 12 for hours
        /// this is used to % mod the time value (s/m/h), in order
        /// to round-over on the strip
        uint8 maxInterval;

        uint16 timePixel;

        uint16 updateInterval = 1000;

    public:
        // data from neoPatterns
        pattern  ActivePattern;  // which pattern is running
        direction Direction;     // direction to run the pattern
        
        unsigned long Interval;   // milliseconds between updates
        unsigned long lastUpdate; // last update of position
        
        uint32_t Color1, Color2;  // What colors are in use
        uint16_t TotalSteps;  // total number of steps in the pattern
        uint16_t Index;  // current step within the pattern
        
        void (*OnComplete)();  // Callback on completion of pattern



        clockStrip();
        clockStrip(int p_numLeds, uint8 p_hw_pin, void (*callback)() = NULL);
        ~clockStrip();
        
        int getNumLeds();
        uint8 getHwPin();

        void setOffset(uint8 p_offset);

        void setBrightness(uint8 p_brightness);
        uint8 getBrightness();

        void setMaxInterval(uint8 p_maxInt);
        uint8 getMaxInterval();

        void setLedsPerStep(uint8 p_steps);
        uint8 getLedsPerStep();

        bool isEnabled();
        void enable();
        void disable();

        void setTime(uint8 p_value);
        void setPixel(uint16 p_pos, uint8 p_R, uint8 p_G, uint8 p_B, uint8 p_W);
        
        // from neoPatterns
        void Update();
        void Increment();
        void Reverse();
        void RainbowCycle(uint8_t interval, direction dir = FORWARD);
        void RainbowCycleUpdate();
        void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD);
        void TheaterChaseUpdate();
        void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD);
        void ColorWipeUpdate();
        void Scanner(uint32_t color1, uint8_t interval);
        void ScannerUpdate();
        void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD);
        void FadeUpdate();
        uint32_t DimColor(uint32_t color);
        void ColorSet(uint32_t color);
        uint8_t Red(uint32_t color);
        uint8_t Green(uint32_t color);
        uint8_t Blue(uint32_t color);
        uint32_t Wheel(byte WheelPos);
};

#endif /* __CLOCKSTRIP_H */