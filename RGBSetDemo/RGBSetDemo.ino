// mod of orig example [garbled explanation, etc] to clarify

#include<FastLED.h>
#define NUM_LEDS 50

CRGBArray<NUM_LEDS> leds;   // the class instance

void setup()   // string type, data pin
{ FastLED.addLeds<NEOPIXEL, 4>(leds, NUM_LEDS);
  FastLED.setBrightness(60);
}

void loop() 
{
  static uint8_t hue;
  for (int i = 0; i < NUM_LEDS / 2; i++) 
  {  // starting at 0, writing to 1st half of string
    // fade hue sequence out, giving appearance of movement, tho none actually coded
    leds.fadeToBlackBy(40); // fades out later values written, so appears as moving comet
    // only 1st 20 or so appear lighted

    //  set an led color
    leds[i] = CHSV(hue++, 255, 255);

    // now, let's first 20 leds to the top 20 leds,(sic) [nonsense in orig example]
    // seems to set values of second 1/2 of string to inverse of 1st half, so comet
    // trails converge from ends to middle
    leds(NUM_LEDS / 2, NUM_LEDS - 1) = leds(NUM_LEDS / 2 - 1 , 0);
    FastLED.delay(33);  // perhaps this works like .show w/ built in delay ?
  }  // end for 
}

