/*
FastRainbo160 makes red to blue rainbow with full string of 160 lites

Uses: FastLED lib
*/

#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 160
#define BRIGHTNESS 100

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
// #define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() 
{  
  delay (1000);
  // Uncomment/edit one of the following lines for your chipset.

  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // need GRB order for correct rendering
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);B>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

  // test to see if colors accurate; yes with GRB config above
  for(int i = 0; i <= NUM_LEDS; i++)
    leds[i] = CRGB::Red;
  FastLED.show();
  delay(1500);

  for(int i = 0; i <= NUM_LEDS; i++)
    leds[i] = CRGB::Green;
  FastLED.show();
  delay(1500);

  for(int i = 0; i <= NUM_LEDS; i++)
    leds[i] = CRGB::Blue;
  FastLED.show();
  delay(1500);

}  // end setup

void loop() 
{ 
  //    // do rainbow w/ full string, start red, end blue
  static uint8_t starthue = 0;
  // fill the array with values
  for(int i = 0; i <= NUM_LEDS; i++)
  {
    leds[i] = CHSV(starthue++, 255, BRIGHTNESS);
  }
  FastLED.show();
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

  delay(50);
}  // end loop
