/*
 * SPI_Max7221_0019
 */

#include <SPI.h>

const int slaveSelect = 10; //pin used to enable the active slave

const int numberOfDigits = 2; // change to match the number of digits wired up
const int maxCount = 99;

int count = 0;

void setup()
{
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
  displayNumber(count);
  count = count + 1;
  if (count > maxCount)
    count = 0;
  delay(100);
}

// function to display up to four digits on a 7-segment display
void displayNumber( int number)
{
  for (int i = 0; i < numberOfDigits; i++)
  {
    byte character = number % 10;  // get the value of the rightmost decade
    // send digit number as command, first digit is command 1
    sendCommand(numberOfDigits-i, character);
    number = number / 10;
  }
}

void sendCommand( int command, int value)
{
  digitalWrite(slaveSelect,LOW); //chip select is active low
  //2 byte data transfer to the 7221
  SPI.transfer(command);
  SPI.transfer(value);
  digitalWrite(slaveSelect,HIGH); //release chip, signal end transfer
}