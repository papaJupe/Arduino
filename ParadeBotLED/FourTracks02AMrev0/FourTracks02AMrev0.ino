/*
 02 adds segments' flash when enabled and stopped, begins iterating functions for pattern
 movement using time;  AM added switch input to test 'going' condition
 */

#include "FastLED.h"

//color of the light
// numeric color order is GRB, not RGB, but we keep RGB in addLeds()


#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF


// How many leds are in the strip?
#define NUM_LEDS 158 // full strip 300
#define DATA_PIN 6

CRGB leds[NUM_LEDS], color;
// bool going = true;
bool going;
bool isRightForward = true;  // we always run fwd when going is true
bool isLeftForward  = false;

unsigned long time = 0;

void setup()
{
  delay(1000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(80);
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);  // to test going condition, can switch this pin to grnd
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = red;  // 2 sec. red confirms initialization
  }
  FastLED.show();  // this sends array data to the string
  Serial.print("Initialized.\n");
  delay(2000);  // delay to see that setup executed

}  // end setup

void loop()
{
   // bools come from labView & cRIO
  time = millis();
  going = digitalRead(A0);
  if (going == 0)  // grounding A0 pin makes condition true
  {
    rightIterate();
    // leftIterate();  not coded here
  } 
  else 
  {
    flash();  
  }

  FastLED.show();
  Serial.print("Loop");
  delay(50);
}  // end loop

void flash()
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
    //    for (int i = 150; i < 225; i++ )
    //    {
    //      leds[i] = yellow;
    //    }
    //    for (int i = 225; i < 300; i++ )
    //    {
    //      leds[i] = green;
    //    }
  } 
  else 
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = black;
    }
  }  // end else
} // end flash

void rightIterate()
{
  if(isRightForward)
  { 
    int cycle = 450;  // every 500 ms, offset goes 0-->14, then repeats
    int offset = (time % cycle) / 30);

    for (int i = 0; i < 150; i++ ) // 1st half of string -- to cycle R side
      // writes a moving segment of 15 lites, some fraction (here 5/15) are blue
    {   //  for each i, (offset + 15) goes 15->29 as time increases;
      // so mod 15 of it cycles 14 to 0; so 5 of the 15
      // will be blue and shift forward each offset increment; subtracting offset
      // simulates forward movement of segment; adding offset simulates reverse
      if( ((i - offset + 15) % 15) < 5 )
      {
        leds[i]=blue;
      } 
      else 
      {
        leds[i]=black;
      }
    }  // end for loop
  } // end if right forward

  else   // need conditions for reverse movement, adding offset instead
  {

  }
}  // end rightInterate

void leftIterate(void)
{

}


