 /*  flash 2 neopixel LEDs at various brain wave entraining frequencies
    and colors; brightness adjusts with pot. v. B -- wherein he adds
    mode input switch for various freq and color sets; v. C simplifies mode I/O
    code, adds pulse option: flashes on for 'count' setting (eg 5), then off (dark)
    for 'pulse' x cycle length. Eg if count_max = 5 and pulse = 3 and cycle len =
    25 msec, then flash 5 trains for 125 ms, then dark 75 ms (95 actually)
    v. D adds slowdown taper option; edits to reduce irregular briteness changes, 
    helped to use 4/21 for 40 hz; made pulse waits longer; no help from changing
    data pin to 5, not doing AR, not using count, etc. 
    Wiring: BCD switch to D2,3,4; prog # LED's to D9,10,11; lites get power from board 
    gnd/5v; string controlled from A1; pot power from gnd/3.3v, center to A2
 
    Uses: FastLED, analogRead, map, setBrightness, rotary BCD switch/case,
    binary LED output, I/O pin arrays, bitWrite/Read, digitalRead/Write,
    EVERY_N_MILLISECONDS(tInt) function from FastLED lib
*/

#include "FastLED.h"
FASTLED_USING_NAMESPACE
// How many leds to control?
#define NUM_LEDS 2
#define BRIGHTNESS 12
#define DATA_PIN A1
#define LED_TYPE WS2812B
int brite = 0; // sets brightness from pot input during looping
// mode var sets flash freq and color, init/defaults to 0
byte mode = 0;   // variable's 8 bits (max) can be read from switches or other input
// then used to set n LEDs on/off to display val as binary; I only use 0-7, 3 LEDs

byte pinIns[] = {2, 3, 4};      // input pins for n bit binary switch
// binary led display of mode is optional;
byte pinOuts[] = {9, 10, 11}; //  these are [1's,2's,4's] bin output pins to LEDs
//  could use more for more modes; but my 3 bit switch only goes 0-7

// params control loop freq, pulsing, in mS
int liteOn;   // byte never can be > 255
int liteOff;  // so int better here
int pulse = 0;  // how long to pause between trains
boolean slowdown = 0; // whether flash rate slows with time
int tInt = 0;  // slowdown interval; large for slow slowdown

CRGB C; // color that mode sets, ? byte triplets
// create color array for leds
CRGB leds[NUM_LEDS];

void setup()
{
  pinMode(2, INPUT_PULLUP);   // 1s -- digital pins read mode from BCD switch
  pinMode(3, INPUT_PULLUP);   // 2s
  pinMode(4, INPUT_PULLUP);   // 4s
  // optional binary display of mode
  pinMode(9, OUTPUT);    // mode display LEDs 9 = low bit, 10=2s, 11=4s
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  delay(1000);
  // Edit one of these lines for your led string type
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setBrightness( BRIGHTNESS );
   // Serial.begin(9600); 
   // to make slowdown modes work I have to set mode in setup
   // set bits of mode var from inputs, e.g. 3 bit rotary switch
  for (byte i = 0; i < sizeof(pinIns); i++)
  {
    boolean bitOn = digitalRead(pinIns[i]);  // switch ON pulls the pin low 
    bitWrite(mode, i, !bitOn);  // when closed, so I invert value
  }

  // optional, binary display # of mode on LEDs, low endian I/O
  for (byte i = 0; i < sizeof(pinOuts); i++) // or "; i < sizeof(pinOuts);"
  {
    digitalWrite(pinOuts[i], bitRead(mode, i));
  }
  
  // mode number sets lite freq, color, pulse, slowdown rates
  switch (mode)  // byte can be used when int expected
  {
    case 0:  // red, 4/sec
      {
        liteOn = 20;
        liteOff = 230;
        C = CRGB::Red;
        pulse = 0;
        break;
      }
    case 1:  // 3/sec
      {
        liteOn = 20;
        liteOff = 310;
        C = CRGB::Red;
        pulse = 0;
        break;
      }
    case 2:  // red 4 -> 1/sec slowdown in 20", no pulse
      {
        liteOn = 20;
        liteOff = 230;
        C =  CRGB(255, 0, 0);
        pulse = 0;
        slowdown = 1;
        tInt = 1610; // ms interval to increm liteOff
        break;
      }
    case 3:  // 40 hz, blu white, no pulse
      {
        liteOn = 4;
        liteOff = 21;
        C = CRGB(0x99ffff); // sl bluish
        pulse = 0;
        break;
      }
    case 4:  // 40/sec w/ pulse
      {
        liteOn = 4;
        liteOff = 21;
        C = CRGB(0x99ffff); // sl bluish
        pulse = 7;
        break;
      }
    case 5: // fast w/ longer pulsing
      {
        liteOn = 4;
        liteOff = 21;
        C = CRGB(0x99ffff); // sl bluish
        pulse = 14;
        break;
      }
    case 6: // 1 hz red w/o pulsing
      {
        liteOn = 66;
        liteOff = 433;
        C = CRGB(0xff0000); // red
        pulse = 0;
        break;
      }
    case 7: // 1 hz red w/ pulsing
      {
        liteOn = 66;
        liteOff = 433;
        C = CRGB(0xff0000);
        pulse = 2;
        break;
      }
    default:
      {
        mode = 0;
        pulse = 0;
        break;
      }
  }  // end switch/case

}  // end setup

void loop()
{ // count enables pulsing: flash (count max)-1, then dark for pulse*period
  static byte count = 1;
  if (count == 6) count = 1;

  // adjust brightness with pot
  brite = analogRead(A2);
  brite = map(brite, 1, 666, 1, 48); // map 0-3 v pot range to briteness
  FastLED.setBrightness(brite);

  leds[0] = C; // some color set by mode
  leds[1] = C;

  FastLED.show();
  FastLED.delay(liteOn);  // timing set by mode, no help compared to plain delay()
  
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;

  FastLED.show();
  FastLED.delay(liteOff);   // timing set by mode

  if ( pulse != 0 && count == 5)  // if not 0, delay 'pulse#' * loop duration
  {
    FastLED.delay(pulse * (liteOn + liteOff));
    //  Serial.println ("pulse " + String (pulse));
  } // end if count

  EVERY_N_MILLISECONDS(tInt) // slowdown increments liteOff
  { if (slowdown)
    {
      liteOff += 1;
     // Serial.println ("liteOff " + String (liteOff));
    }
    // else Serial.println ("liteOff " + String (liteOff));
  }  // end if EVERY-N
  
  count++;
}  // end loop
