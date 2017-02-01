/*
 * PSX sketch
 *
 * Display joystick and button values
 * uses PSX library written by Kevin Ahrendt
 * http://www.arduino.cc/playground/Main/PSXLibrary
 */

// the Psx library may need porting to 1.0 (see Library chapter)
#include <Psx.h>                       // Includes the Psx Library

Psx Psx;                               // Create an instance of the Psx library
const int dataPin  = 5;
const int cmndPin  = 4;
const int attPin   = 3;
const int clockPin = 2;
const int psxDelay = 50;         // determine the clock delay in microseconds

unsigned int data = 0;           // data stores the controller response

void setup()
{
  // initialize the Psx library
  Psx.setupPins(dataPin, cmndPin, attPin, clockPin, psxDelay);
  Serial.begin(9600); // results will be displayed on the Serial Monitor
}


void loop()
{
  data = Psx.read();        // get the psx controller button data

  // check the button bits to see if a button is pressed
  if(data & psxLeft)
    Serial.println("left button");
  if(data & psxDown)
    Serial.println("down button");
  if(data & psxRight)
    Serial.println("right button");
  if(data & psxUp)
    Serial.println("up button");
  if(data & psxStrt)
    Serial.println("start button");
  if(data & psxSlct)
    Serial.println("select button");

  delay(100);
}
      
    
