

/*
  04AMrev4, moves L/R tracks in 4 segments, rt red / blu,  lt yell/grn, fwd & reverse
  03+ uses loop interation count instead of time for offset increments: code a bit simpler
  and might execute faster since calculations are much simpler;  rev4 adds 2 patterns
   that can alternate with flash for variety; offline test setup -- not bot
  04rev5 is same for rRIO/bot pin setup / lites
*/

#include <FastLED.h>

// hex color order is GRB, not RGB, if you config to RGB in  _.addleds

#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF

// How many leds are in the strip? must be >= numbers in functions
#define NUM_LEDS 150   // 155 for test strip;  full strip is 298
#define DATA_PIN 4
#define GOING_PIN 14
#define IS_RIGHT_FORWARD_PIN 18
#define IS_LEFT_FORWARD_PIN 8

CRGB leds[NUM_LEDS];

uint8_t loopCnt = 0;  // count number of loops; count increment and loop speed affects rate
// of flash and segment movement
uint8_t superCount = 0;   //whether to flash or bpm/juggle when stopped
uint8_t gHue = 0; // rotating "base color" used by alt pattern

void setup()
{
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(60);  // use min value needed for environment

  // Serial.begin(9600);  // for debugging only
  // use INPUT if using HIGH for true, INPUT_PULLUP if using LOW for true
  pinMode( GOING_PIN, INPUT_PULLUP );
  pinMode( IS_RIGHT_FORWARD_PIN, INPUT_PULLUP );
  pinMode( IS_LEFT_FORWARD_PIN, INPUT_PULLUP );
  pinMode( DATA_PIN, OUTPUT );
   // uncomment if using HIGH = true below
  //  digitalWrite(GOING + PIN, LOW);  
  //  digitalWrite(IS_RIGHT_FORWARD, LOW);
  //  digitalWrite(IS_LEFT_FORWARD_PIN, LOW);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = red;  // indicates init to stopped state
  }
  FastLED.show();  // for loop above fills the array; .show sends data to led string
  Serial.print("Initialized.\n");
  delay(2000);

}  // end setup

void loop()
{
//  Serial.print("Loop cnt1 "); Serial.println(loopCnt);
//  Serial.print("LoopcntSup1 "); Serial.println(superCount);
//  if (Serial.read() == 'z') exit(1);  // to freeze ser mon to analyse numbers above

  // --  each loop gets bools from labView/cRIO via input pins
  if (digitalRead(GOING_PIN) == 0)  // true when pin is HIGH / LOW, i.e. we are going
  {
    // when bot is going, these functions cycle R/L lites fwd or reverse
    // depending on status of pins for R/L Forward
    rightIterate();
    //  leftIterate();
  }
  else
  {
    if (superCount > 120)
    {
      bpm();   //   or juggle();
    }
    else flash();
  }

  loopCnt++;
  superCount++;

  if (loopCnt > 14)  loopCnt = 0;  // cycles count 0-->14
  if (superCount > 200) superCount = 0; // 20 loops/sec, so this resets every 10 sec

  FastLED.show(); // functions above fill light arrays, this sends data to led strip
  delay(50);  // loop delay controls iteration rate, thus speed of all functions that use count
}  // end loop

void bpm()
{
  // colored stripes pulsing at some Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 60;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  gHue += loopCnt;
  for ( int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
} // end bpm

void flash()  // does this when not 'going'
{
  if (loopCnt < 8)  // ~half the time it will be on
  {
    for (int i = 0; i < 75; i++ )
    {
      leds[i] = red;
    }
    for (int i = 75; i < 149; i++ )
    {
      leds[i] = green;
    }
    // uncomment if L side lites present
    //    for (int i = 149; i < 223; i++ )
    //    {
    //      leds[i] = yellow;
    //    }
    //    for (int i = 223; i < 298; i++ )
    //    {
    //      leds[i] = green;
    //    }
  }   // end flash on
  else   // other half of time it's off
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = black;
    }
  } // end else: flash off

}  // end flash

void juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++)
  {
    leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// cycle lites in 15 lite segments in fwd/back direction:
// for each i, (14 - loopCnt) drops 14->0 as loop increments count (this version
// uses loop count instead of time to increment the 'offset' value from segment end);
// so mod 15 of (i + 14 - loopCnt) also cycles 14 to 0 with time; so 5 of the 15 lites
// will be blue and 'shift forward' each offset increment; SUBTRACTING offset #
// simulates forward movement of segments; ADDING offset simulates reverse movement

void rightIterate()
{
  for (int i = 0; i < 75; i++ )   // R side Rear string
  {
    if (digitalRead(IS_RIGHT_FORWARD_PIN) == 0) // true when pin pulls low/high
    {
      if ( ((i + 14 - loopCnt) % 15) < 7 ) // make last # larger to light up more lites
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
      // ADDING offset (loopCnt) reverses segment motion
      if ( ((i + 14 + loopCnt) % 15) < 7 )
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
    if (digitalRead(IS_RIGHT_FORWARD_PIN) == 0) // true when pin pulls high/low
    {
      if ( ((i + loopCnt) % 15) < 7 ) // make last # larger to light up more lites
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
      // ADDING offset (loopCnt) reverses segment motion
      if ( ((i - loopCnt) % 15) < 7 )
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
  //  for (int i = 149; i < 223; i++ )  // left side front string
  //  {
  //    if (digitalRead( IS_LEFT_FORWARD_PIN) == 1 )  // true when pin is pulled HIGH
  //  {
  //    if ( ((i + 14 - loopCnt) % 15) < 5 ) // make last # larger to light up more lites
  //    {
  //      leds[i] = yellow;
  //    }
  //    else
  //    {
  //      leds[i] = black;
  //    }
  //    } // end if L front forward
  //    else   // cycle reverse
  //    {
  //      if ( (i + 14 + loopCnt) % 15 < 5 )
  //      {
  //        leds[i] = yellow;
  //      }
  //      else
  //      {
  //        leds[i] = black;
  //      }
  //    }  // end else: L reversed
  //  }  // end for L front string

  //  for (int i = 223; i < 298; i++ )  // left side rear string
  //  {
  //    if (digitalRead( IS_LEFT_FORWARD_PIN) == 1 )  // true when pin is pulled HIGH
  //    {
  //      if ( ((i + 14 - loopCnt) % 15) < 5 ) // make last # larger to light up more lites
  //      {
  //        leds[i] = green;
  //      }
  //      else
  //      {
  //        leds[i] = black;
  //  }
  //    } // end if L rear forward
  //    else   // cycle reverse
  //    {
  //      if ( (i + 14 + loopCnt) % 15 < 5 )
  //      {
  //        leds[i] = green;
  //      }
  //      else
  //      {
  //        leds[i] = black;
  //      }
  //    }  // end else: L rear reversed
  //  }  // end for L rear string
} // end L iterate

