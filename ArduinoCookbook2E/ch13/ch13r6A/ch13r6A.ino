#include <Wire.h>

const byte LedDrive = 0x38;   // I2C address for 7-Segment 

// lookup array containing segments to light for each digit
const int lookup[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

int count;

void setup()
{
  Wire.begin();        // join I2C bus (address optional for master)
}

void loop()
{
  Wire.beginTransmission(LedDrive);
  Wire.write((byte)0);
  Wire.write(B01000111); // init the 7-segment driver - see data sheet
  Wire.endTransmission();

  // show numbers from 0 to 9999
  for (count = 0; count <= 9999; count++)
  {
    displayNumber(count);
    delay(10);
  }
}

// function to display up to four digits on a 7-segment I2C display
void displayNumber( int number)
{
  number = constrain(number, 0, 9999);
  Wire.beginTransmission(LedDrive);
  Wire.write(1);
  for(int i =0; i < 4; i++)
  {
    byte digit = number % 10;
    {
       Wire.write(lookup[digit]);
    }
    number = number / 10;
  }
  Wire.endTransmission();
}