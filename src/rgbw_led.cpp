#include "rgbw_led.h"

RGBWLed::RGBWLed(uint8_t dataPin, rmt_channel_t rmtChannel)
    : dataPin(dataPin), rmtChannel(rmtChannel) {}

void RGBWLed::begin(uint16_t numLeds)
{
    leds.resize(numLeds, RGBW(0, 0, 0, 0));
    setupRMT();
}

void RGBWLed::setLED(uint16_t index, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
    if (index < leds.size())
    {
        leds[index] = RGBW(red, green, blue, white);
    }
}

void RGBWLed::setLED(uint16_t index, const RGBW &color)
{
    if (index < leds.size())
    {
        leds[index] = color;
    }
}

void RGBWLed::update()
{
    sendRGBW();
}

// RMT configuration for SK6812 RGBW
void RGBWLed::setupRMT()
{
    rmt_config_t config = {};
    config.rmt_mode = RMT_MODE_TX;
    config.channel = rmtChannel;
    config.gpio_num = (gpio_num_t)dataPin;
    config.clk_div = 2;
    config.mem_block_num = 1;
    config.flags = 0;

    config.tx_config.loop_en = false;
    config.tx_config.carrier_en = false;
    config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    config.tx_config.idle_output_en = true;

    rmt_config(&config);
    rmt_driver_install(rmtChannel, 0, 0);
}

// Convert RGBW to RMT items for SK6812
void RGBWLed::sendRGBW()
{
    const uint32_t T0H = 15; // 0.3us for '0' bit high time
    const uint32_t T0L = 45; // 0.9us for '0' bit low time
    const uint32_t T1H = 30; // 0.6us for '1' bit high time
    const uint32_t T1L = 30; // 0.6us for '1' bit low time

    uint16_t numLeds = leds.size();
    rmt_item32_t items[numLeds * 32 + 1]; // 32 bits per LED (8*4) + reset
    int item_idx = 0;

    for (int led = 0; led < numLeds; led++)
    {
        // SK6812 order: G, R, B, W
        uint8_t data[4] = {leds[led].g, leds[led].r, leds[led].b, leds[led].w};

        for (int byte_idx = 0; byte_idx < 4; byte_idx++)
        {
            for (int bit = 7; bit >= 0; bit--)
            {
                if (data[byte_idx] & (1 << bit))
                {
                    // Send '1'
                    items[item_idx].level0 = 1;
                    items[item_idx].duration0 = T1H;
                    items[item_idx].level1 = 0;
                    items[item_idx].duration1 = T1L;
                }
                else
                {
                    // Send '0'
                    items[item_idx].level0 = 1;
                    items[item_idx].duration0 = T0H;
                    items[item_idx].level1 = 0;
                    items[item_idx].duration1 = T0L;
                }
                item_idx++;
            }
        }
    }

    // Reset pulse
    items[item_idx].level0 = 0;
    items[item_idx].duration0 = 2000; // 50us reset
    items[item_idx].level1 = 0;
    items[item_idx].duration1 = 0;
    item_idx++;

    rmt_write_items(rmtChannel, items, item_idx, true);
}
