#include "clockStrip.h"
#include <clockDebug.h>

clockStrip::clockStrip():
Adafruit_NeoPixel(8, 1, NEO_GRB + NEO_KHZ800)
{
    numLeds = 8;
    hw_pin = 1;
    brightness = 15;
    enabled = true;
    ledsPerStep = 1;
    lastUpdate = millis();
    maxInterval = 60;
    timePixel = 0;
    offset = 0;
    OnComplete = NULL;
}

clockStrip::clockStrip(int p_numLeds, uint8 p_hw_pin, void (*callback)()) :
Adafruit_NeoPixel(p_numLeds, p_hw_pin, NEO_GRB + NEO_KHZ800)
{
    numLeds = p_numLeds;
    hw_pin = p_hw_pin;
    brightness = 15;
    ledsPerStep = 1;
    enabled = true;
    maxInterval = 60;
    lastUpdate = millis();
    timePixel = 0;
    offset = 0;
    OnComplete = callback;
}


clockStrip::~clockStrip() {
    clear();
    show();
    numLeds = 0;
    brightness = 0;
    enabled = false;
}

int clockStrip::getNumLeds() {
    return this->numLeds;
}

uint8 clockStrip::getHwPin() {
    return this->hw_pin;
}

void clockStrip::setOffset(uint8 p_offset) {
    offset = p_offset;
}

void clockStrip::setBrightness(uint8 p_brightness) {
    if (this->enabled) {
        this->brightness = p_brightness;
        Adafruit_NeoPixel::setBrightness(this->brightness);
    }
}

uint8 clockStrip::getBrightness() {
    return this->brightness;
}

void clockStrip::setMaxInterval(uint8 p_maxInt) {
    this->maxInterval = p_maxInt;
}

uint8 clockStrip::getMaxInterval() {
    return this->maxInterval;
}

void clockStrip::setLedsPerStep(uint8 p_steps) {
    this->ledsPerStep = p_steps;
}

uint8 clockStrip::getLedsPerStep() {
    return this->ledsPerStep;
}


bool clockStrip::isEnabled(){
    return this->enabled;
}

void clockStrip::enable() {
    this->enabled = true;
}

void clockStrip::disable() {
    this->enabled = false;
    clear();
    show();
}


void clockStrip::setTime(uint8 p_value) {
    int pixel = p_value;
    timePixel = p_value; ///TODO maybe it is pixel after modulus
    if (this->enabled) {
        // modulus calculation to round over on the strip
        pixel = (pixel + offset )% (this->maxInterval / this->ledsPerStep);
        // clear the strip when reach 0!
        if (timePixel == 0) {
            clear();
        }

        // light up ledsPerStep led's for one time entry
        // pixel*(ledsPerStep-1) - advance to the next slot
        // pixel+i lights up all ledsPerStep pixels
        for(int i=0; i < this->ledsPerStep; i++)
            setPixelColor( (pixel*(ledsPerStep-1))+pixel+i, 0x0000FF);
        show();
    }
}




// Update the pattern
void clockStrip::Update() {
    if (this->enabled) {
        if((millis() - lastUpdate) > Interval) { // time to update    
            lastUpdate = millis();
            switch(ActivePattern) {
                case RAINBOW_CYCLE:
                    RainbowCycleUpdate();
                    break;
                case THEATER_CHASE:
                    TheaterChaseUpdate();
                    break;
                case COLOR_WIPE:
                    ColorWipeUpdate();
                    break;
                case SCANNER:
                    ScannerUpdate();
                    break;
                case FADE:
                    FadeUpdate();
                    break;
                default:
                    break;
            }
        }
    }
}

// Increment the Index and reset at the end
void clockStrip::Increment() {
    if (Direction == FORWARD) {
        Index++;
        if (Index >= TotalSteps) {
            Index = 0;
            if (OnComplete != NULL) {
                OnComplete(); // call the comlpetion callback
            }
        }
    }
    else { // Direction == REVERSE
        --Index;
        if (Index <= 0) {
            Index = TotalSteps-1;
            if (OnComplete != NULL) {
                OnComplete(); // call the comlpetion callback
            }
        }
    }
}

