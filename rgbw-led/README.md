# RGBW LED Library

A PlatformIO library for controlling SK6812 RGBW addressable LEDs on ESP32 using the RMT peripheral.

## Features

- **SK6812 Support**: Optimized for SK6812 RGBW LEDs
- **RMT Peripheral**: Uses ESP32's RMT (Remote Control) hardware for efficient LED control
- **No External Dependency**: Minimal dependencies, just Arduino framework
- **Easy Integration**: Simple API for LED control

## Supported Boards

- ESP32 (all variants)
- ESP32-S2
- ESP32-S3
- ESP32-C3

## Installation

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps =
    rgbw-led
```

Or install directly:

```bash
pio lib install rgbw-led
```

## Usage

### Basic Example

```cpp
#include <Arduino.h>
#include <rgbw_led.h>

void setup() {
    setupRMT();  // Initialize RMT peripheral
    
    // Clear all LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = RGBW(0, 0, 0, 0);
    }
}

void loop() {
    // Set LED to pure white using white channel
    leds[0] = RGBW(0, 0, 0, 255);
    sendRGBW();
    delay(1000);
    
    // Set LED to red
    leds[0] = RGBW(255, 0, 0, 0);
    sendRGBW();
    delay(1000);
}
```

## Configuration

Edit the following defines in `include/rgbw_led.h` to match your setup:

```cpp
#define NUM_LEDS 1          // Number of LEDs
#define DATA_PIN 17         // GPIO pin connected to LED data
#define RMT_CHANNEL RMT_CHANNEL_0  // RMT channel to use
```

## API Reference

### RGBW Structure

```cpp
struct RGBW {
    uint8_t r, g, b, w;
    RGBW(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t white = 0);
};
```

### Functions

#### `void setupRMT()`

Initializes the RMT peripheral for SK6812 LED control. Call this once in `setup()`.

#### `void sendRGBW()`

Sends the current LED data to the strip. Call this after modifying the `leds` array.

### Global Variables

#### `RGBW leds[NUM_LEDS]`

Array of LED color values. Modify this array and call `sendRGBW()` to update LEDs.

## Timing

The library uses the following timing for SK6812:

- **T0H**: 0.3µs (15 RMT units at 2x clock division)
- **T0L**: 0.9µs (45 RMT units at 2x clock division)
- **T1H**: 0.6µs (30 RMT units at 2x clock division)
- **T1L**: 0.6µs (30 RMT units at 2x clock division)
- **Reset**: 50µs (2000 RMT units at 2x clock division)

## LED Order

SK6812 uses GRB+W order in the data stream:
- Byte 0: Green
- Byte 1: Red
- Byte 2: Blue
- Byte 3: White

## License

MIT License - See LICENSE file for details

## Author

Your Name (your.email@example.com)

## Support

For issues, questions, or contributions, please visit: https://github.com/yourusername/rgbw-led
