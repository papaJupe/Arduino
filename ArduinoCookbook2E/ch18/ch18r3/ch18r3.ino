/*
  pulseTimer2
  pulse a pin at a rate set from serial input
 */

#include <MsTimer2.h>

const int pulsePin = 13;
const int NEWLINE = 10; // ASCII value for newline

int period = 100; // 10 milliseconds
boolean output = HIGH; // the state of the pulse pin

void setup()
{
  pinMode(pulsePin, OUTPUT);
  Serial.begin(9600);

  MsTimer2::set(period/2, flash);
  MsTimer2::start();

  period= 0; // reset to zero ready for serial input
}

void loop()
{
   if( Serial.available())
  {
    char ch = Serial.read();
    if( isDigit(ch) ) // is this an ascii digit between 0 and 9?
    {
       period = (period * 10) + (ch - '0'); // yes, accumulate the value
    }
    else if (ch == NEWLINE)  // is the character the newline character
    {
       Serial.println(period);
       MsTimer2::set(period/2, flash);
       MsTimer2::start();
       period = 0; // reset to 0 ready for the next sequence of digits
    }
  }
}

void flash()
{
  digitalWrite(pulsePin, output);
  output = !output;  // invert the output
}