/*
* i2cDebug
* example sketch for i2cDebug library
*/

#include <Wire.h>    // the Arduino I2C library
#include <i2cDebug.h>

const int address = 4;    //the address to be used by the communicating devices
const int sensorPin = 0;  // select the analog input pin for the sensor
int val;                  // variable to store the sensor value


void setup()
{
  Serial.begin(9600);
  i2cDebug.begin(address);
}

void loop()
{
  // read the voltage on the pot(val ranges from 0 to 1023)
  val = analogRead(sensorPin);        
  Serial.println(val);
  i2cDebug.println(val);
}
  
