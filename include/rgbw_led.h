#ifndef RGBW_LED_H
#define RGBW_LED_H

#include <Arduino.h>
#include <driver/rmt.h>
#include <vector>

// RGBW structure
struct RGBW
{
    uint8_t r, g, b, w;
    RGBW(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t white = 0)
        : r(red), g(green), b(blue), w(white) {}
};

// RGBWLed class for managing LED strips
class RGBWLed
{
public:
    // Constructor
    RGBWLed(uint8_t dataPin, rmt_channel_t rmtChannel = RMT_CHANNEL_0);

    // Initialize the LED strip with number of LEDs
    void begin(uint16_t numLeds);

    // Set LED color
    void setLED(uint16_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);
    void setLED(uint16_t index, const RGBW &color);

    // Send data to LEDs
    void update();

    // Get LED array
    std::vector<RGBW> &getLeds() { return leds; }

    // Get number of LEDs
    uint16_t getNumLeds() const { return leds.size(); }

private:
    uint8_t dataPin;
    rmt_channel_t rmtChannel;
    std::vector<RGBW> leds;

    void setupRMT();
    void sendRGBW();
};

#endif // RGBW_LED_H
