/*
  04rev5 for paradeBot: moves L/R track 4 segments, rt red / blu,  lt yell/grn, fwd & reverse
  03+ uses loop interation count instead of time for offset increments: code a bit simpler
  and might execute faster
  rev4 adds possible patterns to alternate w/ flash when stopped;
  rev5 same, tuned for bot 4 quadrant lites, rRIO input
*/

#include "FastLED.h"

// color of the light; Order here is GRB, not RGB

#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF

// How many leds are in the strip?
#define NUM_LEDS 298   // 155 for test strip;  full strip 298
#define DATA_PIN 4
#define IS_LEFT_FORWARD_PIN 8
#define IS_RIGHT_FORWARD_PIN 9
#define GOING_PIN 10

CRGB leds[NUM_LEDS];
int count = 0;  // loop count increments -- loop speed affects rate
                // of flash and segment movement
int superCount = 0;   // whether flash or alternative when stopped
uint8_t gHue = 0; // rotating "base color" used by alternate pattern

void setup()
{
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(70);  // use min value needed for environment

  // Serial.begin(9600);  // for debugging only
  // these are INPUT if using HIGH for true, INPUT_PULLUP if using LOW for true
  pinMode( GOING_PIN, INPUT );
  pinMode( IS_RIGHT_FORWARD_PIN, INPUT );
  pinMode( IS_LEFT_FORWARD_PIN, INPUT );
  pinMode( DATA_PIN, OUTPUT );

  digitalWrite(GOING_PIN, LOW);  // uncomment if using HIGH = true below
  digitalWrite(IS_RIGHT_FORWARD, LOW);
  digitalWrite(IS_LEFT_FORWARD_PIN, LOW);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = projectY;   // indicates init to stopped state
  }
  FastLED.show();  // for loop above fills the array; .show sends data to led string
  delay(2000);

}  // end setup

void loop()
{
  // --  each loop gets bools from labView/__RIO via input pins

  if (digitalRead(GOING_PIN) == 1)  // true when pin is HIGH, i.e. we are going
  {
    // when bot is going, these functions cycle R/L lites fwd or reverse
    // depending on status of pins 8 & 9
    rightIterate();
    leftIterate();
  }
  else
  {
    if (superCount > 120)
    {
      bpm();
    }
    else flash();
  }

  FastLED.show(); // functions above fill light arrays, this fx sends data to led strip

  // Serial.print("Loop");

  count++;  // count increments each loop
  superCount++;
  if (count > 14)   count = 0;  // cycles count 0-->14
  if (superCount > 200) superCount = 0; // 20 loops/sec, so this resets every 10 sec

  delay(50);  // loop delay controls iteration rate, thus speed of all functions that use count
}  // end loop

void bpm()  // when not going, alternates with flash
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

void flash()  // does this when not going
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
    for (int i = 223; i < 298; i++ )
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

void rightIterate()
{
  for (int i = 0; i < 75; i++ )   // R side Rear string
  {
    if (digitalRead(IS_RIGHT_FORWARD_PIN) == 1) // true when pin pulls high
    {
      if ( ((i + 14 - count) % 15) < 7 ) // make x, last #, larger to light up more lites
      {
        leds[i] = red;
      }
      else
      {
        leds[i] = black;
      }
    } // end if R forward true

    else
    {
      // ADDING offset (count) reverses segment motion
      if ( ((i + 14 + count) % 15) < 7 )
      {
        leds[i] = red;
      }
      else
      {
        leds[i] = black;
      }
    }  // end else: R rear reversed
  }  // end for Rt rear side lites

  for (int i = 75; i < 149; i++ )   // R side front string
  {
    if (digitalRead(IS_RIGHT_FORWARD_PIN) == 1) // true when pin pulls high
    {
      if ( ((i + count) % 15) < 7 ) // make last # larger to light up more lites
      {
        leds[i] = blue;
      }
      else
      {
        leds[i] = black;
      }
    } // end if R front forward true
    else
    {
      // reversing offset (count) reverses segment motion
      if ( ((i - count) % 15) < 7 )
      {
        leds[i] = blue;
      }
      else
      {
        leds[i] = black;
      }
    }  // end else: R rear reversed
  } // end for front R quadrant
}  // end R iterate

void leftIterate()   // uncomment if L side lites  present
{
  for (int i = 149; i < 223; i++ )  // left front string
  {
    if (digitalRead( IS_LEFT_FORWARD_PIN) == 1 )  // true when pin is pulled HIGH
    {
      if ( ((i - count) % 15) < 7 ) // make last # larger to light up more lites
      {
        leds[i] = yellow;
      }
      else
      {
        leds[i] = black;
      }
    } // end if L front forward
    else   // cycle reverse
    {
      if ( (i + count) % 15 < 7 )
      {
        leds[i] = yellow;
      }
      else
      {
        leds[i] = black;
      }
    }  // end else: L reversed
  }  // end for L front string

  for (int i = 223; i < 298; i++ )  // left side rear string
  {
    if (digitalRead( IS_LEFT_FORWARD_PIN) == 1 )  // true when pin is pulled HIGH
    {
      if ( ((i - count) % 15) < 7 ) // make last # larger to light up more lites
      {
        leds[i] = green;
      }
      else
      {
        leds[i] = black;
      }
    } // end if L rear forward
    else   // cycle reverse
    {
      if ( (i + count) % 15 < 7 )
      {
        leds[i] = green;
      }
      else
      {
        leds[i] = black;
      }
    }  // end else: L rear reversed
  }  // end for L rear string
} // end L iterate

