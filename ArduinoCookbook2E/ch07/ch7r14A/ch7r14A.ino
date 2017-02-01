// TLC5940 example sketch
              
#include "Tlc5940.h"

const int sensorPin = 0; // connect sensor to analog input 0

void setup()
{
  Tlc.init();  // initialize the TLC library
}

void loop()
{
  int direction = 1;
  int sensorValue = analogRead(0);  // get the sensor value
  int intensity = map(sensorValue, 0,1023, 0, 4095); // map to TLC range
  int dim = intensity / 4;  //  1/4 the value dims the LED
  for (int channel = 0; channel < NUM_TLCS * 16; channel += direction) {
  // the following TLC commands set values to be written by the update method
    Tlc.clear();  // turn off all LEDs
    if (channel == 0) {
      direction = 1;
    }
    else {
      Tlc.set(channel - 1, dim);  // set intensity for prev LED
    }
    Tlc.set(channel, intensity); //  full intensity on this LED
    if (channel != NUM_TLCS * 16 - 1) {
      Tlc.set(channel + 1, dim);  // set the next LED to dim
    }
    else {
      direction = -1;
    }

    Tlc.update();  // this method sends data to the TLC chips to change the LEDs
    delay(75);
  }
}
      
    
