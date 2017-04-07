#include "FastLED.h"
FASTLED_USING_NAMESPACE
// How many leds in your strip?
#define NUM_LEDS 2
#define BRIGHTNESS 22
#define DATA_PIN 15
#define LED_TYPE  WS2812B

// create array of leds object
CRGB leds[NUM_LEDS];

void setup() 
{   
  delay(1500);
  // Uncomment/edit one of the following lines for your led arrangement.

  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);B>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop() 
{ 
  // Turn the LED on, then pause
  leds[0] = CRGB::White;
  leds[1] = CRGB::White;
  //  leds[2] = CRGB::Blue;
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  //    leds[2] = CRGB::Black;
  FastLED.show();
  delay(500);
}