// Reverse pattern direction
void clockStrip::Reverse() {
    if (Direction == FORWARD) {
        Direction = REVERSE;
        Index = TotalSteps-1;
    }
    else {
        Direction = FORWARD;
        Index = 0;
    }
}

// Initialize for a RainbowCycle
void clockStrip::RainbowCycle(uint8_t interval, direction dir ) {
    ActivePattern = RAINBOW_CYCLE;
    Interval = interval;
    TotalSteps = 255;
    Index = 0;
    Direction = dir;
}

// Update the Rainbow Cycle Pattern
void clockStrip::RainbowCycleUpdate() {
    for(int i=0; i < ledsPerStep*timePixel; i++) {
    // for(int i=0; i< numPixels(); i++) {
        setPixelColor((i+offset) % maxInterval, Wheel(((i * 256 / numPixels()) + Index) & 255));
    }
    show();
    Increment();
}


// Initialize for a Theater Chase
void clockStrip::TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir) {
    ActivePattern = THEATER_CHASE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
}

// Update the Theater Chase Pattern
void clockStrip::TheaterChaseUpdate() {
    for(int i=0; i< numPixels(); i++) {
        if ((i + Index) % 3 == 0) {
            setPixelColor(i, Color1);
        }
        else {
            setPixelColor(i, Color2);
        }
    }
    show();
    Increment();
}

// Initialize for a ColorWipe
void clockStrip::ColorWipe(uint32_t color, uint8_t interval, direction dir) {
    ActivePattern = COLOR_WIPE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color;
    Index = 0;
    Direction = dir;
}

// Update the Color Wipe Pattern
void clockStrip::ColorWipeUpdate() {
    setPixelColor(Index, Color1);
    // setPixelColor(Index, random(0xFFFFFFFF));
    show();
    Increment();
}


// Initialize for a SCANNNER
void clockStrip::Scanner(uint32_t color1, uint8_t interval) {
    ActivePattern = SCANNER;
    Interval = interval;
    TotalSteps = (numPixels() - 1) * 2;
    Color1 = color1;
    Index = 0;
}

// Update the Scanner Pattern
void clockStrip::ScannerUpdate() { 
    for (int i = 0; i < numPixels(); i++) {
        if (i == Index) { // Scan Pixel to the right        
            setPixelColor(i, Color1);
        }
        else if (i == TotalSteps - Index) { // Scan Pixel to the left
            setPixelColor(i, Color1);
        }
        else { // Fading tail
            setPixelColor(i, DimColor(getPixelColor(i)));
        }
    }
    show();
    Increment();
}

// Initialize for a Fade
void clockStrip::Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir ) {
    ActivePattern = FADE;
    Interval = interval;
    TotalSteps = steps;
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
}

// Update the Fade Pattern
void clockStrip::FadeUpdate() {
    // Calculate linear interpolation between Color1 and Color2
    // Optimise order of operations to minimize truncation error
    uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
    uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
    uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;
    
    ColorSet(Color(red, green, blue));
    show();
    Increment();
}


// Calculate 50% dimmed version of a color (used by ScannerUpdate)
uint32_t clockStrip::DimColor(uint32_t color) {
    // Shift R, G and B components one bit to the right
    uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}

// Set all pixels to a color (synchronously)
void clockStrip::ColorSet(uint32_t color) {
    for (int i = 0; i < numPixels(); i++) {
        setPixelColor(i, color);
    }
    show();
}

// Returns the Red component of a 32-bit color
uint8_t clockStrip::Red(uint32_t color) {
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t clockStrip::Green(uint32_t color) {
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t clockStrip::Blue(uint32_t color) {
    return color & 0xFF;
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t clockStrip::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if(WheelPos < 170) {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}