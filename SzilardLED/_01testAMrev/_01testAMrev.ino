
#include <FastLED.h>

// some custom colors
// Order is GRB, not RGB in following hex definitions
#define projectY 0xFF0099
// not used here
//#define dis1 projectY
//#define dis2 0x400080
//#define dis3 0x300060
//#define dis4 0x200040
//#define dis5 0x100020

#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF

// How many leds are in the strip?
//#define NUM_LEDS 134  //45+22+22+45
#define NUM_LEDS 60    // for testing strip
#define DATA_PIN 6  // ardu digital output to strip
#define POSITION_PIN A5  // analog
#define ENABLED_PIN A2
#define BALL_IN_PIN A0   


CRGB leds[NUM_LEDS], color;  // leds is an object of type CRGB, color is param sent to it
// unsigned long time = 0;

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin(9600);
  // this makes sensing pins high by default; closing switch grounds, val to 0
  pinMode(BALL_IN_PIN, INPUT_PULLUP);  // A0
  pinMode(ENABLED_PIN, INPUT_PULLUP);  // A2
  pinMode(POSITION_PIN, INPUT); // A5


  // init all lites to one color
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = red;   // red indicates robot is stopped
  }
  FastLED.show();
  Serial.print("Initialized.\n");

}  // end setup

void loop()
{
  //  time = millis();

  if(digitalRead(ENABLED_PIN) != 1) // A2 switch is closed, pin is grounded
  {
    enabled();
  } 
  else // set all red showing robot disabled
  {
    for (int i = 0; i < NUM_LEDS; i++) 
    {
      leds[i] = red;
    }
  } // end else


  FastLED.show();
  delay(50);
  Serial.print("Loop\n");
} // end loop


void enabled(void)
{  // if enabled, then if A0 closed, show green, else blue
  color = (digitalRead(BALL_IN_PIN) != 1) ? green : blue;
  // not doing anything with position pin or flashing in this test
  // int cycleDuration = map(analogRead(POSITION_PIN), 0, 1024, 10, 1000);
  // if( analogRead(POSITION_PIN) )

  for(int i = 0; i < NUM_LEDS ; i++)
  {
    leds[i] = color;
  }

} // end enabled



