/*
v. 04 improves function of case structure, might need more tuneup on bot, 
here pullup is used on input pins, etc
*/

#include "FastLED.h"

// color of the light
// order is GRB, not RGB
#define projectY 0xFF0099
#define BRIGHTNESS 30  // adjust as needed for environment
//
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
//#define NUM_LEDS 138  //45+22+22+45 on bot
#define NUM_LEDS 160    //for testing strip Mar2016
#define DATA_PIN 6
#define ENABLED_PIN A2
#define BALL_IN_PIN A0
#define POSITION_PIN A3  // analog tracking target loc

CRGB leds[NUM_LEDS], color; // color obj not used here
unsigned long time = 0;
int cycleDuration = 0;
int normalizedPosition = 0;
int count = 0; // to control printout

void setup()
{
  Serial.begin(9600);  // for debugging only
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);


  pinMode(ENABLED_PIN, INPUT_PULLUP);  // A2
  pinMode(BALL_IN_PIN, INPUT_PULLUP);  // A0
  pinMode(POSITION_PIN, INPUT);        // A3
  // color = red;  // indicates stopped robot, not looping, not enabled
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = red;
  }
  FastLED.show();  // for loop fills the array with values, this sends them to the lite string
  Serial.print("Initialized.\n");
  delay(1000);
}

void loop()
{
  time = millis();
  // config to use pin pullups to HIGH, pulldown by switch, 0 when ON
  if (digitalRead(ENABLED_PIN) != 1)
  {
    enabled();
  } 
  else
  {
    disable();
  }

  FastLED.show();
  if (count % 30 == 0)  // print normalized position every 30 loops 
        // -- value only updated when enabled & ball in place
  {
    Serial.println(normalizedPosition);
  }
  count++;
  delay(50);

}  // end loop

void enabled()
{
  // if ball is in place, if target centered then make string solid green
  // otherwise flash the side the target is on and make other side red
  if (digitalRead(BALL_IN_PIN) != 1)                                                       
  {  
    // target position: center OR case 0 to 4 ... tgt on center will make all steady green

    normalizedPosition = map(analogRead(POSITION_PIN), 0, 1024, -100, 100);                                                        // put zero so I dont miss a definitions

    // close to center
    if (abs(normalizedPosition) <= 10 )
    { 
      for (int i = 0; i < NUM_LEDS; i++) 
      {
        leds[i] = green;  // steady green background for whole string
      }
    }   // end if close to center
    
    else  // not dead center,
    {  // so we flash 1/2 of string at various rates to indicate direction and distance
      switch (abs(normalizedPosition) / 20 )   // gives an int between 0 and 4
      {
      case 0:  //  NP between 0 and 19
        cycleDuration = 150;
        break;
      case 1:   // between 20 and 39
        cycleDuration = 300;
        break;
      case 2: 
        cycleDuration = 600;
        break;
      case 3: 
        cycleDuration = 1200;
        break;
      case 4: 
        cycleDuration = 1800;
        break;
      default:
        cycleDuration = 1800;
        break;
      }  // end case list

      for (int i = 0; i < NUM_LEDS; i++) 
      {   // flash all lites with green / black, rate depends on how far from center
        leds[i] = (time % cycleDuration < cycleDuration / 2) ? green : black; 
      }
      if ( normalizedPosition < 0 )  // left of center, make 1/2 of string solid red
        for (int i = 0; i < NUM_LEDS / 2; i++) 
        {  
          leds[i] = red;   
        }
      else  // target is right of center, make other 1/2 of string solid red
      for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) 
      {    
        leds[i] = red;  
      }
    }  // end enabled, ball in place, target not centered

  }  // end if ball in place
  else // bot is enabled but ball is not in place -- make string all yellow
  {
    for (int i = 0 ; i <= NUM_LEDS; i++)
    {
      leds[i] = yellow;
    }
  }  // end else

} // end enabled

void disable()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = projectY;
  }
}  // end disable



