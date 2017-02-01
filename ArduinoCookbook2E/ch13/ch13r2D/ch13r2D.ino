/*
 * Send comma seperated nunchuck data
 * Uses Nunchuck Library discussed in Recipe 16.5
 * Label string seperated by commas can be used by receiving program to identify fields
 */

#include <Wire.h>
#include "Nunchuck.h"

int offsetX, offsetY, offsetZ; // values to add to the sensor to get zero reading when centered

#include <Wire.h>
#include "Nunchuck.h"
void setup()
{
    Serial.begin(57600);
    nunchuckSetPowerpins();
    nunchuckInit(); // send the initialization handshake
    nunchuckRead(); // ignore the first time
    delay(50);
}

void loop()
{
  nunchuckRead();
  delay(6);
  nunchuck_get_data();
  boolean btnC = nunchuckGetValue(wii_btnC);
  boolean btnZ = nunchuckGetValue(wii_btnZ);
  
  if (btnC) {
    offsetX = 127 - nunchuckGetValue(wii_accelX) ; 
    offsetY = 127 - nunchuckGetValue(wii_accelY) ; 
    offsetZ = 127 - nunchuckGetValue(wii_accelZ) ;         
  }
  
  Serial.println("Labels,aX,aY,aZ,jX,jY,bC,bZ");
  Serial.print("Data,");
  printAccel(nunchuckGetValue(wii_accelX),offsetX) ; 
  printAccel(nunchuckGetValue(wii_accelY),offsetY) ; 
  printAccel(nunchuckGetValue(wii_accelZ),offsetZ) ; 
  printJoy(nunchuckGetValue(wii_joyX));
  printJoy(nunchuckGetValue(wii_joyY)); 
  printButton(nunchuckGetValue(wii_btnC));  
  printButton(nunchuckGetValue(wii_btnZ));      

  Serial.println();  // this indicates the end of line
}

// send acceleration mapped within the range of nunchuck values     
void printAccel(int value, int offset)
{
  Serial.print(adjReading(value, 127-50, 127+50, offset));
  Serial.print(",");
}

// send the joystick value
void printJoy(int value)
{
  Serial.print(adjReading(value,0, 255, 0));
  Serial.print(",");
}

//send 127 if button pressed, else 0 
void printButton(int value)
{
  if( value != 0)
     value = 127;
  Serial.print(value,DEC);
  Serial.print(",");
}

// map the reading to the given range, add offset value to calibrate if needed
int adjReading( int value, int min, int max, int offset)
{
   value = constrain(value + offset, min, max);
   value = map(value, min, max, -127, 127);
   return value;  
}    
    
