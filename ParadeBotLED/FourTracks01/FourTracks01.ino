/*
01 just puts 4 different colors on 4 strip segments on robot sides
*/

#include "FastLED.h"

// color of the light
// numeric color order is GRB, not RGB; but we still use RGB param in setup code

#define projectY 0xFF0099

#define red 0x02FF02
#define green 0xFF0000
#define blue 0x0000FF
#define yellow 0xFFFF00
#define black 0x000000
#define white 0x101010
#define purple 0x00FFFF


// How many leds are in the strip?
#define NUM_LEDS 300
#define DATA_PIN 6

CRGB leds[NUM_LEDS], color;

void setup()
{
     // small delay advised to ensure string is awake after powering on
  delay(1500);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  Serial.begin(9600);
  Serial.print("Initialized.\n");

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = blue;

  }
  FastLED.show();
  delay(2000);  // 2 sec. of blue confirms init occurred

}

void loop()
{
  //Serial.print("Loop.\n");
  funcFR();       //front right
  funcFL();       //front left
  funcBR();       //back right
  funcBL();       //back left
   // functions above fill the array, .show sends data to the light string
  FastLED.show();
  delay(200);  // redraw 5 times / sec

  Serial.print("Loop");
}  // end loop

void funcFR(void)
{
  for (int i = 0; i < 74; i++ )
  {
    leds[i] = red;
  }
}

void funcFL(void)
{
  for (int i = 75; i < 149; i++ )
  {
    leds[i] = blue;
  }
}

void funcBR(void)
{
  for (int i = 150; i < 224; i++ )
  {
    leds[i] = yellow;
  }
}

void funcBL(void)
{
  for (int i = 225; i < 300; i++ )
  {
    leds[i] = green;
  }
}

