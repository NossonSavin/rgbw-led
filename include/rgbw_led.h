#ifndef RGBW_LED_H
#define RGBW_LED_H

#include <Arduino.h>
#include <driver/rmt.h>

#define NUM_LEDS 1
#define DATA_PIN 17
#define RMT_CHANNEL RMT_CHANNEL_0

// RGBW structure
struct RGBW
{
    uint8_t r, g, b, w;
    RGBW(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t white = 0)
        : r(red), g(green), b(blue), w(white) {}
};

extern RGBW leds[NUM_LEDS];

// Function declarations
void setupRMT();
void sendRGBW();

#endif // RGBW_LED_H
