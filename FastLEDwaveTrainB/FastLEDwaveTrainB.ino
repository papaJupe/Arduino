/* Flash 2 neopixel LEDs at various brain wave entraining frequencies
    and colors; brightness adjusts with pot. v. B -- wherein he adds
    mode input switch for various freq and color sets
    Uses: FastLED, analogRead, map, setBrightness, rotary BCD switch input
    binary LED output, I/O pin arrays, switch/case, bitWrite/Read, digitalRead/Write
*/

#include "FastLED.h"
FASTLED_USING_NAMESPACE
// How many leds to control?
#define NUM_LEDS 2
#define BRIGHTNESS 12
#define DATA_PIN 15  // A1
#define LED_TYPE WS2812B
int brite = 0; // sets brightness from pot input during looping
// mode var sets flash freq and color, init/defaults to 0
byte mode = 0;   // variable's 8 bits (max) can be read from switches or other input
// then used to set n LEDs on/off to display int as binary
// could elim these arrays?
byte pinIns[] = {2, 3, 4};      // input pins for n bit binary switch
// binary display of mode is optional;
byte pinOuts[] = {9, 10, 11}; //  these are [0,1,2,n] output pins to LEDs
//  for 1s,2s,4s,8s; my 3 bit switch only goes 0-7

// 2 params control loop freq
byte liteOn;   // will never be > 254 ms, so changed in later v.'s
byte liteOff;
byte pulse;  // some cycling var, not used yet

// create leds object
CRGB leds[NUM_LEDS];

void setup()
{
  pinMode(2, INPUT_PULLUP);   // 1s -- digital pins read input from mode switch
  pinMode(3, INPUT_PULLUP); // 2s
  pinMode(4, INPUT_PULLUP);   // 4s
  // optional binary display of mode
  pinMode(9, OUTPUT);    // pins light LEDs, 9 = low bit, 10 = 2s, 11=4s
  pinMode(10, OUTPUT);   // could use loop to config I/O pins from arrays
  pinMode(11, OUTPUT);

  delay(1500);
  // Uncomment/edit one of the following lines for your led type
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );

}  // end setup

void loop()
{ 
  static byte count = 0; // enables pulsing lites off every n flashes
  if (count == 6) count = 0; // never go above n
  // set bits of mode var from inputs, e.g. 3 bit rotary switch
  for (byte i = 0; i < sizeof(pinIns); i++)
  { 
    boolean bitOn = digitalRead(pinIns[i]);  // switch pulls the pin low if it's closed
    bitWrite(mode, i, !bitOn);
  }

  // optional binary display mode var on n LEDs, low endian I/O
  for (byte i = 0; i < sizeof(pinOuts); i++) // or "; i < sizeof(pinOuts);"
  {
    digitalWrite(pinOuts[i], bitRead(mode, i));
  }
     // adjust brightness with pot
  brite = analogRead(A2);
  brite = map(brite, 1, 666, 1, 44); // map 0-3 v pot range to briteness
  FastLED.setBrightness(brite);
  
  // mode number sets lite freq, color, pulse rate
  switch (mode)  // byte can be used when int expected
  {
    case 0:  // red, 4/sec
      {
        liteOn = 50;
        liteOff = 200;
        leds[0] = CRGB::Red;
        leds[1] = CRGB::Red;
        break;
      }
    case 1:  // 5/sec
      {
        liteOn = 40;
        liteOff = 160;
        leds[0] = CRGB::Red;
        leds[1] = CRGB::Red;
        break;
      }
    case 2:  // 4/sec w/ pulse
      {
        liteOn = 50;
        liteOff = 200;
        leds[0] = 0xff0000;  // red
        leds[1] = 0xff0000;
        pulse = 3;
        break;
      }
    case 3:  // fast, white, 40/sec
      {
        liteOn = 5;
        liteOff = 20;
//        leds[0] = CRGB::White;
//        leds[1] = CRGB::White;
        leds[0] = 0x99ffff; // sl bluish
        leds[1] = 0x99ffff;
        break;
      }
    case 4:  // 50/sec
      {
        liteOn = 4;
        liteOff = 16;
        leds[0] = 0x99ffff; // sl bluish
        leds[1] = 0x99ffff;
        break;
      }
    case 5: // mode 3 w/ pulsing
      {
        liteOn = 5;
        liteOff = 20;
        leds[0] = 0x99ffff; // sl bluish
        leds[1] = 0x99ffff;
        pulse = 5;
        break;
      }
    default:
      {
        mode = 0;
        break;
      }
  }  // end switch /case

  FastLED.show();
  delay(liteOn);

  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;

  FastLED.show();
  
  delay(liteOff);
  
  if (count == 5 && pulse != 0)  // delay length of 3 loops
  {
    delay((liteOn+liteOff) * pulse);
  }

  count++;
}  // end loop



