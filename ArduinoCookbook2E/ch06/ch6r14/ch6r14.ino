// A simple sketch to detect the Southern Hemisphere
// Assumes: LED on pin 13, GPS connected to Hardware Serial pins 0/1
#include "TinyGPS.h"

TinyGPS gps; // create a TinyGPS object

#define HEMISPHERE_PIN 13

void setup()
{
  Serial.begin(4800); // GPS devices frequently operate at 4800 baud
  pinMode(HEMISPHERE_PIN, OUTPUT);
  digitalWrite(HEMISPHERE_PIN, LOW); // turn off LED to start
}
void loop()
{
  while (Serial.available())
  {
    int c = Serial.read();
    // Encode() each byte
    // Check for new position if encode() returns "True"
    if (gps.encode(c))
    {
      long lat, lon;
      gps.get_position(&lat, &lon);
      if (lat < 0) // Southern Hemisphere?
        digitalWrite(HEMISPHERE_PIN, HIGH);
      else
        digitalWrite(HEMISPHERE_PIN, LOW);
    }
  }
}
