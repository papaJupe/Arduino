/*
 * MultiRX sketch
 * Receive data from two software serial ports
 */
#include <SoftwareSerial.h>
const int rxpin1 = 2;
const int txpin1 = 3;
const int rxpin2 = 4;
const int txpin2 = 5;

SoftwareSerial gps(rxpin1, txpin1); // gps device connected to pins 2 and 3
SoftwareSerial xbee(rxpin2, txpin2); // xbee device connected to pins 4 and 5

void setup()
{
  xbee.begin(9600);
  gps.begin(4800);
  xbee.listen(); // Set “xbee” to be the active device
}

void loop()
{
  if (xbee.available() > 0) // xbee is active. Any characters available?
  {
    if (xbee.read() == 'y') // if xbee received a 'y' character?
    {
      gps.listen(); // now start listening to the gps device

      unsigned long start = millis(); // begin listening to the GPS
      while (start + 100000 > millis())
      // listen for 10 seconds
      {
        if (gps.available() > 0) // now gps device is active
        {
          char c = gps.read();
          // *** process gps data here
        }
      }
      xbee.listen(); // After 10 seconds, go back to listening to the xbee
    }
  }
}
