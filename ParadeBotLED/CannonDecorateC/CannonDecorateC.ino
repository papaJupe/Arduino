/*
  cannon decorate C --   v. C writes orig. paradeBot base pattern, plus dynamic
  pattern to cannon string, using  2 CRGB objects and separate control pins.

  2 dynamic patterns are possible on Mega, not Uno (2K ram too small)

  initial test used 4 different colors to 5 lite segments to clarify each
  segment exact number; same pattern now used on cannon lites when not firing
  read some control pin from sidecar (firing T/F);
  write patterns for cannon segments to move depending on firing status;

*/

#include "FastLED.h"

// color Order here is GRB, not RGB

#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xCCCC00
#define black 0x000000

//#define white 0x101010
//#define purple 0x00FFFF

// # of leds in base strip
#define NUM_LEDS 298
#define DATA_PIN 5


// 2nd cannon string on different control pin
#define NUM_LED2 262
#define DATA_PIN2 12

CRGB leds[NUM_LEDS];
CRGB led2[NUM_LED2];

// 4 pins get digital signal from sidecar, which pulls LOW at
// rest, so I use HIGH for true
#define IS_LEFT_FORWARD_PIN 8
#define IS_RIGHT_FORWARD_PIN 9
#define GOING_PIN 10
#define IS_FIRING_PIN 7

uint8_t count = 0;  // loop count controls things -- loop speed affects rate

void setup()
{
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN2>(led2, NUM_LED2);

  FastLED.setBrightness(60);  // use min value needed for environment

  // Serial.begin(9600);  // for debugging only
  pinMode( GOING_PIN, INPUT );
  pinMode( IS_RIGHT_FORWARD_PIN, INPUT );
  pinMode( IS_LEFT_FORWARD_PIN, INPUT );
  pinMode( IS_FIRING_PIN, INPUT );

  digitalWrite(GOING_PIN, LOW);  // low=false, HIGH = true
  digitalWrite(IS_RIGHT_FORWARD_PIN, LOW);
  digitalWrite(IS_LEFT_FORWARD_PIN, LOW);
  digitalWrite(IS_FIRING_PIN, LOW);

  // control (signal) pins for LED strings
  pinMode( DATA_PIN, OUTPUT );
  pinMode( DATA_PIN2, OUTPUT );

  // init all lites to red

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = red;   // indicates init to stopped state
  }

  for (int i = 0; i < NUM_LED2; i++)
  {
    led2[i] = red;   // indicates init to stopped state
  }

  FastLED.show();  // for loops above set colors; .show sends data to led string
  delay(2000);

}  // end setup

