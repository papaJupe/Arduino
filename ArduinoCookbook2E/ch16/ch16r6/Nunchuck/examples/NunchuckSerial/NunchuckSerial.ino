/*
 * WiichuckSerial
 *
 *  Uses Nunchuck library to sends sensor values to serial port
 */

#include <Wire.h>
#include "Nunchuck.h"

void setup()
{
  Serial.begin(9600);
  nunchuckSetPowerpins();
  nunchuckInit(); // send the initialization handshake
  nunchuckRead(); // ignore the first time
  delay(50);
}

void loop()
{
  nunchuckRead();
  Serial.print("H,");       // header
  for(int i=0; i <  5; i++) // print values of accelerometers and buttons
  {
     Serial.print(nunchuckGetValue(wii_accelX+ i), DEC);
     Serial.write(',');
  }
  Serial.println();
  delay(20); // the time in milliseconds between sends
}
    