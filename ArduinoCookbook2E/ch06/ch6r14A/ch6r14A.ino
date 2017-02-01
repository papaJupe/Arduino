// Another simple sketch to detect the Southern Hemisphere
// Assumes: LED on pin 13, GPS connected to pins 2/3
// (Optional) Serial debug console attached to hardware serial port 0/1

#include "TinyGPS.h"
#include "SoftwareSerial.h"

#define HEMISPHERE_PIN 13
#define GPS_RX_PIN 2
#define GPS_TX_PIN 3

TinyGPS gps; // create a TinyGPS object
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN); // create soft serial object

void setup()
{
  Serial.begin(9600); // for debugging
  ss.begin(4800); // Use Soft Serial object to talk to GPS
  pinMode(HEMISPHERE_PIN, OUTPUT);
  digitalWrite(HEMISPHERE_PIN, LOW); // turn off LED to start
}
void loop()
{
  while (ss.available())
  {
    int c = ss.read();
    Serial.write(c); // display NMEA data for debug
    // Send each byte to encode()
    // Check for new position if encode() returns "True"
    if (gps.encode(c))
    {
      long lat, lon;
     unsigned long fix_age;
    gps.get_position(&lat, &lon, &fix_age);
    if (fix_age == TinyGPS::GPS_INVALID_AGE )
      Serial.println("No fix ever detected!");
    else if (fix_age > 2000)
      Serial.println("Data is getting STALE!");
    else
      Serial.println("Latitude and longitude valid!");
      
      Serial.print("Lat: "); 
      Serial.print(lat);
      Serial.print(" Lon: "); 
      Serial.println(lon);
      if (lat < 0) // Southern Hemisphere?
        digitalWrite(HEMISPHERE_PIN, HIGH);
      else
        digitalWrite(HEMISPHERE_PIN, LOW);
    }
  }
}
