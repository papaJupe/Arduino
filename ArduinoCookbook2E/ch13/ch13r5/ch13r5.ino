/*
 * I2C_Temperature sketch
 * I2C access the TMP75 digital Thermometer
 */

#include <Wire.h>

const byte TMP75_ID = 0x49; // address of the TMP75
const byte NumberOfFields = 2; // the number of fields (bytes) to request

// high byte of temperature (this is the signed integer value in degrees c)
char tempHighByte;
// low byte of temperature  (this is the fractional temperature)  
char tempLowByte;   

float temperature;  // this will hold the floating-point temperature


void setup()  {
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(TMP75_ID);
  Wire.write(1);           // 1 is the configuration register
  // set default configuration, see data sheet for significance of config bits
  Wire.write((byte)0);
  Wire.endTransmission();

  Wire.beginTransmission(TMP75_ID);
  Wire.write((byte)0);   // set pointer register to 0 (the 12-bit temperature)
  Wire.endTransmission();

}

void loop()
{
    Wire.requestFrom(TMP75_ID, NumberOfFields);
    tempHighByte = Wire.read();
    tempLowByte = Wire.read();
    Serial.print("Integer temperature is ");

    Serial.print(tempHighByte, DEC);
    Serial.print(",");

    // least significant 4 bits of LowByte is the fractional temperature
    int t = word( tempHighByte, tempLowByte) / 16 ;
    temperature = t / 16.0; // convert the value to a float
    Serial.println(temperature);
    delay(1000);
}