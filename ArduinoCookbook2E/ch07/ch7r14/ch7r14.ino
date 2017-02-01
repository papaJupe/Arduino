/*
 * TLC sketch
 * Create a Knight Rider-like effect on LEDs plugged into all the TLC outputs
 * this version assumes one TLC with 16 LEDs
 */

#include "Tlc5940.h"

void setup()
{
  Tlc.init();  // initialize the TLC library
}

void loop()
{
  int direction = 1;
  int intensity = 4095; // an intensity from 0 to 4095, full brightness is 4095
  int dim = intensity / 4;  //  1/4 the value dims the LED
  for (int channel = 0; channel < 16; channel += direction) {
  // the following TLC commands set values to be written by the update method
    Tlc.clear();  // turn off all LEDs
    if (channel == 0) {
      direction = 1;
    }
    else {
      Tlc.set(channel - 1, dim);  // set intensity for prev LED
    }
    Tlc.set(channel, intensity); //  full intensity on this LED
    if (channel < 16){
      Tlc.set(channel + 1, dim);  // set the next LED to dim
    }
    else {
      direction = -1;
    }

    Tlc.update();  // this method sends data to the TLC chips to change the LEDs
    delay(75);
  }
}
      
    