void loop()
{
  //   --  bools from labView/cRIO/sidecar --> input pins, control
  //        segment movements

  static uint8_t superCount = 0;   // 1st string flash or alternative when stopped

  if (digitalRead(GOING_PIN) == 1)  // true when pin is HIGH, i.e. we are going
  {
    // when bot is going, these functions cycle R/L lites fwd or reverse
    // depending on status of pins 8 & 9
    rightIterate();
    leftIterate();
  }
  else if (superCount > 120)  bpm();

  else flash();


  if (digitalRead(IS_FIRING_PIN) == 1) // true when pin goes high
  {
    //Segment 1 (Wave Front Right)
    for (int i = 0; i < 24; i++ )  // lite segment affected
    {
      // moving 10 lite segments
      if ( ((i + 10 - count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
    } // end for moving segment 1 (Wave Front Right)

    //Segment 2 (Wave Front Left)
    for (int i = 24; i < 49; i++ )   // lite segment affected

    {
      if ( ((i + count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
    } // end if firing true
    // end for moving segment 2 (Wave Front Left)

    //Segment 3 (Wave Bottom Left)
    for (int i = 49; i < 63; i++ )   // lite segment affected

    {
      led2[i] = green;
    }
    // end for moving segment 3(Wave Bottom Left)
    //Segment 4 (Wave Back Left)
    for (int i = 64; i < 100 ; i++ )   // lite segment affected

    {
      if ( ((i - count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
    } // end if firing true
    // end for moving segment 4(Wave Back Left)

    //Segment 5 (Mini Barrel Left)
    for (int i = 100; i < 117; i++ )   // lite segment affected

    {
      if ( ((i - count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
    } // end if firing true
    // end for moving segment 5 (Mini Barrel Left)


    //Segment 6 (Mini Barrel Right)
    for (int i = 118; i < 138; i++ )   // lite segment affected

    {
      if ( ((i + count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
    } // end if firing true
    // end for moving segment 6

    //Segment 7 (Big Barrel Left)
    for (int i = 137; i < 175; i++ )   // lite segment affected

    {
      if ( ((i - count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
    }
    // end for moving segment 7 (Mini Barrel Right)

    //Segment 8 (Big Barrel Left)
    // moving 10 lite segments (some color, some blk) look like this, eg sides of barrel
    for (int i = 175; i < 214; i++ )   // lite segment affected

    {
      if ( ((i + count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
      // end if firing true
    } // end for moving segment 8 (Big Barrel Left)

    //Segment 9 (Wave Back Right)
    // moving 10 lite segments (some color, some blk) look like this, eg sides of barrel
    for (int i = 214; i < 246; i++ )   // lite segment affected
    {

      if ( ((i + count) % 10) < 7 ) // make last # larger to light up more lites
      {
        led2[i] = red;
      }
      else
      {
        led2[i] = black;
      }
    } // end if firing true
    // end for moving segment 9 (Wave Back Right)

    //Segment 10 (Wave Bottom Right)
    for (int i = 246; i < 260; i++ )   // lite segment affected
    {
      led2[i] = yellow;
    } // end for moving segments 10(Wave Bottom Right)
  }  // end if firing
  else
  {
    //NOT firing -- make series of 4 colored segments, each 5 lites long
    for (int i = 0; i < NUM_LED2; i++)
    {
      byte idx = i % 20;
      if (idx >= 15) led2[i] = green;
      else if (idx >= 10) led2[i] = blue;
      else if (idx >= 5) led2[i] = yellow;
      else led2[i] = red;
    }
  }
  FastLED.show(); // functions above fill light arrays, .show sends data to led strip

  count++;  // count increments each loop, used in movement functions
  superCount++;  // used to alternate patterns in 1st string
  if (count > 9)   count = 0;  // cycles count 0-->9 to match mod 10 (segments of 10 lites)
  if (superCount > 200)   superCount = 0;

  //if (digitalRead(IS_FIRING_PIN) == HIGH){
  //Serial.println("firing");}

  delay(50);  // loop delay controls iteration rate, thus speed of all functions that use count
}  // end loop


void bpm()  // back&forth color segments, alternates with flash on base segments
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
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 262);
  gHue += count;  // somewhat randomizes colors
  for ( int i = 0; i < 254; i++ )
  {
    led2[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}  // end bpm2

void flash()
{
  if (count < 5)  // ~half the time it will be lit
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

// iterate functions set patterns for base segments when "GOING" is true

void rightIterate()
{
  for (int i = 0; i < 73; i++ )   // R side Rear string
  {
    if (digitalRead(IS_RIGHT_FORWARD_PIN) == 1) // true when pin pulls high
    {
      if ( ((i + 9 - count) % 10) < 5 ) // make x, last #, larger to light up more lites
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
      if ( ((i + 9 + count) % 10) < 5 )
      {
        leds[i] = red;
      }
      else
      {
        leds[i] = black;
      }
    }  // end else: R rear reversed
  }  // end for Rt rear side lites

  for (int i = 73; i < 147; i++ )   // R side front string
  {
    if (digitalRead(IS_RIGHT_FORWARD_PIN) == 1) // true when pin pulls high
    {
      if ( ((i + count) % 10) < 5 ) // make last # larger to light up more lites
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
      if ( ((i - count) % 10) < 5 )
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


void leftIterate()   // uncomment if L side lites present
{
  for (int i = 147; i < 221; i++ )  // left front string
  {
    if (digitalRead( IS_LEFT_FORWARD_PIN) == 1 )  // true when pin is pulled HIGH
    {
      if ( ((i - count) % 10) < 5 ) // make last # larger to light up more lites
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
      if ( (i + count) % 10 < 5 )
      {
        leds[i] = yellow;
      }
      else
      {
        leds[i] = black;
      }
    }  // end else: L reversed
  }  // end for L front string

  for (int i = 221; i < 296; i++ )  // left side rear string
  {
    if (digitalRead( IS_LEFT_FORWARD_PIN) == 1 )  // true when pin is pulled HIGH
    {
      if ( ((i - count) % 10) < 5 ) // make last # larger to light up more lites
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
      if ( (i + count) % 10 < 5 )
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

// to cycle lites in 15 lite segments in fwd/reverse direction (use count recycle @ 14):
// for each i, (i+14 - count) or (i - count for larger #s) drops 14->0 as loop increments;
// (AM versions use loop count instead of time to increment the 'offset' value from segment end);
// so mod 15 of (i + 14 - count)/ or (i +/- count for large #) also cycles 14 to 0 with time;
// so [ < x] of the 15 lites will be [color] and 'shift forward' each offset increment;
// SUBTRACTING offset simulates forward movement of segments; ADDING offset simulates reverse

// for 10 lite segments, must use mod 10 and a count that recycles @ 9 (10 values)
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

