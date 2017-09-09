/* Flash 2 neopixel LEDs at various brain wave entraining frequencies
 *  and colors; brightness adjusts with pot, codes one freq/color
 *  Uses: FastLED, analogRead, map, setBrightness
*/


#include "FastLED.h"
FASTLED_USING_NAMESPACE
// How many leds to control?
#define NUM_LEDS 2
#define BRIGHTNESS 12
#define DATA_PIN 15  // A1
#define LED_TYPE WS2812B
int brite = 0; // will set brightness from pot input during looping

// create leds color array
CRGB leds[NUM_LEDS];

void setup()
{
  delay(1500);
  // Uncomment/edit one of the following lines for your led type
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );
}  // end setup

void loop()
{
  brite = analogRead(A2);
  brite = map(brite,1,666,1,44); // map 0-3 v pot range to briteness
  FastLED.setBrightness(brite);
  // Turn the lites on, pause, then off; freq = 1000 msec / loop total msec
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Red;
  
//    leds[0] = CRGB::White;
//    leds[1] = CRGB::White;

  FastLED.show();
  delay(40);

  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;

  FastLED.show();
  delay(210);
}
