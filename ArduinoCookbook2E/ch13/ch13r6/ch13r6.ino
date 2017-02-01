/*
 * I2C_7Segment sketch
 */

#include <Wire.h>

const byte LedDrive = 0x38;   // I2C address for 7-Segment 

int segment,decade;

void setup()  {
  Serial.begin(9600);
  Wire.begin();          // Join I2C bus 

  Wire.beginTransmission(LedDrive);
  Wire.write((byte)0);
  Wire.write(B01000111); // show digits 1 through 4, use maximum drive current
  Wire.endTransmission();
}

void loop()
{
  for (segment = 0; segment < 8; segment++)
  {
    Wire.beginTransmission(LedDrive);
    Wire.write(1);
    for (decade = 0 ; decade < 4; decade++)
    {
      byte bitValue = bit(segment);
      Wire.write(bitValue);
    }
    Wire.endTransmission();
    delay (250);
  }
}