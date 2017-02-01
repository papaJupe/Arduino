

/* 02 adds analog input from DS's sensing of target position. Lites on the 
 side of target position flash; middle goes solid green when exactly centered.
 Lots of calculation to do, and flashing was erratic before; so AM's revision 
 slows loop, condenses calculation of vars, etc.: seems to speed performance
 but fast flashing still erratic
 */

#include "FastLED.h"

//color of the light
//order is GRB, not RGB
#define projectY 0xFF0099
#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF
#define BRIGHTNESS  48

// not sure why these were here
//#define dis1 0xFF0099    // same as projectY
//#define dis2 0x400080
//#define dis3 0x300060
//#define dis4 0x200040
//#define dis5 0x100020

// How many leds in the strip?
#define NUM_LEDS 160    // for test strip 
#define HALF_NUM 80
#define DATA_PIN 6  // ardu's digital control to strip on D6

#define ENABLED_PIN A2
#define BALL_IN_PIN A0
#define POSITION_PIN A3  // just 3 works, same as A3 when called by AR

CRGB leds[NUM_LEDS], color;   // object created, gets color as param
unsigned long time;
int cycleDuration, halfCycle;

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  //  Serial.begin(9600);

  pinMode(ENABLED_PIN, INPUT_PULLUP);  // A2, switch will pull low, to 0
  pinMode(BALL_IN_PIN, INPUT_PULLUP);  // A0
  pinMode(POSITION_PIN, INPUT);  // A3 analog position indicator, ctr ~= 512

  for (int i = 0; i < NUM_LEDS; i++) // red indicates stopped, not enabled
    leds[i] = red;

  FastLED.show();
  //  Serial.print("Initialized.\n");
  delay (1000);
}  // end setup

void loop()
{
  if (digitalRead(ENABLED_PIN) != 1) // true when enabled, pin pulled low by switch
  {
    enabled();
  } 
  else
  {
    disable();  // all lites stay red when disabled (robot stopped)
  }

  FastLED.show();
  delay(50);
  //   Serial.print("Loop\n");

}  // end loop


void enabled()
{      // if enabled and ball in, then color green and do directional flashing, else blue
  time = millis();
  if (digitalRead(BALL_IN_PIN) != 1)  // true when the ball is in position, pin pulled low
  {    
    // ball is in, now color L/R sides according to position of target
    // when target position near centered (512), cycle is fast; gets longer when farther from ctr
    cycleDuration = map(abs(analogRead(POSITION_PIN)-512), 0, 512, 600, 1800);
    halfCycle = cycleDuration * 0.5;  // may be speedier to calc this once
    color = green; 
    // if very near ctr, make center lites of string solid green
    if ((analogRead(POSITION_PIN) <= 524) && (analogRead(POSITION_PIN) >= 500))
    {
      for (int i = HALF_NUM -30; i <= HALF_NUM + 30; i++)
        leds[i] = color;
    }
    // flash one side where target is, keep other stationary and dark
    if (analogRead(POSITION_PIN) < 500 ) // if target is off ctr to left, flash left side lites 
    {   // color 1st half black
      for (int i = 0; i <= HALF_NUM; i++)
        leds[i] = black;
      // and flash 2nd half of string, slow if far, fast if near
      for (int i = HALF_NUM; i <= NUM_LEDS; i++)
      {  // lights on half the time
        if ( time % cycleDuration < halfCycle)
          leds[i] = color;
        else
          leds[i] = black;
      }
    }  // end if target to left

    if (analogRead(POSITION_PIN) > 524 )   // position is right of ctr
    {   // flash 1st half of string
      for (int i = 0; i <= HALF_NUM; i++)
      {
        if ( time % cycleDuration < halfCycle)
          leds[i] = color;
        else
          leds[i] = black;
      }  // keep other solid dark
      for (int i = HALF_NUM; i <= NUM_LEDS; i++)
      {
        leds[i] = black;
      }
    }  // end if target rt of ctr

  } 
  else //ball is not in place, so color all blue
  {
    for (int i = 0 ; i <= NUM_LEDS; i++)
    {
      //  if (time % 400 < 200)  no good reason to flash
      leds[i] = blue;
      //    else
      //   leds[i] = black;
    }
  }  // end else --  enabled but ball not in

}  // end enabled

void disable()
{
  for (int i = 0; i <= NUM_LEDS; i++)
  {
    leds[i] = red;
  }
} // end disable




