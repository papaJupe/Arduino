
// FastLEDemo60, using short 60 lite string to show options

#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Mark Kriegsman, 12/14; 1703 AM mod from DemoReel100 example for (team) Y
//  of 2 intersecting strings;
// mod 2001 AM mod to demo decorative and functional patterns, control options for Team 3145

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN  14   // A0
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    50
#define SW_1        8   // push button
#define SW_2        7  // 

int count = 0;   // used by loop to control segment movement et al
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  30
#define FRAMES_PER_SECOND  50

void setup()
{
  delay(1000); // 1 second delay for recovery

  // tell FastLED about this LED strip configuration
  // FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  // set master brightness
  FastLED.setBrightness(BRIGHTNESS);
  // push button x 2 control display mode
  pinMode(SW_1, INPUT_PULLUP);   // when pressed (value goes false) lites show blue
  pinMode(SW_2, INPUT_PULLUP);   // if pressed (grounded), lites show pattern;

  pinMode(12, OUTPUT); // ground for LED proxy
  pinMode(13, OUTPUT); // pwr for LED proxy on defective board
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
}  // end setup

//// init variable for pattern options
//// List of library patterns to loop thru.  Each is defined as a separate function below.
//typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, juggle }; // took out bpm sinelon
//
//uint8_t gCurrentPatternNumber = 0; // index of current pattern
//uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{
  if (!digitalRead(SW_1)) // change lites to blue
  {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Blue;
      // send the 'leds' array to the LED strip

    }
  }


  // use cycling pre-made pattern
  // Call the current pattern function once/loop, updating the 'leds' array
  // gPatterns[gCurrentPatternNumber]();  // array of ptrs to names of functions ?

  // cycle lites in 15 lite segments in fwd/reverse direction:
  // for each i, (i+14 - count) or (i - count for larger #s) drops 14->0 as loop increments;
  // (I use loop count to increment the 'offset' value from segment end);
  // so mod 15 of (i + 14 - count) or (i +/- count for large #) also cycles 14 to 0 with time;
  // so [ < x] of the 15 lites will be [color] and 'shift forward' each offset increment;
  // SUBTRACTING offset simulates forward movement of segments; ADDING offset simulates reverse

  else if (!digitalRead(SW_2))  // show moving pattern
  {
    // for this demo, just simple red moving block on blue bkgnd
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      if ( ((i + 14 - count) % 15) < 7 ) // make x (last #) larger for bigger moving segment
      {
        leds[i] = CRGB::Red;
      }
      else
      {
        leds[i] = CRGB(0, 0, 40); // dark blue
      }
    }

  }  // end fwd
  //      else
  //      {
  //        // ADDING offset (count) reverses segment motion
  //        if ( ((i + 14 + count) % 15) < 7 )
  //        {
  //          leds[i] = red;
  //        }
  //        else
  //        {
  //          leds[i] = black;
  //        }
  //     }  // end else direction reversed

  else   // show RED
  { for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB::Red;

  } // end default to red

  // send the 'leds' color array to the LED strip
  FastLED.show();
  
  count++;  // count increments each loop, cycles 1++ --> 15 --> 1
  if (count > 14)   count = 1;
  // pulse board LED to avoid overcurrent
  if (count > 7) digitalWrite(12, LOW);
  else digitalWrite(12, HIGH);


  //  //  periodic update colors and patterns
  //  EVERY_N_MILLISECONDS( 20 ) {
  //    gHue++;  // slowly cycle the "base color" through the rainbow
  //  }
  //  EVERY_N_SECONDS( random8(6, 60) ) { // fx from lib8tion: 8 bit random(min,max)
  //    nextPattern();  // change pattern
  //  }

  //  delay keeps the framerate down, controls speed of segment movement
  FastLED.delay(50);
  //  FastLED.delay(1000 / FRAMES_PER_SECOND); // if 50 fps this = 20 ms., pretty fast
}  // end loop


/*
  #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))  // num elements = size of ptr array / size of ptr?

  void nextPattern()
  {
  // add one to the current pattern number, and wrap around at the end
  //  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
  // add some disorder:
  gCurrentPatternNumber = (gCurrentPatternNumber + random8()) % ARRAY_SIZE( gPatterns);
  }

  void rainbow()
  {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  }

  void rainbowWithGlitter()
  {
  // built-in FastLED rainbow, plus some random sparklies
  rainbow();
  addGlitter(80);
  }

  void addGlitter( fract8 chanceOfGlitter)  // larger param = more glitter
  {
  if ( random8() < chanceOfGlitter) {  // why += instead of =
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
  }  // end for
  } // end juggle

*/
