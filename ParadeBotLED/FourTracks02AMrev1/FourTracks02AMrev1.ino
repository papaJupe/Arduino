/*
02AMrev1 expands time-based cycling for both sides, adds reverse, still moving one color /side, configs input pins for pull-down by switch
 */
#include "FastLED.h"

//color of the light
// hex color order is GRB, not RGB
#define projectY 0xFF0099

#define dis1 projectY

#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF

// How many leds are in the strip?
#define NUM_LEDS 300
#define DATA_PIN 2

#define IS_LEFT_FORWARD_PIN 8
#define IS_RIGHT_FORWARD_PIN 9
#define GOING_PIN 10

CRGB leds[NUM_LEDS], color;

unsigned long time = 0;

void setup()
{   
  delay(1000);
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(80);

  pinMode( GOING_PIN, INPUT_PULLUP);  // to stop going condition, can switch this pin to grnd
  pinMode( IS_RIGHT_FORWARD_PIN, INPUT_PULLUP );  // change these to INPUT if using high signal
  pinMode( IS_LEFT_FORWARD_PIN, INPUT );

  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = red;
  }
  FastLED.show();
  Serial.print("Initialized.\n");
  delay(2000);

}  // end setup

void loop()
{
    // loop gets bools from labView / cRIO no distinct fx needed
  time = millis();

  if (digitalRead(GOING_PIN))   // defaults to ON (pullup) for testing 
  {
    rightIterate();
    leftIterate();
  } 
  else 
  {
    flash();
  }

  FastLED.show();  // functions above filled arrays, this sends data to strip
  Serial.print("Loop");
  delay(50);
  
}  // end loop

void flash(void)
{
  if (time % 600 < 300)
  {
    for (int i = 0; i < 75; i++ )
    {
      leds[i] = red;
    }
    for (int i = 75; i < 150; i++ )
    {
      leds[i] = blue;
    }
    for (int i = 150; i < 225; i++ )
    {
      leds[i] = yellow;
    }
    for (int i = 225; i < 300; i++ )
    {
      leds[i] = green;
    }
  }   // end if -- flash on
  else 
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = black;
    }
  }  // end else -- flash off
}  // end flash

void rightIterate()
{   //  cycles strip segments fwd or reverse
  int cycle = 450;
  int offset = (time % cycle) / 30);
  
  if (digitalRead(IS_RIGHT_FORWARD_PIN))
  {
    for (int i = 0; i < 150; i++ ) // 1st half of string -- to cycle R side
      // writes a moving segment of 15 lites, some fraction (x/15) are blue
    {   // (1st # goes 1,0,14,13... so mod 15 cycles 14 to 0; so 5 of the 15
      // will be blue and shift forward each offset increment)
      if( (i - offset + 15) % 15 < 5 )
      {
        leds[i]=blue;
      } 
      else 
      {
        leds[i]=black;
      }
    }  // end for loop
  }   // end if: R forward
  else
  {   // ADDING offset causes reversal of movement
    for (int i = 0; i < 150; i++ )
    {
      if ( (i + offset) % 15 < 5 )
      {
        leds[i] = blue;
      } 
      else
      {
        leds[i] = black;
      }
    }  // end for
  }  // end else: R reversed
}  // end right iterate

void leftIterate(void)
{   //  cycles left side fwd or rev
  int cycle = 450;
  long offset = (time % cycle) / 30);
  if (digitalRead(IS_LEFT_FORWARD_PIN))
  {
    for (int i = 150; i < 300; i++ )
    {
      if ( (i - offset + 15) % 15 < 10 )
      {
        leds[i] = red;
      } 
      else
      {
        leds[i] = black;
      }
    }
  }  // end if: L forward 
  else  
  {   // ADDING offset causes reversal of movement
    for (int i = 150; i < 300; i++ )
    {
      if ( (i + offset) % 15 < 10 )
      {
        leds[i] = red;
      } 
      else
      {
        leds[i] = black;
      }
    }
  }  // end else: L reversed
} // end L iterate








