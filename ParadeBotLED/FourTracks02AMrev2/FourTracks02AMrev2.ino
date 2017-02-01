/*
02AMrev2, moves L/R track segments, still using one color fwd / reverse, time cycling; AM rev added cleanup only, setup for offline testing, not bot
*/
#include "FastLED.h"

//color of the light
//Order is GRB, not RGB
#define projectY 0xFF0099

#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF

// How many leds are in the strip?
#define NUM_LEDS 148
#define DATA_PIN 4
#define GOING_PIN 14
#define IS_RIGHT_FORWARD_PIN 18
#define IS_LEFT_FORWARD_PIN 8   // not used here

CRGB leds[NUM_LEDS], color;

unsigned long time = 0;

void setup()
{   
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(80);  // use min value needed for environment

  Serial.begin(9600);  // to debug only

  pinMode( GOING_PIN, INPUT_PULLUP);
  pinMode( IS_RIGHT_FORWARD_PIN, INPUT_PULLUP );
  pinMode( IS_LEFT_FORWARD_PIN, INPUT_PULLUP );
  pinMode( DATA_PIN, OUTPUT);  

  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = red;  // indicates init to stopped state
  }
  FastLED.show();
  Serial.print("Initialized.\n");
  delay(2000);

}  // end setup

void loop()
{
  // loop gets bools from labView/cRIO via input pins
  time = millis();

  if (digitalRead(GOING_PIN) == 0)   // true when grounding switch pulls pin low
  {
    rightIterate();
    // leftIterate();  disabled here
  } 
  else 
  {
    flash();
  }

  FastLED.show(); // functions above fill light arrays, this sends data to strip

  Serial.print("Loop");
}  // end loop
 
void flash(void)  // lites flash if not 'going'
{
  if (time % 600 < 300)
  {
    for (int i = 0; i < 75; i++ )
    {
      leds[i] = red;
    }
    for (int i = 75; i < 149; i++ )
    {
      leds[i] = blue;
    }
//    for (int i = 149; i < 223; i++ )
//    {
//      leds[i] = yellow;
//    }
//    for (int i = 223; i < 298; i++ )
//    {
//      leds[i] = green;
//    }
  }   // end flash on
  else 
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = black;
    }
  } // end else: flash off
}  // end flash

void rightIterate(void)
{
  int cycle = 450;
  long offset = (time % cycle) / 30);
  if (digitalRead(IS_RIGHT_FORWARD_PIN))
  {  // cycle R side lites in 15 lite segments in fwd direction
    for (int i = 0; i < 149; i++ )
    {
      if ( (i - offset + 15) % 15 < 10 )
      {
        leds[i] = blue;
      } 
      else
      {
        leds[i] = black;
      }
    }  // end for
  } // end if R forward
  else
  {  // ADDING offset reverses segment motion
    for (int i = 0; i < 149; i++ )
    {
      if ( (i + offset) % 15 < 10 )
      {
        leds[i] = blue;
      } 
      else
      {
        leds[i] = black;
      }
    }  // end for
  }  // end else: R reversed
}  // end R iterate

void leftIterate(void)
{
//  int cycle = 450;
//  long offset = (time % cycle) / 30);
//  if (digitalRead(IS_LEFT_FORWARD_PIN))
//  {
//    for (int i = 149; i < 298; i++ )
//    {
//      if ( (i - offset + 15) % 15 < 10 )
//      {
//        leds[i] = red;
//      } 
//      else
//      {
//        leds[i] = black;
//      }
//    }
//  } // end if L forward
//  else
//  {
//    for (int i = 149; i < 298; i++ )
//    {
//      if ( (i + offset) % 15 < 10 )
//      {
//        leds[i] = red;
//      } 
//      else
//      {
//        leds[i] = black;
//      }
//    }
//  }  // end else L reversed
} // end L iterate



