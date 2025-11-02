#include "rgbw_led.h"

RGBW leds[NUM_LEDS];

// RMT configuration for SK6812 RGBW
void setupRMT()
{
    rmt_config_t config = {};
    config.rmt_mode = RMT_MODE_TX;
    config.channel = RMT_CHANNEL;
    config.gpio_num = (gpio_num_t)DATA_PIN;
    config.clk_div = 2;
    config.mem_block_num = 1;
    config.flags = 0;

    config.tx_config.loop_en = false;
    config.tx_config.carrier_en = false;
    config.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    config.tx_config.idle_output_en = true;

    rmt_config(&config);
    rmt_driver_install(RMT_CHANNEL, 0, 0);
}

// Convert RGBW to RMT items for SK6812
void sendRGBW()
{
    const uint32_t T0H = 15; // 0.3us for '0' bit high time
    const uint32_t T0L = 45; // 0.9us for '0' bit low time
    const uint32_t T1H = 30; // 0.6us for '1' bit high time
    const uint32_t T1L = 30; // 0.6us for '1' bit low time

    rmt_item32_t items[NUM_LEDS * 32 + 1]; // 32 bits per LED (8*4) + reset
    int item_idx = 0;

    for (int led = 0; led < NUM_LEDS; led++)
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

    rmt_write_items(RMT_CHANNEL, items, item_idx, true);
}
