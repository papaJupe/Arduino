#include <Wire.h>

const byte TMP75_ID = 0x49; // address of the first TMP75

const byte NumberOfFields  = 2; // the number of fields (bytes) to request
const byte NumberOfDevices = 2; // nbr TMP75 devices with consecutive addresses


char tempHighByte;  // high byte of temperature (this is 
                    // the signed integer value in degrees c)
char tempLowByte;   // low byte of temperature  (this is 
                    // the fractional temperature)

float temperature;  // this will hold the floating-point temperature

void setup()  {
  Serial.begin(9600);
  Wire.begin();

  for (int i=0; i < NumberOfDevices; i++)
  {
    Wire.beginTransmission(TMP75_ID+i);
    Wire.write(1);
    // set default configuration, see data sheet for significance of config bits
    Wire.write((byte)0); 
    Wire.endTransmission();

    Wire.beginTransmission(TMP75_ID+i);
    Wire.write((byte)0);   // set pointer register to 0 (this is the 12-bit temperature)
    Wire.endTransmission();
  }
}

void loop()
{
  for (int i=0; i < NumberOfDevices; i++)
  {
    byte id = TMP75_ID + i;   // address IDs are consecutive
    Wire.requestFrom(id, NumberOfFields);
    tempHighByte = Wire.read();
    tempLowByte = Wire.read();
    Serial.print(id,HEX); // print the device address
    Serial.print(": integer temperature is ");
    Serial.print(tempHighByte, DEC);
    Serial.print(",");

    // least significant 4 bits of LowByte is the fractional temperature
    int t = word( tempHighByte, tempLowByte) / 16 ;
    temperature = t / 16.0; // convert the value to a float
    Serial.println(temperature);
  }
  delay(1000);
}