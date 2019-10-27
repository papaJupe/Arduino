/* FastLEDparseInt
    Goal- display on/off states of 16 element IR array on string
    of 16 fastLEDs. This sketch tests ints input from Ser Mon to turn on
    specific lites, 0-15, from Mega.

    Uses: FastLED
*/


#include "FastLED.h"
// How many leds to control?
#define NUM_LEDS 16 
#define BRIGHTNESS 12
#define DATA_PIN 2
#define LED_TYPE WS2812B

// create leds color array
CRGB leds[NUM_LEDS];

void setup()
{ 
  Serial.begin(9600);

  delay(1000);

  // Uncomment/edit one of the following lines for your led type
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );

}  // end setup

void loop()
{ // void * memset ( void * ptr, int value, size_t num );
  //memset(inByt, 0, 5);  // now all null or '0'?
  
  static int c = 0; // the int input from Ser.Mon.
  
  // read input from Ser Mon
  if (Serial.available() > 0)
  {
    delay(20);
    //    Serial1.readBytes(inByt, 5); // incoming bytes --> inByt array
    //    Serial.print("inByt using .write ");
    //    Serial.write(inByt, 5);
    c = Serial.parseInt(); 
    // print input back to Ser Mon
    Serial.print("input is "); Serial.println(c);
  }  //end if serial.avail

  while (Serial.read() != -1); // clear the input buffer (should be already)

  //    // print new data in Ser Mon
  //    for (int i = 0; i < sizeof(inByt); i++)
  //    { Serial.print(inByt[i]);  // prints bytes as ascii number
  //      Serial.print(" ");

  //  Serial.write(inByt, 5); // this prints w/ line ends since [4]=10
  //  filtered input for valid data, but not needed here
  //  if (inByt[0] != 0)
  //  { }  moved var reset to ser1.avail

  // black out all
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] =  CRGB(0, 0, 0);

  //turn on the one from input
  leds[c] = CRGB::Red;

  //    leds[0] = CRGB::White;
  //    leds[1] = CRGB::White;

  FastLED.show();  // set colors

  delay(100);

}  // end loop
