#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1
#define BRIGHTNESS 64

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
// #define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];


void setup() 
{ 
  // Uncomment/edit one of the following lines for your leds arrangement.

  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);B>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() 
{ 
  //    // do rainbow w/ one lite
  static uint8_t starthue = 0;
  //  fill_rainbow(on strip v. color temper)
  leds[0] = CHSV(starthue++, 255, BRIGHTNESS);
    // blink bulbs
  //  // Turn the LED on, then pause
  //  leds[0] = CRGB::Red;
  //  //  leds[1] = CRGB::White;
  //  //  leds[2] = CRGB::Blue;
  //  FastLED.show();
  //  delay(500);
  //  // Now turn the LED off, then pause
  //  leds[0] = CRGB::Black;
  //  //   leds[1] = CRGB::Black;
  //  //    leds[2] = CRGB::Black;
  FastLED.show();
  delay(20);
}



