/*
  cannon decorate A -- air cannon uses 8 segments, mod of movement code of
  parade bot's 4 tracks
  initial test writes 4 different colors to segments of 5 lites so we can count each
  segment exact number; 
  todo: write patterns for cannon segments to move depending
  on firing status; write patterns for base segments, moving or not?

  Uno test: simple static pattern works, not moving
*/

#include "FastLED.h"

// color Order here is GRB, not RGB

#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF

// How many leds are in the strip
#define NUM_LEDS 11
#define DATA_PIN 12

CRGB leds[NUM_LEDS];
int count = 0;  // loop count increments -- loop speed affects rate
// of flash and segment movement
int superCount = 0;   // whether flash or alternative when stopped
uint8_t gHue = 0; // rotating "base color" used by alternate pattern
boolean IS_FIRING = true;

void setup()
{
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.setBrightness(60);  // 0-255, use min value needed for environment
  // or FastLED.setMaxPowerInVoltsAndMilliamps(uint8_t volts, uint32_t milliamps}
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);

  // Serial.begin(9600);  // for debugging only

  pinMode( DATA_PIN, OUTPUT );

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = red;   // indicates init to stopped state
  }

  FastLED.show();  // for loop above fills the array; .show sends data to led string

  delay(2000);

}  // end setup

void loop()
{
  // --  later: bools from labView/cRIO to input pins to control action

//    if (superCount > 100)  // 20 loops/sec, so this recycles every 10 sec
//      bpm();
//    else  flash();
  
  // make series of 4 colored segments, each 5 lites long
//  for (int i = 0; i < NUM_LEDS; i++)
//  {  
//    byte idx = i % 20;
//    if (idx >= 15) leds[i] = green;
//    else if (idx >= 10) leds[i] = blue;
//    else if (idx >= 5) leds[i] = yellow;
//   // the rest stay red from setup assignments
//  }

// example moving segment of 10 lites
// moving 10 lite segments (some color, some blk) look like this
// subtracting ct shifts forward
   for (int i = 0; i < 10; i++ )
  {
    if (digitalRead(IS_FIRING))    // true when 'firing' pin pulls high
    {  // need add # > max count, for small i lite numbers to work;
      if ( ((i+10 - count) % 10) < 6 ) // make last # larger to light up more lites
      {
        leds[i] = red;
      }
      else
      {
        leds[i] = black;
      }
    } // end if firing  true
    else  // reverse 'movement' of lite segments
    {
      // adding count offset reverses segment motion
      if ( ((i + count) % 10) < 6 )
      {
        leds[i] = red;
      }
      else
      {
        leds[i] = black;
      }
    }  // end else: reversed
  } // end for moving segments
  
  FastLED.show(); // functions above fill light arrays, this fx sends data to led strip

    count++;  // count increments each loop
//    superCount++;
    if (count > 9)   count = 0;  // should be same # of counts as modulo
//    if (superCount > 200)   superCount = 0;

  delay(50);  // loop delay controls iteration rate, thus speed of all functions that use count
}  // end loop

void bpm()  // back&forth color segments, alternates with flash
{
  // colored stripes pulsing at some Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 60;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  gHue += count;  // somewhat randomizes colors
  for ( int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}  // end bpm

void flash()
{
  if (count < 8)  // ~half the time it will be on
  {
    for (int i = 0; i < 75; i++ )
    {
      leds[i] = red;
    }
    for (int i = 75; i < 149; i++ )
    {
      leds[i] = blue;
    }
    // uncomment if L side lites present
    for (int i = 149; i < 223; i++ )
    {
      leds[i] = yellow;
    }
    for (int i = 223; i < NUM_LEDS; i++ )
    {
      leds[i] = green;
    }
  }   // end flash on
  else   // other half of time it's off
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = black;
    }
  } // end else: flash off
}  // end flash


// cycle lites in 15 lite segments in fwd/reverse direction:
// for each i, (i+14 - count)/ (i - count for larger #s) drops 14->0 as loop increments;
// (this version uses loop count instead of time to increment the 'offset' value from segment end);
// so mod 15 of (i + 14 - count)/ or (i +/- count for large #) also cycles 14 to 0 with time;
// so [ < x] of the 15 lites will be [color] and 'shift forward' each offset increment;
// SUBTRACTING offset simulates forward movement of segments; ADDING offset simulates reverse

// moving 10 lite segments (some color, some blk) look like this, eg barrel side
//   for (int i = 26; i < 60; i++ )
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
//    } // end if R front forward true
//    else  // reverse movement looks like this
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

