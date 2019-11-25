/* FastLEDserialCommand adapted from FastLEDwaveTrain
    Uses one or two flashing Neopixls on Mega, with color values input
    over Serial1 from another device, e.g. Simblee module linked w/
    BLE to phone app. To test sending of byte array. Can mod to get
    byte input from ser mon.

    Uses: FastLED, map, setBrightness,
*/


#include "FastLED.h"
// How many leds to control?
#define NUM_LEDS 2  // ok if I only use one
#define BRIGHTNESS 12
#define DATA_PIN A1  // A1 = 15 on Uno, not on Mega
#define LED_TYPE WS2812B

int R, G, B = 200; // vars used in CRGB call, read from serial, set color
int brite = 200; // to set brightness from serial input
byte inByt[5];  // array of 4-5 bytes input expected

// create leds color array
CRGB leds[NUM_LEDS];

void setup()
{
  pinMode(A1, OUTPUT);  // control signal
  pinMode(A2, OUTPUT);  // 5v power
  pinMode(A3, OUTPUT);  // gnd
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
  // to watch in SerMon prn, may not need
  Serial.begin(9600);
  // commands come in on Ser1
  Serial1.begin(9600);  // 18 = Tx, 19 = Rx

  delay(1000);
  // Uncomment/edit one of the following lines for your led type
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );

}  // end setup

void loop()
{ // void * memset ( void * ptr, int value, size_t num );
  memset(inByt, 0, 5);  // now all null or '0'? 

  // read input to Serial1 from Simblee et. al., put values in color vars
  if (Serial1.available() > 0)
  { delay(20);
    Serial1.readBytes(inByt, 5); // incoming bytes --> inByt array
    Serial.print("inByt using .write ");
    Serial.write(inByt, 5);
    while (Serial1.read() != -1); // clear the input buffer (should be already)
    // give RGB vars the new values;
    R = inByt[0];
    G = inByt[1];
    B = inByt[2];
    brite = inByt[3];
    // print new data in Ser Mon
    for (int i = 0; i < sizeof(inByt); i++)
    { Serial.print(inByt[i]);  // prints bytes as ascii number
      Serial.print(" ");
    }
    Serial.println();
// moved here to stop bright flash on any update
    brite = map(brite, 1, 255, 1, 44); // map input byte range to briteness
    FastLED.setBrightness(brite);
  }  // end ser1.avail
  //  Serial.write(inByt, 5); // this prints w/ line ends since [4]=10
  //  filtered input for valid data, but not needed here
  //  if (inByt[0] != 0)
  //  { }  moved var reset to ser1.avail

  // Turn the lites on, pause, then off; freq = 720 msec/ loop total
  
  // use color params from ser. input
  leds[0] =  CRGB(R, G, B);
  
  leds[1] = CRGB::Red;

  //    leds[0] = CRGB::White;
  //    leds[1] = CRGB::White;

  FastLED.show();  // flash new colors
  delay(240);

  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;

  FastLED.show();

  delay(480);

}  // end loop
