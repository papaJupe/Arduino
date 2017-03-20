#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Mod of DemoReel100 example: various animated patterns using FastLED built-ins
//
//
// Mark Kriegsman, 12/14; 1703 AM mod for  (team) Y of 2 intersecting strings

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3

#define LED_TYPE    WS2812B  // our strings
#define COLOR_ORDER GRB
#define NUM_LEDS    143
CRGB leds[NUM_LEDS];

#define BRIGHTNESS    60
#define FRAMES_PER_SECOND  60

void setup()
{
  delay(2000); // 2 second delay for recovery

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, juggle }; // took out bpm sinelon

uint8_t gCurrentPatternNumber = 0; // index of current pattern
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
  // Call the current pattern function once/loop, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();  // array of ptrs to names of functions ?

  // paint some black
  for (byte i = 49; i < 69; i++)  // 1st 20 so dark; lighted ones make a Y
    leds[i] = 0x000000;

  // send the 'leds' array to the LED strip
  FastLED.show();
  //  delay keeps the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND); // if 120 fps this = 8 ms., pretty fast

  //  periodic update colors and patterns
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_SECONDS( random8(6,60) ) {  // fx from lib8tion: 8 bit random(min,max)
    nextPattern();  // change pattern
  }
}  // end loop

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))  // num elements = size of ptr array / size of ptr?

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  //  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns); // add some disorder:
  gCurrentPatternNumber = (gCurrentPatternNumber + random8()) % ARRAY_SIZE( gPatterns);
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)  // larger param = more glitter
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 9);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at some Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 9; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

