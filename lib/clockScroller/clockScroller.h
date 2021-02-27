#ifndef __CLOCK_SCROLLER_H
#define __CLOCK_SCROLLER_H

#include <MD_MAX72xx.h>


class clockScroller: public MD_MAX72XX {
    private:
        uint16 scrollSpeed;
        uint8 brightness;
        bool enabled;
    public:
        clockScroller();
        ~clockScroller();

        uint16 getScrollSpeed();
        void setScrollSpeed(uint16 p_scrollSpeed);

        void scrollText(const char *p);

        void enable();
        void disable();

        void setBrightness(uint8 p_brightness);
        uint8 getBrightness();
};

#endif /*__CLOCK_SCROLLER_H */
