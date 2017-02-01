#include <FrequencyTimer2.h>  // include this library to handle the refresh

byte pins[] = {2,3,4};
const int NUMBER_OF_PINS = sizeof(pins)/ sizeof(pins[0]);
const int NUMBER_OF_LEDS = NUMBER_OF_PINS * (NUMBER_OF_PINS-1);

byte pairs[NUMBER_OF_LEDS/2][2] = { {0,1}, {1,2}, {0,2} };

int ledStates = 0; //holds states for up to 15 LEDs
int refreshedLed;  // the LED that gets refreshed

void setup()
{
  FrequencyTimer2::setPeriod(20000/ NUMBER_OF_LEDS); // set the period
  // the next line tells FrequencyTimer2 the function to call (ledRefresh)
  FrequencyTimer2::setOnOverflow(ledRefresh);
  FrequencyTimer2::enable();
}

void loop()
{
  const int analogInPin = 0; // Analog input pin connected to the variable resistor

  // here is the code from the bargraph recipe
  int sensorValue = analogRead(analogInPin);          // read the analog in value
  int ledLevel = 
    map(sensorValue, 0, 1023, 0, NUMBER_OF_LEDS);    // map to the number of LEDs
  for (int led = 0; led < NUMBER_OF_LEDS; led++)
  {
    if (led < ledLevel ) {
      setState(led, HIGH);     // turn on pins less than the level
    }
    else {
      setState(led, LOW);      // turn off pins higher than the level
    }
  }
   // the LED is no longer refreshed in loop, it's handled by FrequencyTimer2
}

// the remaining code is the same as the previous example
      
    
