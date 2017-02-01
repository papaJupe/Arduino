/*
  03AMrev0 uses loop iteration count instead of time for motion simulation -- may be 
  quicker calc and simpler to understand; 2 pull-down switches for going and R 
  forward/reverse, one color, one side, for offline testing, not bot
 */

#include "FastLED.h"

// numeric color order is GRB, not RGB

#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF


// How many leds are in the strip?  must be >= largest number in filling fx's
#define NUM_LEDS 100 // test strip; full strip ~300
#define DATA_PIN 4

CRGB leds[NUM_LEDS];  // was + color;
// bool going = true;
bool going = false;
bool rightForward = false;  // run fwd when going is true, else reverse;
bool leftForward  = false;  //  could add switch and input to control this too
int count = 0;  // counts loop iterations, used in offset calc instead of time

void setup()
{
  delay(1000);
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(60);
  // Serial.begin(9600);  for debug
  pinMode(A0, INPUT_PULLUP);  // to test going condition, can switch this pin to grnd
  pinMode(A4, INPUT_PULLUP);   // pullup needed if you switch to gnd for true
  
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = CRGB::Yellow; // or CRGB(red) or just red;  // 2 sec. red confirms init
  }
  FastLED.show();  // this sends array data to the lite string
  // Serial.print("Initialized.\n");
  delay(2000);  // delay to see that setup executed

}  // end setup

void loop()
{
  //update Inputs:   bools come from labView & cRIO

  //  first loop count (global) = 0, set in init
  going = digitalRead(A0);
  rightForward = digitalRead(A4);
  
  if (going == 0)  // grounding A0 pin makes condition true
  {
    rightIterate();
     // leftIterate();  // not coded here
  } 
  else 
  {
    flash();  
  }

  FastLED.show();
  count++;
  if (count > 14) count = 0;  // cycles count 0-14, increments 1 each 50 ms loop
  // Serial.print("Loop");
  delay(40);   // adjust loop delay for faster or slower flashing and segment movement
}  // end loop

void flash()
{
  // was if (time % 600 < 300)
  if (count < 8)  // ~half the time it will be on
  {
    for (int i = 0; i < 75; i++ )
    {
      leds[i] = red;
    }
    for (int i = 75; i < NUM_LEDS; i++ )
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
  else   // other half of the time
  {
    for (int i = 0; i < NUM_LEDS; i++ )
    {
      leds[i] = black;
    }
  }  // end else
} // end flash

void rightIterate()
{
  for (int i = 0; i < 98; i++ ) // 1st half of string -- to cycle R side -->
    // writes a moving segment of 15 lites, some fraction (here 5/15) are blue;
    // for each i, (14 - count) drops 14->0 as loop increments count;
    // so mod 15 of (i + 14 - count) also cycles 14 to 0 with time; so 5 of the 15
    // will be blue and 'shift forward' each offset increment; subtracting offset
    // simulates forward movement of segment; adding offset simulates reverse
  {
    if(rightForward == 0)   // grounding switch pulls to 0 (true condition here)
    {
      if( ((i + 14 - count) % 15) < 5 )  // if >, then fraction on is 10/15
            // if < 5 then 5/15 light up; incr number to light more lights
      {
        leds[i]=blue;
      } 
      else 
      {
        leds[i]=black;
      }
    } // end if R forward 

    else   // for reverse movement
    {  // ADDING offset simulates reverse
      if( ((i + 14 + count) % 15) > 5 )
      {
        leds[i]=blue;
      } 
      else 
      {
        leds[i]=black;
      }
    } //  end else right reverse

  } // end for: R lights

}  // end rightInterate

void leftIterate()
{

}






