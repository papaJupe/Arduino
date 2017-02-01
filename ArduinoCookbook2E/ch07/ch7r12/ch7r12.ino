/*
 *  Max7221_digits
 */

#include <SPI.h>  // Arduino SPI library introduced in Arduino version 0019

const int slaveSelect = 10;   // pin used to enable the active slave
const int numberOfDigits = 2; // change these to match the number 
                              // of digits wired up
const int maxCount = 99;
int number = 0;

void setup()
{
  Serial.begin(9600);
  SPI.begin();   // initialize SPI
  pinMode(slaveSelect, OUTPUT);
  digitalWrite(slaveSelect,LOW);  //select slave
  // prepare the 7221 to display 7-segment data - see data sheet
  sendCommand(12,1);  // normal mode (default is shutdown mode);
  sendCommand(15,0);  // Display test off
  sendCommand(10,8);  // set medium intensity (range is 0-15)
  sendCommand(11,numberOfDigits);  // 7221 digit scan limit command
  sendCommand(9,255); // decode command, use standard 7-segment digits
  digitalWrite(slaveSelect,HIGH);  //deselect slave
}

void loop()
{
  // display a number from the serial port terminated by the end of line
  // character
  if (Serial.available())
  {
     char ch  = Serial.read();
     if( ch == '\n')
     {
       displayNumber(number);
       number = 0;
     }
     else
     number = (number * 10) + ch - '0'; // see Chapter 4 for details
  }
}

// function to display up to four digits on a 7-segment display
void displayNumber( int number)
{
  for (int i = 0; i < numberOfDigits; i++)
  {
    byte character = number % 10;  // get the value of the rightmost decade
    if(number == 0 && i > 0)
       character = 0xf;  // the 7221 will blank the segments when receiving value
    // send digit number as command, first digit is command 1
    sendCommand(numberOfDigits-i, character);
    number = number / 10;
  }
}

void sendCommand(int command, int value)
{
  digitalWrite(slaveSelect,LOW); //chip select is active low
  //2-byte data transfer to the 7221
  SPI.transfer(command);
  SPI.transfer(value);
  digitalWrite(slaveSelect,HIGH); //release chip, signal end transfer
}
      
    
