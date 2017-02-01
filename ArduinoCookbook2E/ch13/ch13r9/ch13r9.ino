/*
 * I2C_Master
 * Echo Serial data to an I2C slave
 */

#include <Wire.h>

const int address = 4;  //the address to be used by the communicating devices

void setup()
{
   Wire.begin();
}


void loop()
{
  char c;
  if(Serial.available() > 0 )
  {
     // send the data
     Wire.beginTransmission(address); // transmit to device
     Wire.write(c);
     Wire.endTransmission();
  }
}