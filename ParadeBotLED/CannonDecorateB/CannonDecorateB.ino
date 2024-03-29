/*
  cannon decorate B -- for parade bot, tests with 2 long strings, various patterns
  on various boards. 2 dynamic patterns seem possible on Mega, not Uno (2K ram)
  
  initial test writes 4 different colors to 5 lite segments to clarify each
  segment exact number; todo: write patterns for cannon segments to move depending
  on firing status; write patterns for base segments
  v. B tests putting 2 long strings on one controller using
  2 CRGB objects and separate loops, also tried as one long (570) string, both OK
  on Mega, not on Uno if either string is dynamic
*/

#include "FastLED.h"

// color Order here is GRB, not RGB

#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
//#define white 0x101010
//#define purple 0x00FFFF

// # of leds in 1st strip
#define NUM_LEDS 300
#define DATA_PIN 15   // mega can handle 2 moving obj, Uno can't

// 2nd string has its own object on different control pin
#define NUM_LED2 270
#define DATA_PIN2 4

CRGB leds[NUM_LEDS];
CRGB led2[NUM_LED2];

uint8_t count = 0;  // loop count controls things -- loop speed affects rate

void setup()
{
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN2, RGB>(led2, NUM_LED2);

  FastLED.setBrightness(60);  // use min value needed for environment

  // Serial.begin(9600);  // for debugging only

  pinMode( DATA_PIN, OUTPUT );
  pinMode( DATA_PIN2, OUTPUT );

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = projectY;   // indicates init to stopped state
  }

  for (int i = 0; i < NUM_LED2; i++)
  {
    led2[i] = red;   // indicates init to stopped state
  }

  FastLED.show();  // for loop above fills the array; .show sends data to led string
  delay(2000);

}  // end setup

void loop()
{
  //   --  later: each loop gets bools from labView/cRIO via input pins to control
  //    segment movement

  static uint8_t superCount = 0;   // 1st string flash or alternative when stopped
  
  if (superCount > 100)  // 20 loops/sec, so this recycles every 10 sec
    bpm();   // patterns for 1st string
  else  flash();

  // 2nd string static for now
    for (int i = 0; i < NUM_LED2; i++)
    {
      byte idx = i % 20;
      if  (idx >= 15) led2[i] = green;
      else if (idx >= 10) led2[i] = blue;
      else if (idx >= 5) led2[i] = yellow;
      // else  leds[i] = red;  // rest stay red from setup
  
    }
  // test w/ 2nd string moving, works on Mega, not Uno or ProMicro (insuff. RAM)
  //bpm2();

  FastLED.show(); // functions above fill light arrays, .show sends data to led strip

  count++;  // count increments each loop, used in movement functions
  superCount++;  // used to alternate patterns in 1st string
  if (count > 14)   count = 0;  // cycles count 0-->14
  if (superCount > 200)   superCount = 0;

  delay(50);  // loop delay controls iteration rate, thus speed of all functions that use count
}  // end loop

void bpm()  // back&forth color segments, alternates with flash
{
  // colored stripes pulsing at some Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 60;
  uint8_t gHue = 0; // rotating "base color" used by alternate bpm pattern
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  gHue += count;  // somewhat randomizes colors
  for ( int i = 0; i < 300; i++)
  {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}  // end bpm

void bpm2()  // back&forth color segments, alternates with flash
{
  // colored stripes pulsing at some Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 60;
  uint8_t gHue = 0; // rotating "base color" used by alternate bpm pattern
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  gHue += count;  // somewhat randomizes colors
  for ( int i = 0; i < 270; i++ )
  {
    led2[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}  // end bpm2

void flash()
{
  if (count < 8)  // ~half the time it will be lit
  {
    for (int i = 0; i < 75; i++ )
    {
      leds[i] = red;
    }
    for (int i = 75; i < 149; i++ )
    {
      leds[i] = blue;
    }
    //  1 for loop for each parade bot segment
    for (int i = 149; i < 223; i++ )
    {
      leds[i] = yellow;
    }
    for (int i = 223; i < 300; i++ )
    {
      leds[i] = green;
    }
  }   // end flash on
  else   // other half of time it's off
  {
    for (int i = 0; i < 300; i++ )
    {
      leds[i] = black;
    }
  } // end else: flash off
}  // end flash


// to cycle lites in 15 lite segments in fwd/reverse direction:
// for each i, (i+14 - count) or (i - count for larger #s) drops 14->0 as loop increments;
// (AM versions use loop count instead of time to increment the 'offset' value from segment end);
// so mod 15 of (i + 14 - count)/ or (i +/- count for large #) also cycles 14 to 0 with time;
// so [ < x] of the 15 lites will be [color] and 'shift forward' each offset increment;
// SUBTRACTING offset simulates forward movement of segments; ADDING offset simulates reverse

// moving 10 lite segments (some color, some blk) look like this, eg sides of barrel
//   for (int i = 26; i < 60; i++ )   // lite segment affected
//  {
//    if (digitalRead(IS_FIRING) == 0) // true when pin pulls low
//    {
//      if ( ((i + count) % 10) < 7 ) // make last # larger to light up more lites
//      {
//        leds[i] = red;
//      }
//      else
//      {
//        leds[i] = black;
//      }
//    } // end if firing true
//     // reverse movement looks like this; use on  segment opposite side of barrel
//    {
//      // reversing offset (count) reverses segment motion
//      if ( ((i - count) % 10) < 7 )
//      {
//        leds[i] = red;
//      }
//      else
//      {
//        leds[i] = black;
//      }
//    }  // end else: reversed
//  } // end for moving segments

